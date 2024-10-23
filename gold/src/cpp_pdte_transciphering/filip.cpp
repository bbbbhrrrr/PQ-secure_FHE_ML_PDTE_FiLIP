#include "filip.h"
#include "utils.h"
#include "final/FINAL.h"
//#include "vectorutils.hpp"

using namespace std;

FiLIP::FiLIP(int len_sk, int size_subset, int size_domain_thr, int threshold_limit)
    : len_sk(len_sk), size_subset(size_subset), size_domain_thr(size_domain_thr),
        threshold_limit(threshold_limit), 
        num_bits_xored(size_subset - size_domain_thr),
        uni_sampler(0, len_sk)
{

    sk = vector<int>(len_sk);
    Sampler::get_binary_vector(sk);

    whitening = vector<int>(size_subset);
}

// 生成子集并进行置乱和掩码处理
vector<int> FiLIP::subset_permut_whiten()
{
    vector<int> v(sk); // 复制密钥
    vector<int> res(size_subset);

    // 采样掩码
    for(int i = 0; i < size_subset; i++)
        whitening[i] = uni_sampler(rand_engine) % 2;

    // 置乱
    shuffle(v, rand_engine, uni_sampler);

    // 取子集并应用掩码
    for(int i = 0; i < res.size(); i++){
        res[i] = v[i] ^ whitening[i];
    }
     
    return res;
}

// 计算异或和阈值
int FiLIP::compute_xor_thr(vector<int> perm_subset_sk)
{
    int _xor = 0;
    int i;
    // 对 perm_subset_sk 的前几个比特进行异或
    for(i = 0; i < num_bits_xored; i++)
        _xor = (_xor + perm_subset_sk[i]) % 2;

    // 对 perm_subset_sk 的后几个比特求和
    int sum = 0;
    for(; i < size_subset; i++)
        sum += perm_subset_sk[i];
    int T_d_n = (sum < threshold_limit ? 0 : 1);
    return _xor ^ T_d_n; // XOR(x_1, ..., x_k) 异或 T_{d,n}(y_1, ..., y_n) 
}

// 加密单个比特
int FiLIP::enc_bit(int b)
{

    assert(0 == b || 1 == b);

    vector<int> permuted_subset = subset_permut_whiten();

    int f_x_y = compute_xor_thr(permuted_subset);

    return (b + f_x_y) % 2;
}

/**
*  使用初始化向量 iv 加密 msg 的每个条目，
*  msg 应该是一个二进制向量。
*/
std::vector<int> FiLIP::enc(long int iv, std::vector<int> msg)
{
    rand_engine = default_random_engine(iv); // 使用给定的 iv 重置种子

    vector<int> ctxt(msg.size());

    for(int i = 0; i < msg.size(); i++)
        ctxt[i] = enc_bit(msg[i]);
    return ctxt;
}

// 解密
std::vector<int> FiLIP::dec(long int iv, std::vector<int> c)
{
    return enc(iv, c); // 解密和加密实际上是相同的
}

// 重载输出运算符
std::ostream& operator<<(std::ostream& os, const FiLIP& u){
    os << "FiLIP: {" 
        << "len_sk: " << u.len_sk
        << ", size_subset: " << u.size_subset
        << ", size_domain_threshold: " << u.size_domain_thr
        << ", threshold_limit: " << u.threshold_limit 
        << "}";
    return os;
}