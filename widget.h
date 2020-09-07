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
#include <chrono>
#include <wchar.h>
#include <queue>
#include <condition_variable>
#include <sstream>
#include <boost/multiprecision/gmp.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
#include <boost/format.hpp>

using std::cout; using std::wcout; using std::endl;
using std::array; using std::vector; using std::string; using std::wstring; using std::to_string;
using namespace std::literals::string_literals;
using hr = std::chrono::high_resolution_clock;
using namespace std::chrono;

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
		void F5Th(const string& s) noexcept;

	protected:

	// オリジナルのsignalは自ら定義する必要がある
	signals:
		void changeLOG();

	// slotsで宣言するとスロットとしても使える
	public slots:
		void updateLOG() noexcept;
		void updateTh() noexcept;

	private:
		QTimer* ThProgress;
		string logs;
        std::thread mainThread;
		std::array<QTextEdit*, 16> Ths;
		string Th_string;

};

void Lucas(Widget* ui) noexcept;

#endif // EXAMPLE_WIDGET_H_