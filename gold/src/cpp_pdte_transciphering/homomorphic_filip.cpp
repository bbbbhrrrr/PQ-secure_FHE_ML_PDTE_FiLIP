#include "filip.h"
#include "utils.h"
#include "homomorphic_filip.h"
#include "final/FINAL.h"

using namespace std;

HomomorphicFiLIP::HomomorphicFiLIP(const FiLIP& f)
    : len_sk(f.len_sk), size_subset(f.size_subset), size_domain_thr(f.size_domain_thr),
        threshold_limit(f.threshold_limit), 
        num_bits_xored(f.size_subset - f.size_domain_thr),
        uni_sampler(0, f.len_sk)
{
    log_B_ngs = 7; 
    B_ngs = 1 << log_B_ngs;
    l_ngs = ceil(log(q_boot) / log(B_ngs));

    auto start = clock();
    encrypt_sk(f.sk);
    cout << "使用 FHE 加密 FiLIP 的 sk 所需时间: " << float(clock()-start)/CLOCKS_PER_SEC << " 秒" << endl;

    // 初始化向量，用于存储在 FiLIP::解密期间使用的置乱和掩码的子集
    subset_enc_sk = std::vector<Ctxt_LWE>(size_subset);
    subset_enc_X_to_sk = std::vector<NGSFFTctxt>(size_subset);

    whitening = vector<int>(size_subset);
}

// 加密密钥
void HomomorphicFiLIP::encrypt_sk(const vector<int>& sk){
    int q = parLWE.q_base;
    // 将 sk 的每个位加密为 Delta = q/2 的 LWE 密文，
    // 即 (a, b) \in Z_q^(n+1)，其中
    // b = a*s + e + (q/2) * sk[i]
    enc_sk = std::vector<Ctxt_LWE>(len_sk);
    for(int i = 0; i < len_sk; i++){
        fhe.encrypt(enc_sk[i], 0);
        enc_sk[i].b += (q/2) * sk[i];
        parLWE.mod_q_base(enc_sk[i].b);
    }

    // 预计算 LWE::enc(NOT(sk[i]))
    enc_not_sk = std::vector<Ctxt_LWE>(len_sk);
    for(int i = 0; i < len_sk; i++){
        enc_not_sk[i].a = enc_sk[i].a;
        enc_not_sk[i].b = enc_sk[i].b + (q/2);
        parLWE.mod_q_base(enc_not_sk[i].b);
    }
    
    enc_X_to_sk = std::vector<NGSFFTctxt>(len_sk);
    enc_X_to_not_sk = std::vector<NGSFFTctxt>(len_sk);
    for(int i = 0; i < len_sk; i++){
        ModQPoly msg(Param::N, 0L);
        ModQPoly not_msg(Param::N, 0L);
        if(1 == sk[i]){
            msg[1] = 1; // msg = X^sk[i]
            not_msg[0] = 1; // not_msg = X^(not sk[i])
        }else{
            msg[0] = 1; // msg = X^sk[i]
            not_msg[1] = 1; // not_msg = X^(not sk[i])
        }
        enc_ngs(enc_X_to_sk[i], msg, l_ngs, B_ngs, fhe.sk_boot);
        enc_ngs(enc_X_to_not_sk[i], not_msg, l_ngs, B_ngs, fhe.sk_boot);
    }
}

// 生成子集并进行置乱和掩码处理
void HomomorphicFiLIP::subset_permut_whiten()
{
    // 采样掩码
    for(int i = 0; i < size_subset; i++)
        whitening[i] = uni_sampler(rand_engine) % 2;

    vector<int> indexes(len_sk);
    for(int i = 0; i < len_sk; i++)
        indexes[i] = i;
    shuffle(indexes, rand_engine, uni_sampler);

    // 现在，indexes 表示随机置乱，即如果 indexes[i] = j，
    // 则置乱子集的第 i 个元素是原始集合的第 j 个元素

    // 复制置乱的密钥子集并应用掩码
    int q = parLWE.q_base;
    for(int i = 0; i < size_subset; i++){
        int j = indexes[i];
        int wi = whitening[i];

        // 复制 LWE.enc( XOR(sk[j], wi) ) 和 NGS.enc( X^XOR(sk[j], wi) )
        if (0 == wi){
            subset_enc_sk[i] = enc_sk[j];
            subset_enc_X_to_sk[i] = enc_X_to_sk[j];
        }else{
            subset_enc_sk[i] = enc_not_sk[j];
            subset_enc_X_to_sk[i] = enc_X_to_not_sk[j];
        }
    }
}

// 获取阈值测试向量
ModQPoly get_test_vector_for_threshold(int N, int d, int Q)
{
    ModQPoly t(N); // t(X) = sum_{i=0}^{d-1} 0 * X^{2N - i}+sum_{i=d}^{N-1} 1 * X^{2N - i}
                   //      = sum_{i=0}^{d-1} 0 * X^{N - i} - sum_{i=d}^{N-1} X^{N - i}
                   //      = - X^1 - X^2 - ... - X^{N-d}
    t[0] = 0;
    for(int i = N-d+1; i < N; i++)
        t[i] = 0;
    for(int i = 1; i <= N-d; i++)
        t[i] = -(Q/2); // 实际上我们定义的是 (Q/2) * t(X) 而不是 t(X)

    return t;
}

