#ifndef CONSOLETEXTEDIT_H
#define CONSOLETEXTEDIT_H

#include <QTextEdit>
#include <streambuf>
#include <ostream>
#include <sstream>

class ConsoleTextEdit : public QTextEdit {
    Q_OBJECT

public:
    explicit ConsoleTextEdit(QWidget *parent = nullptr);
    void setOutputStream(std::ostream &stream);
    std::string getBufferContent() const;
    void clearBuffer(); // 添加 clearBuffer 方法

private:
    class StreamBuffer : public std::streambuf {
    public:
        StreamBuffer(ConsoleTextEdit *textEdit);
        std::streambuf* getOldBuf() const { return oldBuf; }
        void setOldBuf(std::streambuf *buf) { oldBuf = buf; }
        std::string getBufferContent() const { return buffer.str(); }
        void clearBuffer() { buffer.str(""); buffer.clear(); } // 添加 clearBuffer 方法

    protected:
        int overflow(int c) override;
        std::streamsize xsputn(const char* s, std::streamsize n) override;

    private:
        ConsoleTextEdit *textEdit;
        std::streambuf *oldBuf;
        std::ostringstream buffer;
    };

    StreamBuffer buffer;
    std::ostream *oldStream;
};

#endif // CONSOLETEXTEDIT_H