/**
 *  实现了 FiLIP 流密码，使用 f = XOR-THR，如论文
 * 'Transciphering, using FiLIP and TFHE for an efficient
 * delegation of computation' (https://eprint.iacr.org/2020/1373.pdf) 中所述。
 */

#ifndef __FiLIP_XOT_THR__
#define __FiLIP_XOT_THR__

#include <vector>
#include <random>

class FiLIP
{
    public:

        int len_sk; // 密钥的位数
        std::vector<int> sk; // 密钥

        int size_subset; // 用于加密每个位的密钥位数

        int size_domain_thr; // 阈值函数中添加的位数
        int threshold_limit; // 添加的位数与此限制进行比较

        int num_bits_xored; // 与阈值进行异或的位数。
                            // 我们有：size_subset = size_domain_thr + num_bits_xored

        std::vector<int> whitening; // 用于掩码置乱子集的随机位


        std::default_random_engine rand_engine;
        std::uniform_int_distribution<int> uni_sampler;


        FiLIP(int len_sk, int size_subset, int size_domain_thr, int threshold_limit);

        /** 
         *  置乱密钥，然后取其子集并与掩码向量进行异或。
         * 返回长度为 size_subset 的结果向量。
         */
        std::vector<int> subset_permut_whiten();


        /**
         *  使用初始化向量 iv 加密 msg 的每个条目，
         *  msg 应该是一个二进制向量。
         */
        std::vector<int> enc(long int iv, std::vector<int> msg);


        /**
         *  使用初始化向量 iv 解密 c 的每个条目，
         *  c 应该是一个二进制向量。
         */
        std::vector<int> dec(long int iv, std::vector<int> c);


        /**
         *  用于加密和解密的辅助函数。
         *  它接收密钥子集的置乱，
         * 表示为 x_1, x_2, ..., x_(n-k), y_1, ..., y_k，并输出
         *      x_1 XOR ... XOR x_(n-k) XOR THR(y_1, ..., y_k)
         * 其中 THR(y_1, ..., y_k) 如果 sum y_i < threshold_limit 则为 0，否则为 1。
         */
        int compute_xor_thr(std::vector<int> perm_subset_sk);


        int enc_bit(int b);
};


std::ostream& operator<<(std::ostream& os, const FiLIP& u);

#endif