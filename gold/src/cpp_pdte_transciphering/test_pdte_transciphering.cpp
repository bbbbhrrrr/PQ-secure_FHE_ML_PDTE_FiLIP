/*********************************************************************************
*   使用 FINAL 库实现 PDTE 加转密。
*   所有树都存储为名为 data 的目录中的 JSON 文件。
*   
*   首先，我们运行同态比较（使用我们的递归分组比较算法）来计算树的每个内部节点的加密控制位。
*   然后，我们使用 FINAL::AND 门运行树遍历。
********************************************************************************/


#include "final/FINAL.h"
#include "node.h"
#include "utils.h"
#include <codecvt>
#include <fstream>
#include <iostream>
#include <assert.h>
#include <type_traits>
#include <stdlib.h>
#include "filip.h"
#include "homomorphic_filip.h"
#include <cassert>
#include <vector>

using namespace std;

// void print(const vector<Node*>& u){
// 	for (unsigned int i = 0; i < u.size(); i++){
// 		print_node(*(u[i]));
// 	}
// }

// 接收加密文本 c 加密 m。
// 返回加密文本，加密比较结果 m > v
Ctxt_LWE greater_than(const SchemeLWE& fhe, const vector<Ctxt_LWE>& c, const vector<int>& v)
{
    int num_bits = c.size();
    vector<int> not_v(num_bits);
    for(int i = 0; i < num_bits; i++)
        not_v[i] = 1 - v[i];

    Ctxt_LWE res, tmp, x;
    fhe.encrypt(x, 1);
    if (1 == not_v[num_bits-1]){
        res = c[num_bits-1];
    }else{
        fhe.encrypt(res, 0);
    }

    for(int i = num_bits-2; i >= 0; i--){
        // compute tmp = x_{i+1} := v[i+1]*c[i+1] + not(v[i+1]) * not(c[i+1]) \in {0,1}
        if (v[i+1])
            tmp = c[i+1];
        else
            fhe.not_gate(tmp, c[i+1]);
        // update x = x_{num_bits} * x_{num_bits-1} * ... * x_i
        fhe.and_gate(x, x, tmp);
        if (not_v[i]){
            fhe.and_gate(tmp, x, c[i]); // tmp = x * c[i]
            res = (res + tmp);
        }
    }
    return res;
}

Ctxt_LWE less_or_equal(const SchemeLWE& fhe, const vector<Ctxt_LWE>& c, const vector<int>& v)
{
    Ctxt_LWE res = greater_than(fhe, c, v);
    fhe.not_gate(res, res);
    return res;
}

// 假设 a 和 b 都是比特
const Ctxt_LWE& xnor(const Ctxt_LWE& a, const Ctxt_LWE& not_a, int b){
    if (b)
        return a;
    return not_a;
}



std::map<int, Ctxt_LWE> R1;
std::map<int, Ctxt_LWE> X1;

/**
 * 比较整数 m 与所有整数 v[0], v[1], ...
 * 假设所有整数都在 0 到 2^nbits - 1 之间，其中 nbits = bits_m.size()。
 * 结果存储在 R1 中，即调用此函数后，
 * R1[v[i]] 存储 (m > v[i]) 的结果。
 * 返回评估中使用的同态门的数量。
*/
int grouped_comp(const SchemeLWE& fhe, const vector<Ctxt_LWE>& bits_m, const vector<int>& v)
{
    int nbits = bits_m.size();
    Ctxt_LWE zero;
    fhe.encrypt(zero, 0);
    Ctxt_LWE tmp;
    vector<Ctxt_LWE> bits_not_m(nbits);
    for(int i = 0; i < nbits; i++)
        fhe.not_gate(bits_not_m[i], bits_m[i]);

    R1 = {
            {0, bits_m[nbits-1] },
            {1, zero }
        };
    X1 = {
            {0, bits_not_m[nbits-1] },
            {1, bits_m[nbits-1] }    
        };


int num_and_gates = 0;
int k = nbits-2;
while (k >= 0){
    std::map<int, Ctxt_LWE> R;
    std::map<int, Ctxt_LWE> X;
    for(int i = 0; i < v.size(); i++){
        int vk = v[i] >> k; // v[i] 的 nbits-k 个最高有效位
        int vk1 = v[i] >> (k+1); // v[i] 的 nbits-k+1 个最高有效位
        int kth_bit = vk % 2; // v[i] 的第 k 位
        if (0 == X.count(vk)){
            fhe.and_gate(X[vk], xnor(bits_m[k], bits_not_m[k], kth_bit), X1[vk1]); // X[vk] = XNOR() * X1[vk1]
            // 现在计算 R[vk] = R1[vk1] + X1[vk1] * (1-kth_bit) * bits_m[k] 
            if (kth_bit){
                R[vk] = R1[vk1];
                num_and_gates += 1;
            }else{
                fhe.and_gate(tmp, X1[vk1], bits_m[k]);
                R[vk] = tmp + R1[vk1];
                num_and_gates += 2;
            }
        }
    }
    X1 = X;
    R1 = R;
    k -= 1;
}
return num_and_gates;
}


