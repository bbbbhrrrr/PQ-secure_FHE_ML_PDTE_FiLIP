#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QStackedWidget>
#include <QApplication>
#include <QStyleFactory>
#include <QLabel>
#include <QIcon>
#include <QClipboard>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QComboBox>
#include <vector>
#include <string>
#include "ConsoleTextEdit.h"

class TestDialogUI {
public:
    QLineEdit *inputCsvPathLineEdit; // 添加输入 CSV 文件路径的 QLineEdit
    QLineEdit *outputCsvPathLineEdit; // 添加输出 CSV 文件路径的 QLineEdit
    QComboBox *modelComboBox; // 添加模型选择的 QComboBox
    QPushButton *runTestButton;
    QLineEdit *encryptInputLineEdit;
    QPushButton *encryptButton;
    ConsoleTextEdit *encryptOutputTextEdit; // 修改为 ConsoleTextEdit
    QListWidget *functionList;
    QStackedWidget *stackedWidget;
    QLabel *imageLabel;

    void setupUi(QDialog *dialog) {
        // 设置窗口标题
        dialog->setWindowTitle("基于后量子安全的机器学习算法");

        // 设置应用程序风格为 Fusion
        QApplication::setStyle(QStyleFactory::create("Fusion"));

        inputCsvPathLineEdit = new QLineEdit(dialog); // 初始化输入 CSV 文件路径的 QLineEdit
        outputCsvPathLineEdit = new QLineEdit(dialog); // 初始化输出 CSV 文件路径的 QLineEdit
        modelComboBox = new QComboBox(dialog); // 初始化模型选择的 QComboBox
        runTestButton = new QPushButton("运行", dialog);
        runTestButton->setIcon(QIcon("/home/bbbbhrrrr/qt_sh/icons/start.png")); // 为按钮添加图标

        encryptInputLineEdit = new QLineEdit(dialog);
        encryptButton = new QPushButton("加密", dialog);
        encryptButton->setIcon(QIcon("/home/bbbbhrrrr/qt_sh/icons/start.png")); // 为按钮添加图标
        encryptOutputTextEdit = new ConsoleTextEdit(dialog); // 修改为 ConsoleTextEdit

        functionList = new QListWidget(dialog);
        stackedWidget = new QStackedWidget(dialog);

        // 添加功能项到功能列表
        QListWidgetItem *homeItem = new QListWidgetItem("主页");
        homeItem->setIcon(QIcon("/home/bbbbhrrrr/qt_sh/icons/home.png"));
        QListWidgetItem *mlItem = new QListWidgetItem("FHEML--PDTE");
        mlItem->setIcon(QIcon("/home/bbbbhrrrr/qt_sh/icons/ml.png"));
        QListWidgetItem *encryptItem = new QListWidgetItem("FiLIP加密");
        encryptItem->setIcon(QIcon("/home/bbbbhrrrr/qt_sh/icons/crypto.png"));
        functionList->addItem(homeItem);
        functionList->addItem(mlItem);
        functionList->addItem(encryptItem);

        // 创建主页页面
        QWidget *homePage = new QWidget();
        QVBoxLayout *homeLayout = new QVBoxLayout(homePage);

        // 添加图片的 QLabel
        imageLabel = new QLabel(homePage);
        imageLabel->setPixmap(QPixmap("/home/bbbbhrrrr/home.png")); // 设置图片路径
        imageLabel->setAlignment(Qt::AlignCenter); // 设置图片居中对齐
        homeLayout->addWidget(imageLabel);

        // 添加文本的 QLabel
        QLabel *homeLabel = new QLabel("欢迎使用后量子安全的机器学习算法！", homePage);

        // 设置 homeLabel 的字体大小和样式
        QFont font = homeLabel->font();
        font.setPointSize(24); // 设置字体大小为 24
        font.setBold(true); // 设置字体加粗
        homeLabel->setFont(font);

        homeLabel->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
        homeLayout->addWidget(homeLabel);

        // 添加两个按钮模块
        QHBoxLayout *buttonLayout = new QHBoxLayout();

        QPushButton *learnMoreButton = new QPushButton("了解更多", homePage);
        learnMoreButton->setIcon(QIcon("/home/bbbbhrrrr/qt_sh/icons/github.png")); // 设置图标路径
        buttonLayout->addWidget(learnMoreButton);

        QPushButton *shareButton = new QPushButton("分享", homePage);
        shareButton->setIcon(QIcon("/home/bbbbhrrrr/qt_sh/icons/share.png")); // 设置图标路径
        buttonLayout->addWidget(shareButton);

        homeLayout->addLayout(buttonLayout);

        homePage->setLayout(homeLayout);

        // 创建机器学习页面
        QWidget *mlPage = new QWidget();
        QVBoxLayout *mlLayout = new QVBoxLayout(mlPage);

        QHBoxLayout *inputPathLayout = new QHBoxLayout(); // 创建一个水平布局
        QLabel *inputPathIcon = new QLabel(mlPage); // 创建一个 QLabel 用于显示图标
        QLabel *inputPathLabel = new QLabel("输入测试数据文件路径", mlPage); // 创建文本标签

        // 获取文本标签的字体高度
        int fontHeight = inputPathLabel->fontMetrics().height();

        // 设置图标路径并调整大小
        QPixmap iconPixmap("/home/bbbbhrrrr/qt_sh/icons/upload.png");
        inputPathIcon->setPixmap(iconPixmap.scaled(fontHeight, fontHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        // 将图标和文本标签添加到水平布局
        inputPathLayout->addWidget(inputPathIcon);
        inputPathLayout->addWidget(inputPathLabel);

        // 设置水平布局的对齐方式为左对齐
        inputPathLayout->setAlignment(Qt::AlignLeft);

        mlLayout->addLayout(inputPathLayout); // 将水平布局添加到主布局

        mlLayout->addWidget(inputCsvPathLineEdit); // 添加输入 CSV 文件路径的 QLineEdit

        QHBoxLayout *outputPathLayout = new QHBoxLayout(); // 创建一个水平布局
        QLabel *outputPathIcon = new QLabel(mlPage); // 创建一个 QLabel 用于显示图标
        QLabel *outputPathLabel = new QLabel("输入测试结果文件保存路径", mlPage); // 创建文本标签

        // 获取文本标签的字体高度
        int fontHeight_ = outputPathLabel->fontMetrics().height();

        // 设置图标路径并调整大小
        QPixmap oconPixmap("/home/bbbbhrrrr/qt_sh/icons/download.png");
        outputPathIcon->setPixmap(oconPixmap.scaled(fontHeight_, fontHeight_, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        // 将图标和文本标签添加到水平布局
        outputPathLayout->addWidget(outputPathIcon);
        outputPathLayout->addWidget(outputPathLabel);

        // 设置水平布局的对齐方式为左对齐
        outputPathLayout->setAlignment(Qt::AlignLeft);

        mlLayout->addLayout(outputPathLayout); // 将水平布局添加到主布局

        mlLayout->addWidget(outputCsvPathLineEdit); // 添加输出 CSV 文件路径的 QLineEdit

        // 添加模型选择的 QComboBox
        mlLayout->addWidget(new QLabel("选择模型", mlPage));
        mlLayout->addWidget(modelComboBox);

        // 添加模型选项
        modelComboBox->addItem("breast");
        modelComboBox->addItem("electricity");
        modelComboBox->addItem("phoneme");
        modelComboBox->addItem("spam");

        mlLayout->addWidget(runTestButton);

        // 创建加密页面
        QWidget *encryptPage = new QWidget();
        QVBoxLayout *encryptLayout = new QVBoxLayout(encryptPage);
        encryptLayout->addWidget(new QLabel("输入特征向量", encryptPage));
        encryptLayout->addWidget(encryptInputLineEdit);
        encryptLayout->addWidget(encryptButton);
        encryptLayout->addWidget(new QLabel("加密结果", encryptPage));
        encryptLayout->addWidget(encryptOutputTextEdit); // 修改为 ConsoleTextEdit
        encryptPage->setLayout(encryptLayout);

        // 将页面添加到 stackedWidget
        stackedWidget->addWidget(homePage);
        stackedWidget->addWidget(mlPage);
        stackedWidget->addWidget(encryptPage);

        // 设置布局
        QHBoxLayout *mainLayout = new QHBoxLayout(dialog);
        mainLayout->addWidget(functionList);
        mainLayout->addWidget(stackedWidget);
        dialog->setLayout(mainLayout);

        // 连接功能列表的信号与 stackedWidget 的切换槽
        QObject::connect(functionList, &QListWidget::currentRowChanged, stackedWidget, &QStackedWidget::setCurrentIndex);

        // 连接按钮的点击信号与槽函数
        QObject::connect(learnMoreButton, &QPushButton::clicked, []() {
            QDesktopServices::openUrl(QUrl("https://github.com/bbbbhrrrr?tab=repositories"));
        });

        QObject::connect(shareButton, &QPushButton::clicked, [dialog]() {
            QClipboard *clipboard = QApplication::clipboard();
            clipboard->setText("https://github.com/bbbbhrrrr?tab=repositories");
            QMessageBox::information(dialog, "分享", "链接已复制到粘贴板！");
        });
    }
};

class TestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TestDialog(QWidget *parent = nullptr);
    ~TestDialog();

private slots:
    void on_runTestButton_clicked();
    void on_encryptButton_clicked();

private:
    TestDialogUI *ui;
    std::string model_file;
    int num_bits;
    void runTests(const std::string& input_csv_path, const std::string& output_csv_path, const std::string& model_file_path, int model_num_bits);
    void runModel1(const std::string& input_csv_path, const std::string& output_csv_path);
    void runModel2(const std::string& input_csv_path, const std::string& output_csv_path);
    void runModel3(const std::string& input_csv_path, const std::string& output_csv_path);
    void runModel4(const std::string& input_csv_path, const std::string& output_csv_path);
    void filip_enc(const std::vector<unsigned int>& plain_features, int num_bits);
};

#endif // TESTDIALOG_H