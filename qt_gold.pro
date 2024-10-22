QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += main.cpp \
           ConsoleTextEdit.cpp \
           gold/src/cpp_pdte_transciphering/filip.cpp \
           gold/src/cpp_pdte_transciphering/final/src/fft.cpp \
           gold/src/cpp_pdte_transciphering/final/src/keygen.cpp \
           gold/src/cpp_pdte_transciphering/final/src/lwehe.cpp \
           gold/src/cpp_pdte_transciphering/final/src/ntruhe.cpp \
           gold/src/cpp_pdte_transciphering/final/src/sampler.cpp \
           gold/src/cpp_pdte_transciphering/final/test.cpp \
           gold/src/cpp_pdte_transciphering/homomorphic_filip.cpp \
           gold/src/cpp_pdte_transciphering/node.cpp \
           gold/src/cpp_pdte_transciphering/test_filip.cpp \
           gold/src/cpp_pdte_transciphering/test_homomorphic_filip.cpp \
           gold/src/cpp_pdte_transciphering/test_node.cpp \
           gold/src/cpp_pdte_transciphering/test_pdte_transciphering.cpp \
           gold/src/cpp_pdte_transciphering/utils.cpp \
           testdialog.cpp \

HEADERS += \
           ConsoleTextEdit.h \
           gold/src/cpp_pdte_transciphering/filip.h \
           gold/src/cpp_pdte_transciphering/final/FINAL.h \
           gold/src/cpp_pdte_transciphering/final/include/fft.h \
           gold/src/cpp_pdte_transciphering/final/include/keygen.h \
           gold/src/cpp_pdte_transciphering/final/include/lwehe.h \
           gold/src/cpp_pdte_transciphering/final/include/ntruhe.h \
           gold/src/cpp_pdte_transciphering/final/include/params.h \
           gold/src/cpp_pdte_transciphering/final/include/sampler.h \
           gold/src/cpp_pdte_transciphering/homomorphic_filip.h \
           gold/src/cpp_pdte_transciphering/json.hpp \
           gold/src/cpp_pdte_transciphering/node.h \
           gold/src/cpp_pdte_transciphering/utils.h \
           gold/src/cpp_pdte_transciphering/vectorutils.hpp \
           testdialog.h \

INCLUDEPATH += $$PWD/gold/src/cpp_pdte_transciphering/final/include
INCLUDEPATH += $$PWD/gold/src/cpp_pdte_transciphering

LIBS += -L/path/to/ntl/lib -lntl
LIBS += -L/path/to/fftw/lib -lfftw3

FORMS += \
         testdialog.ui

DISTFILES += \
    gold/src/cpp_pdte_transciphering/Makefile \
    gold/src/cpp_pdte_transciphering/data/breast_11bits/model.json \
    gold/src/cpp_pdte_transciphering/data/electricity_10bits/model.json \
    gold/src/cpp_pdte_transciphering/data/phoneme_10bits/model.json \
    gold/src/cpp_pdte_transciphering/data/spam_11bits/model.json \
    gold/src/cpp_pdte_transciphering/filip.o \
    gold/src/cpp_pdte_transciphering/final/Makefile \
    gold/src/cpp_pdte_transciphering/final/fft.o \
    gold/src/cpp_pdte_transciphering/final/keygen.o \
    gold/src/cpp_pdte_transciphering/final/libfinal.a \
    gold/src/cpp_pdte_transciphering/final/lwehe.o \
    gold/src/cpp_pdte_transciphering/final/ntruhe.o \
    gold/src/cpp_pdte_transciphering/final/sampler.o \
    gold/src/cpp_pdte_transciphering/final/test \
    gold/src/cpp_pdte_transciphering/homomorphic_filip.o \
    gold/src/cpp_pdte_transciphering/node.o \
    gold/src/cpp_pdte_transciphering/test_filip \
    gold/src/cpp_pdte_transciphering/test_homomorphic_filip \
    gold/src/cpp_pdte_transciphering/test_node \
    gold/src/cpp_pdte_transciphering/test_pdte_transciphering \
    gold/src/cpp_pdte_transciphering/utils.o \
    home.png \
    icons/crypto.png \
    icons/download.png \
    icons/github.png \
    icons/home.png \
    icons/ml.png \
    icons/share.png \
    icons/start.png \
    icons/upload.png \
    styles/daymode.qss \
    styles/nightmode.qss

RESOURCES += \
    resources.qrc
