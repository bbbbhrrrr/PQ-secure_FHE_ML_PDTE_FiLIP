QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += main.cpp \
           ConsoleTextEdit.cpp \
           testdialog.cpp \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/filip.cpp \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/final/src/fft.cpp \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/final/src/keygen.cpp \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/final/src/lwehe.cpp \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/final/src/ntruhe.cpp \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/final/src/sampler.cpp \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/final/test.cpp \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/homomorphic_filip.cpp \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/node.cpp \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/test_filip.cpp \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/test_homomorphic_filip.cpp \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/test_node.cpp \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/test_pdte_transciphering.cpp \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/utils.cpp

HEADERS += \
           ConsoleTextEdit.h \
           testdialog.h \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/filip.h \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/final/FINAL.h \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/final/include/fft.h \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/final/include/keygen.h \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/final/include/lwehe.h \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/final/include/ntruhe.h \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/final/include/params.h \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/final/include/sampler.h \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/homomorphic_filip.h \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/json.hpp \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/node.h \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/utils.h \
           ../sortinghat/SortingHat/src/cpp_pdte_transciphering/vectorutils.hpp

INCLUDEPATH += ../sortinghat/SortingHat/src/cpp_pdte_transciphering/final/include
INCLUDEPATH += ../sortinghat/SortingHat/src/cpp_pdte_transciphering

LIBS += -L/path/to/ntl/lib -lntl
LIBS += -L/path/to/fftw/lib -lfftw3

FORMS += \
         testdialog.ui

DISTFILES += \
    ../Downloads/8666686_github_icon.png \
    ../Downloads/8666691_home_icon.png \
    ../Downloads/8666696_share_2_icon.png \
    ../Downloads/8666697_shield_icon.png \
    ../Downloads/8666719_share_icon.png \
    ../Downloads/8666778_download_down_save_icon.png \
    ../Downloads/crypto.png \
    ../Downloads/download.png \
    ../Downloads/github.png \
    ../Downloads/home.png \
    ../Downloads/ml.png \
    ../Downloads/share.png \
    ../Downloads/upload.png \
    ../home.png \
    ../sortinghat/SortingHat/src/cpp_pdte_transciphering/Makefile \
    ../sortinghat/SortingHat/src/cpp_pdte_transciphering/data/electricity_10bits/model.json \
    ../sortinghat/SortingHat/src/cpp_pdte_transciphering/final/Makefile \
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
