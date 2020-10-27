#ifndef HelloWorld_WIDGET_H
#define HelloWorld_WIDGET_H

#define NO_MIN_MAX

#include <stdio.h>
#include <chrono>
#include "ui_widget.h"
#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QStyleFactory>
#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <windows.h>
#include <mutex>
#include <thread>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>

using namespace std::chrono;
using hr = high_resolution_clock;
namespace mp = boost::multiprecision;

class Widget : public QWidget, public Ui::Widget {
    Q_OBJECT

	public:
		// コンストラクタ
		// 引数のQWidgetのポインタを渡すとそのウィジェットを親とする
		explicit Widget(QWidget *parent = nullptr) noexcept;
		~Widget() noexcept {}
		void print(const QString str) noexcept;
		void flush() noexcept;
		void clearLog() noexcept;
		std::mutex mtx;
		protected:

	// オリジナルのsignalは自ら定義する必要がある
	signals:
		void changeLog();

	// slotsで宣言するとスロットとしても使える
	public slots:
		void display() noexcept;

	private:
		QString log;
		std::thread workingThread;

};

void work(Widget* ui) noexcept;

#endif // EXAMPLE_WIDGET_H_