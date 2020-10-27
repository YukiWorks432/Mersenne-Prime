#ifndef HelloWorld_WIDGET_H
#define HelloWorld_WIDGET_H

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

class Widget : public QWidget, public Ui::Widget {
    Q_OBJECT

	public:
		// コンストラクタ
		// 引数のQWidgetのポインタを渡すとそのウィジェットを親とする
		explicit Widget(QWidget *parent = nullptr);
		~Widget() noexcept;
	protected:

	// オリジナルのsignalは自ら定義する必要がある
	signals:

	// slotsで宣言するとスロットとしても使える
	public slots:

	private:

};

#endif // EXAMPLE_WIDGET_H_