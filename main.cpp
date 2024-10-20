#include <QApplication>
#include <QStyleFactory>
#include "testdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 设置应用程序风格为 Fusion
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    // 自定义调色板
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(0, 0, 0)); // 深灰色背景
    palette.setColor(QPalette::WindowText, QColor(220, 220, 220)); // 浅灰色文本
    palette.setColor(QPalette::Base, QColor(30, 30, 30)); // 深灰色输入框背景
    palette.setColor(QPalette::AlternateBase, QColor(50, 50, 50)); // 交替行背景
    palette.setColor(QPalette::ToolTipBase, QColor(255, 255, 220)); // 浅黄色工具提示背景
    palette.setColor(QPalette::ToolTipText, QColor(0, 0, 0)); // 黑色工具提示文本
    palette.setColor(QPalette::Text, QColor(220, 220, 220)); // 浅灰色文本
    palette.setColor(QPalette::Button, QColor(70, 70, 70)); // 深灰色按钮背景
    palette.setColor(QPalette::ButtonText, QColor(220, 220, 220)); // 浅灰色按钮文本
    palette.setColor(QPalette::BrightText, QColor(255, 0, 0)); // 红色高亮文本
    palette.setColor(QPalette::Link, QColor(0, 122, 204)); // 蓝色链接
    palette.setColor(QPalette::Highlight, QColor(0, 122, 204)); // 蓝色高亮
    palette.setColor(QPalette::HighlightedText, QColor(255, 255, 255)); // 白色高亮文本

    a.setPalette(palette);

    // 自定义样式表
    a.setStyleSheet(
        "QToolTip { color: #000000; background-color: #ffffdc; border: 1px solid white; }"

        "QPushButton {"
        "    background-color: #007acc;"
        "    border: none;"
        "    padding: 10px;"
        "    border-radius: 5px;"
        "    color: white;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #005f99;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #004080;"
        "}"
        "QLineEdit { padding: 5px; border: 1px solid #007acc; border-radius: 5px; }"
        "QTextEdit { padding: 5px; border: 1px solid #007acc; border-radius: 5px; }"
        "QListWidget { padding: 5px; border: 1px solid #007acc; border-radius: 5px; }"
    );

    TestDialog w;
    w.show();
    return a.exec();
}