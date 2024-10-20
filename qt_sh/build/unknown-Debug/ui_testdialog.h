/********************************************************************************
** Form generated from reading UI file 'testdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TESTDIALOG_H
#define UI_TESTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QDialogButtonBox *buttonBox;
    QListWidget *functionList;
    QStackedWidget *stackedWidget;
    QWidget *homePage;
    QVBoxLayout *verticalLayoutHome;
    QLabel *homeLabel;
    QLabel *imageLabel;
    QWidget *mlPage;
    QVBoxLayout *verticalLayoutML;
    QTextEdit *outputTextEdit;
    QLineEdit *inputLineEdit;
    QPushButton *runTestButton;
    QWidget *encryptPage;
    QVBoxLayout *verticalLayoutEncrypt;
    QLineEdit *encryptInputLineEdit;
    QPushButton *encryptButton;
    QTextEdit *encryptOutputTextEdit;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(674, 300);
        buttonBox = new QDialogButtonBox(Dialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        functionList = new QListWidget(Dialog);
        QIcon icon(QIcon::fromTheme(QString::fromUtf8("face-angry")));
        QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(functionList);
        __qlistwidgetitem->setIcon(icon);
        new QListWidgetItem(functionList);
        new QListWidgetItem(functionList);
        functionList->setObjectName(QString::fromUtf8("functionList"));
        functionList->setGeometry(QRect(10, 10, 60, 280));
        stackedWidget = new QStackedWidget(Dialog);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setGeometry(QRect(140, 10, 411, 280));
        homePage = new QWidget();
        homePage->setObjectName(QString::fromUtf8("homePage"));
        verticalLayoutHome = new QVBoxLayout(homePage);
        verticalLayoutHome->setObjectName(QString::fromUtf8("verticalLayoutHome"));
        homeLabel = new QLabel(homePage);
        homeLabel->setObjectName(QString::fromUtf8("homeLabel"));
        QFont font;
        font.setPointSize(24);
        font.setBold(true);
        font.setItalic(true);
        homeLabel->setFont(font);

        verticalLayoutHome->addWidget(homeLabel);

        imageLabel = new QLabel(homePage);
        imageLabel->setObjectName(QString::fromUtf8("imageLabel"));
        imageLabel->setPixmap(QPixmap(QString::fromUtf8(":/home/bbbbhrrrr/home.png")));
        imageLabel->setAlignment(Qt::AlignCenter);

        verticalLayoutHome->addWidget(imageLabel);

        stackedWidget->addWidget(homePage);
        mlPage = new QWidget();
        mlPage->setObjectName(QString::fromUtf8("mlPage"));
        verticalLayoutML = new QVBoxLayout(mlPage);
        verticalLayoutML->setObjectName(QString::fromUtf8("verticalLayoutML"));
        outputTextEdit = new QTextEdit(mlPage);
        outputTextEdit->setObjectName(QString::fromUtf8("outputTextEdit"));
        QFont font1;
        font1.setPointSize(12);
        outputTextEdit->setFont(font1);

        verticalLayoutML->addWidget(outputTextEdit);

        inputLineEdit = new QLineEdit(mlPage);
        inputLineEdit->setObjectName(QString::fromUtf8("inputLineEdit"));
        inputLineEdit->setFont(font1);

        verticalLayoutML->addWidget(inputLineEdit);

        runTestButton = new QPushButton(mlPage);
        runTestButton->setObjectName(QString::fromUtf8("runTestButton"));
        QFont font2;
        font2.setPointSize(12);
        font2.setBold(true);
        runTestButton->setFont(font2);

        verticalLayoutML->addWidget(runTestButton);

        stackedWidget->addWidget(mlPage);
        encryptPage = new QWidget();
        encryptPage->setObjectName(QString::fromUtf8("encryptPage"));
        verticalLayoutEncrypt = new QVBoxLayout(encryptPage);
        verticalLayoutEncrypt->setObjectName(QString::fromUtf8("verticalLayoutEncrypt"));
        encryptInputLineEdit = new QLineEdit(encryptPage);
        encryptInputLineEdit->setObjectName(QString::fromUtf8("encryptInputLineEdit"));
        encryptInputLineEdit->setFont(font1);

        verticalLayoutEncrypt->addWidget(encryptInputLineEdit);

        encryptButton = new QPushButton(encryptPage);
        encryptButton->setObjectName(QString::fromUtf8("encryptButton"));
        encryptButton->setFont(font2);

        verticalLayoutEncrypt->addWidget(encryptButton);

        encryptOutputTextEdit = new QTextEdit(encryptPage);
        encryptOutputTextEdit->setObjectName(QString::fromUtf8("encryptOutputTextEdit"));
        encryptOutputTextEdit->setFont(font1);

        verticalLayoutEncrypt->addWidget(encryptOutputTextEdit);

        stackedWidget->addWidget(encryptPage);

        retranslateUi(Dialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Dialog, SLOT(reject()));
        QObject::connect(functionList, SIGNAL(currentRowChanged(int)), stackedWidget, SLOT(setCurrentIndex(int)));

        stackedWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));

        const bool __sortingEnabled = functionList->isSortingEnabled();
        functionList->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = functionList->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("Dialog", "\344\270\273\351\241\265", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = functionList->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("Dialog", "\346\234\272\345\231\250\345\255\246\344\271\240", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = functionList->item(2);
        ___qlistwidgetitem2->setText(QCoreApplication::translate("Dialog", "\345\212\240\345\257\206", nullptr));
        functionList->setSortingEnabled(__sortingEnabled);

        homeLabel->setText(QCoreApplication::translate("Dialog", "\346\254\242\350\277\216\344\275\277\347\224\250\351\207\217\345\255\220\345\256\211\345\205\250\347\232\204\346\234\272\345\231\250\345\255\246\344\271\240\347\256\227\346\263\225\357\274\201", nullptr));
        runTestButton->setText(QCoreApplication::translate("Dialog", "\350\277\220\350\241\214", nullptr));
        encryptButton->setText(QCoreApplication::translate("Dialog", "\345\212\240\345\257\206", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTDIALOG_H
