#include "widget.h"

Widget::Widget(QWidget *parent) noexcept : QWidget(parent) {
    setupUi(this);
	setStyle(QStyleFactory::create("windowsvista"));
    connect(this, SIGNAL(changeLog()), this, SLOT(display()));

    workingThread = std::thread(work, this);
}

void work(Widget* ui) noexcept {
    const auto t = hr::now();
    const QString Hello = QString("Hello World!");
    for (int i = 0; i < 5'000'000; ++i) {
        ui->print(Hello);
    }
    const auto d = QString::number(duration_cast<duration<float>>(hr::now() - t).count());
    ui->print(QString("\n") + d + QString("sec."));
    std::lock_guard lock(ui->mtx);
    ui->flush();
}

void Widget::print(const QString str) noexcept {
    std::lock_guard lock(mtx);
    log += str;
    if ((512 * 1024 * 1024) - static_cast<int>(log.length()) <= 48) emit changeLog();
}

void Widget::display() noexcept {
    const auto t = hr::now();
    console->appendPlainText(log);
    QTextCursor c = console->textCursor();
    c.movePosition(QTextCursor::End);
    console->setTextCursor(c);
    console->update();
    log.clear();
    const auto d = QString::number(duration_cast<duration<float>>(hr::now() - t).count());
    qDebug() << d;
}

void Widget::flush() noexcept {
    emit changeLog();
}

void Widget::clearLog() noexcept {
    log.clear();
}