#define NOMINMAX

//#include "widget.h"
#include <thread>
#include <chrono>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <cstdlib>
#include <windows.h>
#include <sys/stat.h>

using namespace std::chrono;
using hr = high_resolution_clock;
namespace mp = boost::multiprecision;

namespace old {
    mp::uint1024_t doing = 0;
    const mp::uint1024_t Uint1024Max = (mp::uint1024_t(1) << 1024) - 1;

    inline int oldfunc() {
        if (setvbuf(stdout, NULL, _IOFBF, 1 * 1024 * 1024 * 1024)) {
            printf("failed setvbuf()\n");
            system("pause");
            return 0;
        }
        while (true) {
            printf("Hello World!");
            ++doing;
        }
        fflush(stdout);
        system("pause > nul");
        return 0;
    }

    inline int oldmain() {
        #pragma omp parallel sections
        {
            #pragma omp section
                oldfunc();
            #pragma omp section
            {
                const auto ep = system_clock::now();
                while (true) {
                    if (duration_cast<minutes>(system_clock::now() - ep) >= minutes(3)) {
                        FILE* fp;
                        while (true) {
                            if (fopen_s(&fp, "Result.txt", "w+") == 0) {
                                fprintf(fp, "%s", doing.str().c_str());
                                fclose(fp);
                                break;
                            }
                        }
                        exit(0);
                    }
                    std::this_thread::sleep_for(seconds(1));
                }
            }
        }
        return 0;
    }
}

namespace multi {
    const mp::uint1024_t Uint1024Max = mp::uint1024_t(0) - 1;
    unsigned long int doing = 0;

    void writeCount() {
        struct stat statBuf;
        while(true) {
            if (stat("Writing.txt", &statBuf) != 0) break;
            std::this_thread::sleep_for(milliseconds(3));
        }
        FILE* writing;
        while(true) {
            if (fopen_s(&writing, "Writing.txt", "w") == 0) {
                fclose(writing);
                break;
            }
        }
        const auto time_start = hr::now();
        FILE* fp_r;
        char* readBuf =  reinterpret_cast<char*>(malloc(sizeof(char) * 350));
        while (true) {
            if (fopen_s(&fp_r, "Result.txt", "r") == 0) {
                fgets(readBuf, 350, fp_r);
                fclose(fp_r);
                mp::uint1024_t allcount(readBuf);
                free(readBuf);
                if (Uint1024Max - allcount < doing) {
                    FILE* tmp;
                    fopen_s(&tmp, "Complete.txt", "w");
                    fclose(tmp);
                    _exit(0);
                }
                allcount += doing;
                doing = 0;
                FILE* fp_w;
                while (true) {
                    if (fopen_s(&fp_w, "Result.txt", "w") == 0) {
                        fprintf_s(fp_w, "%s", allcount.str().c_str());
                        fclose(fp_w);
                        break;
                    } else OutputDebugStringA("Failed fopen_s(\"Result.txt\", \"w\")");
                }
                break;
            } else OutputDebugStringA("Failed fopen_s(\"Result.txt\", \"r\")");
        }
        DeleteFileA("Writing.txt");
    }

    inline int multifunc() {
        std::thread writeFunc([]{return;});

        while (true) {
            printf("Hello World!");
            ++doing;
            if (doing == 1'000'000'000'000ul) {
                writeFunc.join();
                writeFunc = std::thread(writeCount);
            }
        }

        return 0;
    }

    inline int multimain() {
        if (setvbuf(stdout, NULL, _IOFBF, 1 * 1024 * 1024 * 1024)) {
            printf("failed setvbuf()\n");
            system("pause");
            return 0;
        }

        struct stat statBuf;
        if (stat("Result.txt", &statBuf) != 0) {
            FILE* fp;
            fopen_s(&fp, "Result.txt", "w");
            fprintf_s(fp, "0");
            fclose(fp);
        }

        #pragma omp parallel sections num_threads(2)
        {
            #pragma omp section
                multifunc();
            #pragma omp section
            {
                const auto ep = system_clock::now();
                while (true) {
                    if (duration_cast<minutes>(system_clock::now() - ep) >= minutes(1)) {
                        writeCount();
                        _exit(0);
                    }
                    std::this_thread::sleep_for(minutes(1));
                }
            }
        }

        return 0;
    }
}

/*
inline int Qtmain(int argc, char* argv[]) {
    QApplication app(argc, argv);
    Widget *widget = new Widget;
    widget->show();
    return app.exec();
}
*/

int main() {
    multi::multimain();
    return 0;
}