int BOUND_RECURSION = 4;


int rec_split_grouped_comp(const SchemeLWE& fhe, const vector<Ctxt_LWE>& bits_m, const vector<int>& v, bool compX = true)
{

    int nbits = bits_m.size();
    if (nbits <= log(v.size())/log(2))
        return grouped_comp(fhe, bits_m, v);
    
    int k = floor(nbits / 2);
    int two_to_k = 1 << k;
    // XXX TODO 使用指针以避免多次复制密文
    vector<Ctxt_LWE> lsb_m(k);
    vector<Ctxt_LWE> msb_m(nbits - k);
    for(int i = 0; i < k; i++)
        lsb_m[i] = bits_m[i];
    for(int i = 0; i < nbits-k; i++)
        msb_m[i] = bits_m[k + i];

    vector<int> lsb_v(v.size());
    vector<int> msb_v(v.size());
    for(int i = 0; i < v.size(); i++)
        lsb_v[i] = v[i] % two_to_k;
    for(int i = 0; i < v.size(); i++)
        msb_v[i] = v[i] >> k;

//    for(int i = 0; i < v.size(); i++){
//        assert(v[i] == lsb_v[i] + two_to_k * msb_v[i]);
//    }

    int msb_num_ands = rec_split_grouped_comp(fhe, msb_m, msb_v);
    auto msbR = R1;
    auto msbX = X1;
    int lsb_num_ands = rec_split_grouped_comp(fhe, lsb_m, lsb_v);
    auto lsbR = R1;
    auto lsbX = X1;

    std::map<int, Ctxt_LWE> R;
    std::map<int, Ctxt_LWE> X;
    Ctxt_LWE tmp;

    int num_ands_X = 0;
    for(int i = 0; i < v.size(); i++){
        int vi = v[i];
        int msb_vi = msb_v[i];
        int lsb_vi = lsb_v[i];
        // R[vi] = (msbR[ msb_vi ]) OR (msbX[ msb_vi ] 和 lsbR[ lsb_vi ])
        // 但我们实际上可以使用加法代替 OR 门，因为只有一个子句可以为真
        if (0 == X.count(vi)){
            // R[vi] = msbR[ msb_vi ] + msbX[ msb_vi ] 和 lsbR[ lsb_vi ]
            fhe.and_gate(tmp, msbX[ msb_vi ], lsbR[ lsb_vi ]);
            R[vi] = msbR[ msb_vi ] + tmp;
            num_ands_X += 1;
            if(compX){
                //X[vi] = msbX[ msb_vi ] 和 lsbX[ lsb_vi ]
                fhe.and_gate(X[vi], msbX[ msb_vi ], lsbX[ lsb_vi ]);
                num_ands_X += 1;
            }
        }
    }
    int total_gates = msb_num_ands + lsb_num_ands + num_ands_X;
    R1 = R;
    X1 = X;
    return total_gates;
}


int bits_to_int(const vector<int>& bits)
{
    int pow2 = 1;
    int res = 0;
    for (int i = 0; i < bits.size(); i++){
        res += bits[i] * pow2;
        pow2 *= 2;
    }
    return res;
}

vector<int> int_to_bits(int m, int num_bits)
{
    vector<int> bits(num_bits);
    for(int i = 0; i < num_bits; i++){
        bits[i] = m % 2;
        m /= 2;
    }
    return bits;
}


void print_R1(){
    for (auto it = R1.begin(); it != R1.end(); ++it)
           std::cout << it->first << " => " << it->second.b << endl;
}

