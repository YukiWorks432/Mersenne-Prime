#include  "Header.hpp"

unsigned int fontnum = 0;

int CALLBACK EnumFontFamExProc(ENUMLOGFONTEXW *lpelfe, NEWTEXTMETRICEX *lpntme, int FontType , LPARAM lParam) {
	auto p = reinterpret_cast<wstring *>(lParam);
    if (fontnum >= 1024) return 1;
	p[fontnum] = wstring(lpelfe->elfFullName);
	++fontnum;
	return 1;
}

class Fonts {
	public:
		std::array<wstring, 1024> FontNames;
		bool searchFont(const wstring &name) {
            for (unsigned int i = 0; i < fontnum; ++i) {
                const wstring font = FontNames[i];
				if(font.find(name) != wstring::npos) {  
					return true;
				}
			}
			return false;
		}
		Fonts() {
            FontNames.fill(L"NaN\0");
			LOGFONTW lf;
			HDC hdc = GetDC(0);
			lf.lfFaceName[0] = 0;
			lf.lfPitchAndFamily = DEFAULT_PITCH;
			lf.lfCharSet = SHIFTJIS_CHARSET; 
			EnumFontFamiliesExW(hdc, &lf, (FONTENUMPROCW)EnumFontFamExProc, reinterpret_cast<LPARAM>(this->FontNames.data()), 0);
			ReleaseDC(0, hdc);
		}
};

void error_stop() {
    MessageBox(NULL, "Error!!!", "Error!!!", MB_OK);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    std::set_terminate(error_stop);

    try {
        // mainの最初でQApplicationを作っておく
        QApplication app(argc, argv);
        
        Fonts font;
        QFont uifont;
        if (font.searchFont(wstring(L"MigMix 1P Bold"))) {
            uifont = QFont("MigMix 1P Bold");
        } else {
            uifont = QFont("Meiryo UI Bold");
        }
        app.setFont(uifont);

        auto palette = QPalette();
        palette.setColor(QPalette::ColorRole::Window, QColor(53, 53, 53));
        palette.setColor(QPalette::ColorRole::WindowText, Qt::white);
        palette.setColor(QPalette::ColorRole::Base, QColor(25, 25, 25));
        palette.setColor(QPalette::ColorRole::AlternateBase, QColor(53, 53, 53));
        palette.setColor(QPalette::ColorRole::ToolTipBase, Qt::black);
        palette.setColor(QPalette::ColorRole::ToolTipText, Qt::white);
        palette.setColor(QPalette::ColorRole::Text, Qt::white);
        palette.setColor(QPalette::ColorRole::Button, QColor(53, 53, 53));
        palette.setColor(QPalette::ColorRole::ButtonText, Qt::white);
        palette.setColor(QPalette::ColorRole::BrightText, Qt::red);
        palette.setColor(QPalette::ColorRole::Link, QColor(42, 130, 218));
        palette.setColor(QPalette::ColorRole::Highlight, QColor(42, 130, 218));
        palette.setColor(QPalette::ColorRole::HighlightedText, Qt::black);
        app.setPalette(palette);

        // 自作のWidgetクラスを生成、表示
        Widget *widget = new Widget;
        widget->show();

        return app.exec();
    } catch (const std::runtime_error &ex) {
        qDebug() << "std::runtime_error!!!\n" << ex.what();
        system("pause > nul");
        exit(1);
    } catch (const std::out_of_range &ex) {
        qDebug() << "std::out_of_range!!!\n" << ex.what();
        system("pause > nul");
        exit(1);
    } catch (const std::logic_error &ex) {
        qDebug() << "std::logic_error!!!\n" << ex.what();
        system("pause > nul");
        exit(1);
    } catch (const std::exception &ex) {
        qDebug() << "std::exception!!!\n" << ex.what();
        system("pause > nul");
        exit(1);
    } catch (...) {
        error_stop();
    }
}