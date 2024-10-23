#include "filip.h"
#include "utils.h"
#include "homomorphic_filip.h"
#include <cassert>
#include <iostream>
#include <vector>
//#include "vectorutils.hpp"


using namespace std;


// int bits_to_int(const vector<int>& bits)
// {
//     int pow2 = 1;
//     int res = 0;
//     for (int i = 0; i < bits.size(); i++){
//         res += bits[i] * pow2;
//         pow2 *= 2;
//     }
//     return res;
// }

// vector<int> int_to_bits(int m, int num_bits)
// {
//     vector<int> bits(num_bits);
//     for(int i = 0; i < num_bits; i++){
//         bits[i] = m % 2;
//         m /= 2;
//     }
//     return bits;
// }



// int main()
// {
//     srand(time(NULL));

//     int N = 1 << 14; // 密钥的长度
//     int n = 144;    // 用于加密每个位的子集大小
//     int k = 63;     // 阈值函数中添加的位数
//     int d = 35;     // 阈值限制
//     FiLIP filip(N, n, k, d);

//     cout << filip << endl;

//     HomomorphicFiLIP homFilip(filip);
//     cout << "HomomorphicFiLIP homFilip(filip);" << endl;
//     cout << homFilip << endl;

//     long int iv = 1234567;

//     int N_TESTS = 50;
//     int size_msg = 10;
//     float avg_time = 0.0;
//     for(int _i = 0; _i < N_TESTS; _i++){
//         // 创建一个随机消息
//         // vector<int> m(size_msg);
//         // for(int i = 0; i < size_msg; i++)
//         //     m[i] = rand() % 2;
//         int m = rand() % 1024;
//         vector<int> m_bits = int_to_bits(m, size_msg);

//         // 使用 FiLIP 加密 m
//         vector<int> c = filip.enc(iv, m_bits);

//         // 同态运行 FiLIP::解密，因此 c_lwe 必须加密 m
//         auto start = clock();
//         vector<Ctxt_LWE> c_lwe = homFilip.transform(iv, c);
//         avg_time += float(clock()-start)/CLOCKS_PER_SEC;

//         // 运行 LWE 解密。因此，dec_m 必须等于 m
//         vector<int> dec_m = homFilip.dec(c_lwe);

//         int dec_m_int = bits_to_int(dec_m);


//     //    cout << "    m  = " << m << endl;
//     //    cout << " m_bits = " << m_bits << endl;
//     //    cout<<"dec(c_bits) = "<<dec_m<<endl;
//     //    cout << "dec(c) = " << dec_m_int << endl;

//         assert(m == dec_m_int);

//         if (0 == _i % 10)
//             cout << "噪声: " << homFilip.noise(c_lwe, m_bits) << endl;
//     }
//     cout << "同态解密的平均时间: "
//          << avg_time/N_TESTS << " 秒" << endl;
//     cout << "同态解密每个位的平均时间: "
//          << (avg_time/N_TESTS) / size_msg << " 秒" << endl;

//     return 0;
// }