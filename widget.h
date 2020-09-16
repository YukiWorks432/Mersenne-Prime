#ifndef PRIME_WIDGET_H_
#define PRIME_WIDGET_H_

#define NO_MIN_MAX

#include "ui_widget.h"
#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QStyleFactory>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QGraphicsOpacityEffect>
#include <QtWidgets/QGraphicsView>
#include <QtCore/QtCore>
#include <QtCore/QtDebug>
#include <QtCore/QProcess>
#include <QtCore/QThread>
#include <QtCore/QTextCodec>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QAbstractAnimation>
#include <QtGui/QtGui>
#include <QtGui/QClipboard>
#include <QtGui/QFont>
#include <QtGui/QFontDatabase>
#include <QtGui/QPalette>
#include <QtGui/QColor>
#include <windows.h>
#include <stdio.h>
#include <algorithm>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <iostream>
#include <regex>
#include <string>
#include <omp.h>
#include <fstream>
#include <array>
#include <list>
#include <chrono>
#include <wchar.h>
#include <queue>
#include <condition_variable>
#include <sstream>
#include <psapi.h>
#include <boost/multiprecision/gmp.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
#include <boost/format.hpp>

using std::cout; using std::wcout; using std::endl; using std::flush;
using std::array; using std::vector; using std::string; using std::wstring; using std::to_string;
using namespace std::literals::string_literals;
using namespace std::chrono;
using hr = high_resolution_clock;
namespace mp = boost::multiprecision;
using Bint = mp::mpz_int;
using Real = mp::mpf_float;
using namespace std::chrono_literals;
using uint = unsigned int;
using ull = unsigned long long int;

struct MER_DATA_INFO {
	ull p_start;
	ull index_start;
	int thread_nums;
};

class Widget : public QWidget, public Ui::Widget {
    Q_OBJECT

	public:
		// コンストラクタ
		// 引数のQWidgetのポインタを渡すとそのウィジェットを親とする
		explicit Widget(QWidget *parent = nullptr);
		~Widget() noexcept;
		void closeEvent(QCloseEvent *e) noexcept;
        void addLOG(const string& src) noexcept;
        void addLOG(const QString& qs) noexcept;
        void addLOG(const char* const cs) noexcept;
		void F5Th(const string& s, const int id) noexcept;
		constexpr inline int getThreadNums() const noexcept
		{	return mer_data_info_real.thread_nums;		}

		std::atomic<bool> endFlag = false;
		std::atomic<bool> ended = false;
		std::list<ull> p_data;
		ull *last_p;
		ull last_index;

	protected:

	// オリジナルのsignalは自ら定義する必要がある
	signals:
		void changeLOG();

	// slotsで宣言するとスロットとしても使える
	public slots:
		void updateLOG() noexcept;
		void updateTh() noexcept;
		void updatePriority(int num) noexcept;

	private:
		QTimer* ThProgress;
		string logs;
        std::thread mainThread;
		std::vector<string> Th_string;
		std::vector<QTextEdit*> Ths;
		std::array<DWORD, 5> PriorityClass;
		MER_DATA_INFO mer_data_info_real;
		const char* const Data_File_Name = "MersennePrimeData.ympdb";
		const char* const History = "MersennePrimeHistory.txt";

};

void Lucas(Widget* ui, const ull P_START, const ull INDEX_START) noexcept;

#endif // EXAMPLE_WIDGET_H_