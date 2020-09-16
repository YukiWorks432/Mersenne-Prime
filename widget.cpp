#include "widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ThProgress(new QTimer) {
    // setupUiは、UICが生成したクラスに実装されている関数
    // これを呼ぶことでウィジェット内の要素の確保や初期値の設定などをDesignerで設定した値通りの状態にするための処理が行われる
    setupUi(this);
	setStyle(QStyleFactory::create("Fusion"));
    addLOG("calculation start");
	connect(this, SIGNAL(changeLOG()), this, SLOT(updateLOG()));
	connect(ThProgress, SIGNAL(timeout()), this, SLOT(updateTh()));
	connect(Priority, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePriority(int)));
	
	std::fstream data(Data_File_Name, std::ios::in | std::ios::binary);
	if (data) {
		MER_DATA_INFO d__;
		data.read(reinterpret_cast<char *>(&d__), sizeof(d__));
		mer_data_info_real.thread_nums = d__.thread_nums;
		mer_data_info_real.p_start = d__.p_start <= 5 ? 3 : d__.p_start;
		mer_data_info_real.index_start = d__.index_start <= 2 ? 1 : d__.index_start;
		data.close();
	} else {
		mer_data_info_real.thread_nums = omp_get_max_threads();
		mer_data_info_real.p_start = 3;
		mer_data_info_real.index_start = 1;
		p_data.push_back(2);
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
	p_data.resize(50);

	ThreadNum->setText(QString::number(mer_data_info_real.thread_nums));
	ThreadNum->update();
	updateTh();
	updatePriority(0);
	ThProgress->start(10);
    mainThread = std::thread(Lucas, this, mer_data_info_real.p_start, mer_data_info_real.index_start);
}
Widget::~Widget() noexcept {
    mainThread.join();
}

void Widget::closeEvent(QCloseEvent *e) noexcept {
	endFlag = true;
	p_data.sort();
	std::fstream data(Data_File_Name, std::ios::out | std::ios::binary);
	if (data) {
		MER_DATA_INFO d__;
		d__.p_start = last_p;
		d__.index_start = last_index;
		d__.thread_nums = std::stoi(ThreadNum->toPlainText().toStdString());
		data.write(reinterpret_cast<char *>(&d__), sizeof(d__));
		data.close();
	} else {
		qDebug() << QString::fromUtf8("データファイルが開けません");
	}

	std::fstream hist(History, std::ios::app);
	for (auto itr = p_data.begin(); itr != p_data.end(); itr++) {
		if (*itr == 0) continue;
		hist << *itr << std::endl;
	}
	std::exit(0);
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
	if (SetPriorityClass(hP, PriorityClass[num]) == 0) {
		auto Er = ::GetLastError();
		LPWSTR buf = nullptr;
		size_t size = FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, Er, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  (LPWSTR)&buf, 0, NULL
		);
		qDebug() << QString::fromStdWString(buf);
	}
}