#include "widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ThProgress(new QTimer) {
    // setupUiは、UICが生成したクラスに実装されている関数
    // これを呼ぶことでウィジェット内の要素の確保や初期値の設定などをDesignerで設定した値通りの状態にするための処理が行われる
    setupUi(this);
	setStyle(QStyleFactory::create("WindowsXP"));
    addLOG("calculation start");
	connect(this, SIGNAL(changeLOG()), this, SLOT(updateLOG()));
	connect(ThProgress, SIGNAL(timeout()), this, SLOT(updateTh()));
	
	updateTh();
	ThProgress->start(10);
    mainThread = std::thread(Lucas, this);
}
Widget::~Widget() noexcept {
    mainThread.join();
}

void Widget::closeEvent(QCloseEvent *e) noexcept {
	exit(0);
}

void Widget::addLOG(const string& src) noexcept {
	if (logs.empty()) {
		logs = src;
		return;
	}
    logs += "\n" + src;
	emit changeLOG();
}
void Widget::addLOG(const QString& qs) noexcept {
	string src = qs.toStdString();
	addLOG(src);
	emit changeLOG();
}
void Widget::addLOG(const char* const cs) noexcept {
    string src(cs);
    addLOG(src);
	emit changeLOG();
}
void Widget::updateLOG() noexcept {
	LOG->setText(QString::fromStdString(logs));
	QTextCursor c = LOG->textCursor();
	c.movePosition(QTextCursor::End);
	LOG->setTextCursor(c);
	LOG->update();
}

void Widget::F5Th(const string& s) noexcept {
	Th_string = s;
}

void Widget::updateTh() noexcept {
	Th->setText(QString::fromStdString(Th_string));
}