// 计算异或和阈值
Ctxt_LWE HomomorphicFiLIP::compute_xor_thr()
{
    Ctxt_LWE _xor = subset_enc_sk[0]; // 复制第一个置乱位的加密
    int i;
    // 对置乱的密钥子集的前几个比特进行异或
    for(i = 1; i < num_bits_xored; i++)
        _xor = (_xor + subset_enc_sk[i]); // XXX: 在 FINAL 中实现 += 并替换此行

    // 对置乱的密钥子集的后几个比特求和
    int N = Param::N;
    int Q = q_boot; // NGS 方案中使用的 Q（累加器）

    ModQPoly acc = get_test_vector_for_threshold(N, threshold_limit, Q);


    vector<long> tmp_acc_long(N);
    for(; i < size_subset; i++){
        // tmp_acc_long = decompose(acc) * subset_enc_X_to_sk[i]
        external_product(tmp_acc_long, acc, subset_enc_X_to_sk[i], B_ngs, log_B_ngs, l_ngs);
        // acc = tmp_acc_long mod Q
        mod_q_boot(acc, tmp_acc_long);
    }
    // 现在 acc 加密了 t(X) * X^(sk[num_bits_xored] + ... + sk[size_subset-1])
    // 其中 t(X) 是对应于阈值函数的测试多项式。

    // 从 Q 模切换到 q_base
    modulo_switch_to_base_lwe(acc);
    
    // 密钥切换
    Ctxt_LWE ct;
    fhe.key_switch(ct, acc);
    // 现在 ct 是一个 LWE 密文，加密了阈值 T_{d, n}(y_1, ..., y_n)
    // delta 等于 q/2。

    return _xor + ct; // enc( XOR(x_1, ..., x_k) xor T_{d,n}(y_1, ..., y_n) )
}

// 加密单个比特
Ctxt_LWE HomomorphicFiLIP::enc_bit(int ci)
{
    assert(0 == ci || 1 == ci);

    this->subset_permut_whiten();

    // 计算 f(x, y) = XOR(x) + THR(y) % 2 的加密
    Ctxt_LWE f_x_y = compute_xor_thr();
    
    // 将其与加密的比特 c_i 进行异或，以获得 m_i 的 LWE 加密
    int q = parLWE.q_base;
    f_x_y.b += ci * (q/2);
    parLWE.mod_q_base(f_x_y.b);

    return f_x_y;
}

// 转换
std::vector<Ctxt_LWE> HomomorphicFiLIP::transform(long int iv, const std::vector<int>& c)
{
    rand_engine = default_random_engine(iv); // 使用给定的 iv 重置种子

    vector<Ctxt_LWE> ctxt(c.size());

    for(int i = 0; i < c.size(); i++){
        ctxt[i] = enc_bit(c[i]);
    }
    return ctxt;
}

// 解密
std::vector<int> HomomorphicFiLIP::dec(std::vector<Ctxt_LWE> c)
{
    vector<int> m(c.size()); 
    for(int i = 0; i < m.size(); i++)
        // 我们不能简单地返回 SchemeLWE::decrypt(c[i])，因为 SchemeLWE 
        // 假设加密的消息乘以 q/4，
        // 但在我们的情况下，它乘以 q/2，所以在 SchemeLWE::decrypt 中，
        // 当我们乘以 4/q 时，我们得到 (4/q) * (q/2) * m = 2*m 而不是 m。
        // 因此，我们在这里通过将 2*m 映射回二进制消息来修复它。
        m[i] = (fhe.decrypt(c[i]) == 0 ? 0 : 1);
    return m;
}

// 计算噪声
double HomomorphicFiLIP::noise(const std::vector<Ctxt_LWE>& c, const std::vector<int>& m)
{
    int q = parLWE.q_base;
    double noise = 0;
    for(int i = 0; i < c.size(); i++){
        Ctxt_LWE ct = c[i];
        ct.b -= (q/2) * m[i];
        ct.b = parLWE.mod_q_base(ct.b); // 现在 ct 加密了零
                                        // 这是必要的，因为 fhe.noise 
                                        // 假设 m 乘以 q/4，但这里 c 使用 q/2
        double noise_i = fhe.noise(ct, 0);
        if (noise_i > noise)
            noise = noise_i;
    }
    return noise;
}

// 重载输出运算符
std::ostream& operator<<(std::ostream& os, const HomomorphicFiLIP& u){
    os << "HomomorphicFiLIP: {" 
        << "len_sk: " << u.len_sk
        << ", size_subset: " << u.size_subset
        << ", size_domain_threshold: " << u.size_domain_thr
        << ", threshold_limit: " << u.threshold_limit 
        << ", FINAL: {"
        << " l_ngs: " << u.l_ngs
        << ", log_B_ngs: " << u.log_B_ngs
        << ", N: " << parLWE.N
        << ", logQ: " << ceil(log(q_boot) / log(2))
        << ", n: " << parLWE.n
        << ", logq: " << ceil(log(parLWE.q_base) / log(2))
        << "} }";
    return os;
}