void test_comparisons() {
    int num_bits = 10; // 将要比较的值的位长度
    int upper_bound = 1 << num_bits; // 最大值
    int n = 50;    // 将要与密文进行比较的明文数量
        
    SchemeLWE fhe;

    int N_TESTS = 5;
    double avg_time = 0.0;
    double avg_gates = 0.0;
    for(int _i = 0; _i < N_TESTS; _i++){
        cout << "# 测试 " << _i << endl;
        // 我们想要比较 enc(m) 和 v
        int m = rand() % upper_bound;
        vector<int> bits_m(num_bits); // m[0] 是 m 的最低有效位
        bits_m = int_to_bits(m, num_bits);
        vector<Ctxt_LWE> c(num_bits);
        for(int i = 0; i < num_bits; i++){
            fhe.encrypt(c[i], bits_m[i]);
        }
//        int m = bits_to_int(bits_m);
//        int v = bits_to_int(bits_v);
//        cout << "bits_m = " << bits_m << endl;
//        cout << "m = " << m << endl;


        vector<int> v(n); // 将要与 m 进行比较的值
        for(int i = 0; i < n; i++){
            v[i] = rand() % upper_bound;
        }
//        sort(v.begin(), v.end());
//        cout << "v = " << v << endl;

        auto start = clock();
//        int num_and_gates = grouped_comp(fhe, c, v);
        cout << "int num_and_gates = rec_split_grouped_comp(fhe, c, v);" << endl;
        int num_and_gates = rec_split_grouped_comp(fhe, c, v);

        avg_time += float(clock()-start)/CLOCKS_PER_SEC;

        avg_gates += num_and_gates;

        for (int i = 0; i < v.size(); i++){
            const Ctxt_LWE& res = R1[v[i]];
            int dec_comp = fhe.decrypt(res);

//            cout << "v[" << i << "] = " << v[i] << endl;
//            cout << "R1[v[i]].count() = " << R1.count(v[i]) << endl;
//            cout << "dec_comp = " << dec_comp << endl;
//            cout << endl;
            assert(dec_comp == (m > v[i]));
        }

    }

    cout << "同态比较 m 和 " << n 
         << " 个 " << num_bits << " 位整数。" << endl;
    cout << "同态比较的平均时间: " 
         << avg_time/N_TESTS << " 秒" << endl;
    cout << "同态门的平均数量: " << avg_gates / N_TESTS << endl;
}

// 假设每个内部节点的控制位已经设置好

void traverse_rec(unsigned &out, const Node& node, const std::vector<unsigned int> &features, unsigned parent) {
    if (node.is_leaf()) {
        out += node.class_leaf * parent;
        cout << "out = " << out << endl;
    } else {
        if (node.op == "leq") {
            if (features[node.feature_index] <= node.threshold) {
                traverse_rec(out, *node.left, features, parent);
                traverse_rec(out, *node.right, features, parent * (1 - parent));
            } else {
                traverse_rec(out, *node.left, features, parent * (1 - parent));
                traverse_rec(out, *node.right, features, parent);
            }
        } else {
            // 未实现
            assert(false);
        }
    }
}

