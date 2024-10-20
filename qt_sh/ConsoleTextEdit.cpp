#include "ConsoleTextEdit.h"

ConsoleTextEdit::ConsoleTextEdit(QWidget *parent)
    : QTextEdit(parent), buffer(this), oldStream(nullptr) {
    setReadOnly(true);
}

void ConsoleTextEdit::setOutputStream(std::ostream &stream) {
    if (oldStream) {
        oldStream->rdbuf(buffer.getOldBuf());
    }
    oldStream = &stream;
    buffer.setOldBuf(stream.rdbuf(&buffer));
}

std::string ConsoleTextEdit::getBufferContent() const {
    return buffer.getBufferContent();
}

void ConsoleTextEdit::clearBuffer() {
    buffer.clearBuffer();
    this->clear(); // 清除窗口中的内容
}

ConsoleTextEdit::StreamBuffer::StreamBuffer(ConsoleTextEdit *textEdit)
    : textEdit(textEdit), oldBuf(nullptr) {}

int ConsoleTextEdit::StreamBuffer::overflow(int c) {
    if (c != EOF) {
        buffer.put(c);
    }
    return c;
}

std::streamsize ConsoleTextEdit::StreamBuffer::xsputn(const char* s, std::streamsize n) {
    buffer.write(s, n);
    return n;
}