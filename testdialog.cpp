#include "testdialog.h"
#include <QMessageBox>
#include <sstream>
#include <stdexcept>
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

// 声明 model_predict 函数
int model_predict(const std::string& model_file, const std::vector<unsigned int>& plain_features, int num_bits);

// 声明 int_to_bits 函数
std::vector<int> int_to_bits(int m, int num_bits);

TestDialog::TestDialog(QWidget *parent) :
    QDialog(parent)
{
    ui = new TestDialogUI();
    ui->setupUi(this);

    connect(ui->runTestButton, &QPushButton::clicked, this, &TestDialog::on_runTestButton_clicked);
    connect(ui->encryptButton, &QPushButton::clicked, this, &TestDialog::on_encryptButton_clicked);
    
    // 设置 consoleTextEdit 的输出流
    ui->encryptOutputTextEdit->setOutputStream(std::cout);
}

TestDialog::~TestDialog()
{
    delete ui;
}

void TestDialog::on_runTestButton_clicked()
{
    QString input_csv_path = ui->inputCsvPathLineEdit->text();
    QString output_csv_path = ui->outputCsvPathLineEdit->text();
    std::string input = input_csv_path.toStdString();
    std::string output = output_csv_path.toStdString();

    // 获取选择的模型
    QString selectedModel = ui->modelComboBox->currentText();

    if (selectedModel == "breast") {
        runModel1(input, output);
    } else if (selectedModel == "electricity") {
        runModel2(input, output);
    } else if (selectedModel == "phoneme") {
        runModel3(input, output);
    } else if (selectedModel == "spam") {
        runModel4(input, output);
    } else {
        QMessageBox::warning(this, "模型错误", "请选择一个模型。");
    }
}

void TestDialog::on_encryptButton_clicked()
{
    ui->encryptOutputTextEdit->clearBuffer();

    std::vector<unsigned int> plain_features;
    QString input = ui->encryptInputLineEdit->text();
    std::stringstream ss(input.toStdString());
    std::string item;
    while (std::getline(ss, item, ',')) {
        try {
            plain_features.push_back(std::stoi(item));
        } catch (const std::invalid_argument& e) {
            QMessageBox::warning(this, "输入无效", "请输入有效的整数。");
            plain_features.clear();
            return;
        }
    }

    if (plain_features.size() != 8) {
        QMessageBox::warning(this, "输入无效", "特征向量长度不足8或输入无效，程序结束。");
        return;
    }

    filip_enc(plain_features, 10);
    ui->encryptOutputTextEdit->append(QString::fromStdString(ui->encryptOutputTextEdit->getBufferContent()));
}

void TestDialog::runModel1(const std::string& input_csv_path, const std::string& output_csv_path)
{
    // 模型1的实现
    model_file = ":/sortinghat/SortingHat/src/cpp_pdte_transciphering/data/breast_11bits/model.json";
    num_bits = 11;
    runTests(input_csv_path, output_csv_path, model_file, num_bits);
}

void TestDialog::runModel2(const std::string& input_csv_path, const std::string& output_csv_path)
{
    // 模型2的实现
    model_file = ":/sortinghat/SortingHat/src/cpp_pdte_transciphering/data/electricity_10bits/model.json";
    num_bits = 10;
    runTests(input_csv_path, output_csv_path, model_file, num_bits);
}

void TestDialog::runModel3(const std::string& input_csv_path, const std::string& output_csv_path)
{
    // 模型3的实现
    model_file = ":/sortinghat/SortingHat/src/cpp_pdte_transciphering/data/phoneme_10bits/model.json";
    num_bits = 10;
    runTests(input_csv_path, output_csv_path, model_file, num_bits);
}

void TestDialog::runModel4(const std::string& input_csv_path, const std::string& output_csv_path)
{
    // 模型4的实现
    model_file = ":/sortinghat/SortingHat/src/cpp_pdte_transciphering/data/spam_11bits/model.json";
    num_bits = 11;
    runTests(input_csv_path, output_csv_path, model_file, num_bits);
}

void TestDialog::runTests(const std::string& input_csv_path, const std::string& output_csv_path,const std::string& model_file_path, int model_num_bits)
{
    model_file = model_file_path;
    num_bits = model_num_bits;

    std::ifstream input_file(input_csv_path);
    if (!input_file.is_open()) {
        QMessageBox::warning(this, "文件错误", "无法打开输入文件。");
        return;
    }

    std::ofstream output_file(output_csv_path);
    if (!output_file.is_open()) {
        QMessageBox::warning(this, "文件错误", "无法打开输出文件。");
        return;
    }

    std::string line;
    while (std::getline(input_file, line)) {
        std::stringstream ss(line);
        std::string item;
        std::vector<unsigned int> plain_features;

        while (std::getline(ss, item, ',')) {
            try {
                plain_features.push_back(std::stoi(item));
            } catch (const std::invalid_argument& e) {
                QMessageBox::warning(this, "输入无效", QString::fromStdString("无效的输入: " + item));
                continue;
            }
        }

        int prediction = model_predict(model_file, plain_features, num_bits);
        output_file << prediction << std::endl;
    }

    input_file.close();
    output_file.close();

    QMessageBox::information(this, "完成", "预测已完成，结果已保存到输出文件。");
}

void TestDialog::filip_enc(const std::vector<unsigned int>& plain_features, int num_bits)
{
    // 重定向 std::cout 到 consoleTextEdit
    ui->encryptOutputTextEdit->setOutputStream(std::cout);

    std::cout << "开始加密..." << std::endl;

    // 这里是加密函数的实现
    int upper_bound = 1 << num_bits; // 最大值
    SchemeLWE fhe;

    int N = 1 << 14; // length of the secret key
    int n = 144;    // size of subset used to encrypt each bit
    int k = 63;     // number of bits added in the threshold function
    int d = 35;          // threshold limit
    FiLIP filip(N, n, k, d);

    HomomorphicFiLIP homFilip(filip);
    long int iv = 1234567;

    int size_msg = 10;

    // 加密输入特征
    std::vector<std::vector<int>> bits_m(plain_features.size());
    std::cout << "plain_features.size() = " << plain_features.size() << std::endl;
    std::vector<std::vector<Ctxt_LWE>> enc_bits(plain_features.size()); // enc_bits[i] == i-th 特征的加密位
    std::vector<std::vector<int>> enc_features(plain_features.size());

    for (int i = 0; i < plain_features.size(); i++) {
        bits_m[i] = int_to_bits(plain_features[i], num_bits);

        // encipher m with FiLIP
        std::vector<int> c_filip = filip.enc(iv, bits_m[i]);

        std::vector<Ctxt_LWE> c_lwe = homFilip.transform(iv, c_filip);

        enc_bits[i] = c_lwe;
    }

    // 输出enc_bits
    for (int i = 0; i < enc_bits.size(); i++) {
        for (int j = 0; j < enc_bits[i].size(); j++) {
            std::cout << enc_bits[i][j].b << " ";
        }
        std::cout << std::endl;
    }

    // 加密实现
    std::cout << "加密完成。" << std::endl;
}