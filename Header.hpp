#ifndef PRIMENUMBER_HPP_
#define PRIMENUMBER_HPP_
#include "widget.h"

template <typename T>
string formatNumber(T num) noexcept {
    std::vector<T> sepnum;

    while (num / 1000) {
        sepnum.push_back(num % 1000);
        num /= 1000;
    }

    std::stringstream  ss;
    ss << num;
    for (auto i = sepnum.rbegin(); i < sepnum.rend(); ++i) {
        ss << "," << std::setfill('0') << std::setw(3) << *i;
    }
    return ss.str();
}

template <>
string formatNumber<Bint>(Bint num) noexcept {
    std::vector<unsigned int> sepnum;

    while (num / 1000) {
        sepnum.push_back(static_cast<unsigned int>(num % 1000));
        num /= 1000;
    }

    std::stringstream  ss;
    ss << num.str();
    for (auto i = sepnum.rbegin(); i < sepnum.rend(); ++i) {
        ss << "," << std::setfill('0') << std::setw(3) << *i;
    }
    return ss.str();
}

namespace my {
    inline int xxHash(const uint& data, const uint& seed) noexcept {
        const uint PRIME32_2 = 2246822519U;
        const uint PRIME32_3 = 3266489917U;
        const uint PRIME32_4 = 668265263U;
        const uint PRIME32_5 = 374761393U;
        uint h32 = data * PRIME32_3;
        h32 += seed + PRIME32_5 + 4U;
        h32 = (h32 << 17) | (h32 >> 15);
        h32 *= PRIME32_4;
        h32 ^= h32 >> 15;
        h32 *= PRIME32_2;
        h32 ^= h32 >> 13;
        h32 *= PRIME32_3;
        h32 ^= h32 >> 16;
        return static_cast<uint>(h32);
    }

    /*
    inline Bint pow(Bint x, Bint y) noexcept {
        Bint i = 1;
        while (y > 0) {
            if (y & 1) i *= x;
            x *= x;
            y >>= 1;
        }
        return i;
    }
    //*/

    inline Bint modPow(Bint x, Bint k, Bint m) {
        Bint res = 1;
        while(k){
            if(k & 1){
                res = res * x % m;
            }
            k >>= 1;
            x = x * x % m;
        }
        return res;
    }

    inline string puts(const Bint& mer, const Bint& p, const Bint& index) noexcept {
        const auto mes = mer.str();
        const auto ps = formatNumber(p);
        const auto is = formatNumber(index);
        string s = (boost::format("\n\n%s\t:\n  2 ^ %s - 1 = \n\n%s\n\n") % is.c_str() % ps.c_str() % mes.c_str()).str();
        return s;
    }

    inline bool lucasLehmerTest(const unsigned long& p_) {
        Bint m = (Bint(1) << p_) - 1;
        Bint s = 4;
        
        for (Bint i = 2; i < p_; ++i) {
            Bint s2 = s * s;
            s = (s2 & m) + (s2 >> p_);
            if (s >= m) s -= m;
            s -= 2;
        }
        if (s == 0) return true;
        return false;
    }
}

void Lucas(Widget* ui, const ull P_START, const ull INDEX_START) noexcept {
    constexpr ull P_END = 82'589'933 + 16;  //82'589'933 is 51st, according to Wikipedia.
    std::queue<Bint> Mers;
    std::queue<ull> p;
    constexpr ull MAX_MEMORY = 1024ull * 1024ull * 1024ull * 32ull;

    omp_set_nested(true);

    if (P_START == 3) {
        ui->addLOG(my::puts(3, 2, 1));
    }
    #pragma omp parallel sections num_threads(3)
    {
        #pragma omp section
        {
            omp_set_num_threads(ui->getThreadNums());
            #pragma omp parallel for schedule(dynamic)
            for (ull p_ = P_START; p_ < P_END; p_ += 2) {
                #pragma omp cancel for if(ui->endFlag)
                #pragma omp cancellation point for
                #pragma omp critical
                    ui->last_p[omp_get_thread_num()] = p_;
                if ((p_ + 1) % (p_ < 10'000 ? 10 : 2) == 0) {
                    #pragma omp critical
                    ui->F5Th(formatNumber(p_), omp_get_thread_num());
                }
                if (mp::miller_rabin_test(p_, 5));
                
                Bint m = (Bint(1) << p_) - 1;
                if (mp::miller_rabin_test(m, 20)) {
                    #pragma omp critical
                    {
                        Mers.push(m);
                        p.push(p_);
                    }
                }
            }
            #pragma omp critical
            {
                std::this_thread::sleep_for(2s);
                if (ui->endFlag) {
                    ui->addLOG(QString::fromUtf8("演算を終了しました"));
                } else {
                    ui->addLOG(QString::fromUtf8("演算が終了しました"));
                }
            }
            ui->ended = true;
        }
        #pragma omp section
        {
            ull index = INDEX_START;
            while (true) {
                bool emp;
                #pragma omp critical
                {
                    emp = Mers.empty() || p.empty();
                }
                if (ui->endFlag) {
                    if (!emp) {
                        #pragma omp critical
                        {
                            const ull p_ = p.front();
                            ui->p_data.push_back(p_);
                            ui->addLOG(my::puts(Mers.front(), p_, index));
                            Mers.pop();
                            p.pop();
                            ui->last_index = index;
                        }
                        continue;
                    }
                    #pragma omp critical
                    {
                        ui->addLOG("LOGの更新が終了しました");
                    }
                    break;
                }
                if (emp) {
                    std::this_thread::sleep_for(1s);
                    continue;
                }
                if (!emp) {
                    #pragma omp critical
                    {
                        const ull p_ = p.front();
                        ui->p_data.push_back(p_);
                        ui->addLOG(my::puts(Mers.front(), p_, index));
                        Mers.pop();
                        p.pop();
                        ui->last_index = index;
                    }
                    ++index;
                }
            }
        }
        #pragma omp section
        {
            while (true) {
                std::this_thread::sleep_for(1s);
                HANDLE hProc = GetCurrentProcess();
                PROCESS_MEMORY_COUNTERS_EX pmc;
                BOOL isSuccess = GetProcessMemoryInfo(hProc, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
                CloseHandle(hProc);
                if (isSuccess == FALSE) continue;
                if (pmc.WorkingSetSize > MAX_MEMORY) {
                    ui->endFlag = true;
                    qDebug() << QString::fromUtf8("使用メモリが32GBを超えました");
                    std::this_thread::sleep_for(10s);
                    break;
                }
            }
        }
    }
}

#endif