// num_bits 是将要比较的值的位长度
void test_json_tree(string filename, int num_bits, bool verbose) {
    int upper_bound = 1 << num_bits; // 最大值
    
    SchemeLWE fhe;

//    std::ifstream ifs("data/phoneme_10bits/model.json");
    std::ifstream ifs(filename);
    json j = json::parse(ifs);
    auto root = Node(j);
    if (verbose)
        print_tree(root);

    vector< vector<Node*> > nodes_by_feat = nodes_by_feature(root);

    int num_features = nodes_by_feat.size();
    std::vector< std::vector< int > > thrs_by_feat = thresholds_by_feature(nodes_by_feat);

    int N_TESTS = 3;
    double avg_time = 0.0;
    for(int _i = 0; _i < N_TESTS; _i++){
        cout << "# 测试 " << _i << endl;
        // 我们想要比较 enc(m) 和 v
        vector<int> plain_features(num_features);
        vector<vector<int> > bits_m(num_features);
        vector<vector<Ctxt_LWE> > enc_bits(num_features); // enc_bits[i] == 第 i 个特征的位的加密
        for(int _j = 0; _j < num_features; _j++){
            plain_features[_j] = rand() % upper_bound;
            cout << "plain_features["<<_j<<"] = " << plain_features[_j] << endl;
            bits_m[_j] = vector<int>(num_bits); // m[0] 是 m 的最低有效位
            bits_m[_j] = int_to_bits(plain_features[_j], num_bits);
            vector<Ctxt_LWE> c(num_bits);
            for(int i = 0; i < num_bits; i++){
                fhe.encrypt(c[i], bits_m[_j][i]);
            }
            enc_bits[_j] = c;
        }

        // 现在，比较每个特征
        for (int i = 0; i < num_features; i++){
            int m = plain_features[i];
            cout << "m = " << m << endl;
            vector<int>& v = thrs_by_feat[i];
            int n = v.size();    // 将要与密文进行比较的明文数量
            vector<Ctxt_LWE>& c = enc_bits[i];

            if (0 == n)
                continue;
            auto start = clock();
            float run_time;
            vector<Node*>& nodes = nodes_by_feat[i];
            if (n == 1){
                Ctxt_LWE enc_cmp = greater_than(fhe, c, int_to_bits(v[0], num_bits));
                nodes[0]->control_bit = enc_cmp;
                run_time = float(clock()-start)/CLOCKS_PER_SEC;
            } else {
                int num_and_gates = rec_split_grouped_comp(fhe, c, v);
                run_time = float(clock()-start)/CLOCKS_PER_SEC;
                avg_time += run_time;

                for (int j = 0; j < v.size(); j++){
                    const Ctxt_LWE& res = R1[v[j]];
                    int dec_comp = fhe.decrypt(res);

                    assert(dec_comp == (m > v[j]));
                }

                // 现在用比较结果更新控制位
                vector<Node*>& nodes = nodes_by_feat[i];
                for(int j = 0; j < n; j++){
                    nodes[j]->control_bit = R1[v[j]];
                }
            }

            cout << "同态比较特征["<<i<<"] 和 " << n    
            << " 个 " << num_bits << " 位整数。" << endl;
            cout << "运行时间 = " << run_time << endl;
        }
        // 现在每个特征都与相应的节点进行了比较，我们遍历树
        Ctxt_LWE out;
        auto start = clock();
        // traverse_rec(out, root, fhe, 1, plain_features);

        // 解密预测结果
        int prediction = fhe.decrypt(out);
        cout << "预测结果: " << prediction << endl;

        float run_time = float(clock()-start)/CLOCKS_PER_SEC;
        cout << "同态遍历时间: " << run_time << endl;
    }
}

// 用于模型预测的函数
// Function for model prediction

int model_predict(const string& model_file, const vector<unsigned int>& plain_features, int num_bits) {
    int upper_bound = 1 << num_bits; // 最大值
    SchemeLWE fhe;

    // 加载模型
    // Load the model
    std::ifstream ifs(model_file);
    json j = json::parse(ifs);
    Node root = Node(j);

    // 加密输入特征
    vector<vector<int>> bits_m(plain_features.size());
    cout << "plain_features.size() = " << plain_features.size() << endl;
    vector<vector<Ctxt_LWE>> enc_bits(plain_features.size()); // enc_bits[i] == i-th 特征的加密位

    for (int i = 0; i < plain_features.size(); i++) {
        bits_m[i] = int_to_bits(plain_features[i], num_bits);
        vector<Ctxt_LWE> c(num_bits);
        for (int j = 0; j < num_bits; j++) {
            fhe.encrypt(c[j], bits_m[i][j]);
        }
        enc_bits[i] = c;
    }

    // 比较每个特征并设置控制位
    // Now, compare each feature
    vector<vector<Node*>> nodes_by_feat = nodes_by_feature(root);
    vector<vector<int>> thrs_by_feat = thresholds_by_feature(nodes_by_feat);

    for (int i = 0; i < nodes_by_feat.size(); i++) {
        vector<int>& v = thrs_by_feat[i];
        vector<Ctxt_LWE>& c = enc_bits[i];
        int m = plain_features[i];
        cout << "m = " << m << endl;
        
        if (v.size() == 0) {
            continue;
        }
        if (v.size() == 1) {
            Ctxt_LWE enc_cmp = greater_than(fhe, c, int_to_bits(v[0], num_bits));
            nodes_by_feat[i][0]->control_bit = enc_cmp;
        } else {
            rec_split_grouped_comp(fhe, c, v);

            for (int j = 0; j < v.size(); j++){
                const Ctxt_LWE& res = R1[v[j]];
                int dec_comp = fhe.decrypt(res);

                assert(dec_comp == (m > v[j]));
            }

            for (int j = 0; j < v.size(); j++) {
                nodes_by_feat[i][j]->control_bit = R1[v[j]];
            }
        }

        
    }

    // 遍历树以获得预测结果
    // Now that every feature was compared to the corresponding nodes, we traverse the tree
    unsigned out = 0;
    unsigned parent = 1;
    traverse_rec(out, root, plain_features, parent);
    return out;

}




