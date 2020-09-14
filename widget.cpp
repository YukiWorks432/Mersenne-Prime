#include "widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ThProgress(new QTimer) {
    // setupUiは、UICが生成したクラスに実装されている関数
    // これを呼ぶことでウィジェット内の要素の確保や初期値の設定などをDesignerで設定した値通りの状態にするための処理が行われる
    setupUi(this);
	setStyle(QStyleFactory::create("WindowsXP"));
    addLOG("calculation start");
	connect(this, SIGNAL(changeLOG()), this, SLOT(updateLOG()));
	connect(ThProgress, SIGNAL(timeout()), this, SLOT(updateTh()));
	connect(Priority, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePriority(int)));
	connect(ReButton, SIGNAL(clicked()), this, SLOT(SetThreads()));
	
	std::ifstream tdata(Data_File_Name, std::ios::in | std::ios::binary);
	if (tdata) {
		string tnums;
		tdata >> tnums;
		ThreadNums = std::stoi(tnums);
	} else {
		ThreadNums = omp_get_max_threads();
	}

	PriorityClass = {
		IDLE_PRIORITY_CLASS, BELOW_NORMAL_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS, ABOVE_NORMAL_PRIORITY_CLASS, HIGH_PRIORITY_CLASS
	};
	Ths = {
		Th_1, Th_2, Th_3, Th_4, Th_5, Th_6, Th_7, Th_8,
		Th_9, Th_10, Th_11, Th_12, Th_13, Th_14, Th_15, Th_16
	};
	Th_string.resize(16);
	for (int i = 0; i < 16; ++i) {
		Th_string[i] = "\0";
	}

	ThreadNum->setText(QString::number(ThreadNums));
	ThreadNum->update();
	updateTh();
	updatePriority(PriorityClass[0]);
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

void Widget::F5Th(const string& s, const int id) noexcept {
	Th_string[id] = s;
}

void Widget::updateTh() noexcept {
	for (int i = 0; i < 16; ++i) {
		Ths[i]->setText(QString::fromStdString(std::to_string(i) + " :\n" + Th_string[i]));
	}
}

void Widget::updatePriority(int num) noexcept {
	HANDLE hP = GetCurrentProcess();
	SetPriorityClass(hP, PriorityClass[num]);
}

void Widget::SetThreads() noexcept {
	std::ofstream tdata(Data_File_Name, std::ios::out | std::ios::binary);
	if (tdata) {
		tdata << ThreadNum->toPlainText().toStdString() << std::endl;
	}
}