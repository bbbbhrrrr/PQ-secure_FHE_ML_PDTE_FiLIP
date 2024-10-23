/**
 *  实现了 FiLIP 流密码的同态版本，
 *  使用 f = XOR-THR，如论文
 * 'Transciphering, using FiLIP and TFHE for an efficient delegation of computation' 
 * (https://eprint.iacr.org/2020/1373.pdf) 中所述。
 */

#ifndef __HOM_FiLIP_XOT_THR__
#define __HOM_FiLIP_XOT_THR__

#include "filip.h"

#include "final/FINAL.h"

class HomomorphicFiLIP
{
    public:

        SchemeLWE fhe;

        std::vector<Ctxt_LWE> enc_sk;
        std::vector<Ctxt_LWE> enc_not_sk;
        std::vector<NGSFFTctxt> enc_X_to_sk;
        std::vector<NGSFFTctxt> enc_X_to_not_sk;

        std::vector<Ctxt_LWE> subset_enc_sk;
        std::vector<NGSFFTctxt> subset_enc_X_to_sk;

        int len_sk; // 密钥的位数
        int size_subset; // 用于加密每个位的密钥位数

        int size_domain_thr; // 阈值函数中添加的位数
        int threshold_limit; // 添加的位数与此限制进行比较

        int num_bits_xored; // 与阈值进行异或的位数。
                            // 我们有：size_subset = size_domain_thr + num_bits_xored

        int B_ngs; // NGS 向量密文中使用的分解基数。
        int l_ngs; // NGS 向量密文的维度。l_ngs = log_{B_ngs}(Q)
        int log_B_ngs; // 以 2 为底的 log(B_ngs)


        std::default_random_engine rand_engine;
        std::uniform_int_distribution<int> uni_sampler;

        std::vector<int> whitening; // 用于掩码置乱子集的随机位

        HomomorphicFiLIP(const FiLIP& filip);

        /**
         *  将 FiLIP 流密码的密钥加密为 FHE 密文。
         */
        void encrypt_sk(const vector<int>& sk);


        /**
         *  选择密钥加密位的随机子集
         * 并应用掩码。
         *  然后，更新变量 subset_enc_sk 和 subset_enc_X_to_sk
         * 以存储指向此置乱和掩码子集的指针。
         */
        void subset_permut_whiten();


        /**
         *  接收初始化向量 iv，该向量先前由 
         * FiLIP 用于在密钥 sk 下将二进制向量 m 加密为密文 c。
         *  使用 iv 和 sk 的加密同态评估 
         * FiLIP 的解密函数，生成加密 m 的 LWE 密文向量。
         */
        std::vector<Ctxt_LWE> transform(long int iv, const std::vector<int>& c);


        /**
         *  接收 LWE 密文向量并解密它们。
         */
        std::vector<int> dec(std::vector<Ctxt_LWE> c);

        /**
         *  接收 LWE 密文向量及其加密的消息。
         * 计算每个密文的噪声对数
         * 并返回这些对数的最大值。
         */
        double noise(const std::vector<Ctxt_LWE>& c, const std::vector<int>& m);


        /**
         *  用于加密和解密的辅助函数。
         *  它接收密钥子集的置乱，
         * 表示为 x_1, x_2, ..., x_(n-k), y_1, ..., y_k，并输出
         *      x_1 XOR ... XOR x_(n-k) XOR THR(y_1, ..., y_k)
         * 其中 THR(y_1, ..., y_k) 如果 sum y_i < threshold_limit 则为 0，否则为 1。
         */
        Ctxt_LWE compute_xor_thr();


        /**
         *  同态应用 FiLIP 加密到输入的比特 ci。
         *  此函数由 HomomorphicFiLIP::transform 使用。
         */
        Ctxt_LWE enc_bit(int ci);
};

std::ostream& operator<<(std::ostream& os, const HomomorphicFiLIP& u);

#endif