// // 用于模型预测的函数
// void filip_enc(const vector<unsigned int>& plain_features, int num_bits) {
//     int upper_bound = 1 << num_bits; // 最大值
//     SchemeLWE fhe;

//     int N = 1 << 14; // length of the secret key
//     int n = 144;    // size of subset used to encrypt each bit
//     int k = 63;     // number of bits added in the threshold function
//     int d = 35;          // threshold limit
//     FiLIP filip(N, n, k, d);

//     HomomorphicFiLIP homFilip(filip);
//     long int iv = 1234567;

//     int size_msg = 10;

//     // 加密输入特征
//     vector<vector<int>> bits_m(plain_features.size());
//     cout << "plain_features.size() = " << plain_features.size() << endl;
//     vector<vector<Ctxt_LWE>> enc_bits(plain_features.size()); // enc_bits[i] == i-th 特征的加密位
//     vector<vector<int>> enc_features(plain_features.size());

//     for (int i = 0; i < plain_features.size(); i++) {
//         bits_m[i] = int_to_bits(plain_features[i], num_bits);

//         // encipher m with FiLIP
//         vector<int> c_filip = filip.enc(iv, bits_m[i]);

//         vector<Ctxt_LWE> c_lwe = homFilip.transform(iv, c_filip);

//         enc_bits[i] = c_lwe;
//     }

//     // 输出enc_bits
//     cout << enc_bits << endl;
// }




void runTests()
{
    srand(time(NULL));

    bool verbose = false;

    string model_file = "/home/bbbbhrrrr/sortinghat/SortingHat/decision_tree.json";
    int num_bits = 10;
    while (true) {
        vector<unsigned int> plain_features;
        string input;
        cout << "请输入长度为8的特征向量（例如：480,341,239,47,368,3,382,569）：" << endl;
        getline(cin, input);

        stringstream ss(input);
        string item;
        while (getline(ss, item, ',')) {
            try {
                plain_features.push_back(stoi(item));
            } catch (const std::invalid_argument& e) {
                cout << "输入无效，请输入有效的整数。" << endl;
                plain_features.clear();
                break;
            }
        }

        if (plain_features.size() != 8) {
            cout << "特征向量长度不足8或输入无效，程序结束。" << endl;
            break;
        }

        int prediction = model_predict(model_file, plain_features, num_bits);
        cout << "预测结果: " << prediction << endl;

        char choice;
        cout << "是否继续测试？(y/n): ";
        cin >> choice;
        cin.ignore();
        if (choice != 'y' && choice != 'Y') {
            break;
        }
    }
}

// int main()
// {
//     srand(time(NULL));

//     bool verbose = false;

// //    test_json_tree("data/phoneme_10bits/model.json", 10, verbose);
// //    test_json_tree("data/electricity_10bits/model.json", 10, verbose);
// //    test_json_tree("data/fake_art_11bits/model.json", 11, verbose);
// //    test_json_tree("data/fake_art_16bits/model.json", 16, verbose);
// //    test_json_tree("data/fake_hou_11bits/model.json", 11, verbose);
// //    test_json_tree("data/fake_hou_16bits/model.json", 16, verbose);
// //    test_json_tree("data/spam_11bits/model.json", 11, verbose);

//     string model_file = "/home/bbbbhrrrr/sortinghat/SortingHat/decision_tree.json";
//     int num_bits = 10;
//     while (true) {
//         // 获取用户输入的特征值
//         vector<unsigned int> plain_features;
//         string input;
//         cout << "请输入长度为8的特征向量（例如：480,341,239,47,368,3,382,569）：" << endl;
//         getline(cin, input);

//         stringstream ss(input);
//         string item;
//         while (getline(ss, item, ',')) {
//             try {
//                 plain_features.push_back(stoi(item));
//             } catch (const std::invalid_argument& e) {
//                 cout << "输入无效，请输入有效的整数。" << endl;
//                 plain_features.clear();
//                 break;
//             }
//         }

//         if (plain_features.size() != 8) {
//             cout << "特征向量长度不足8或输入无效，程序结束。" << endl;
//             break;
//         }

//         // 进行预测
//         int prediction = model_predict_filip(model_file, plain_features, num_bits);
//         cout << "预测结果: " << prediction << endl;

//         // 询问用户是否继续
//         char choice;
//         cout << "是否继续测试？(y/n): ";
//         cin >> choice;
//         cin.ignore(); // 忽略换行符
//         if (choice != 'y' && choice != 'Y') {
//             break;
//         }
//     }

//     return 0;
// }
