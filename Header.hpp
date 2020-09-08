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

constexpr unsigned long long P_START = 3;//43'112'609;

void Lucas(Widget* ui) noexcept {
    std::queue<Bint> Mers;
    std::queue<unsigned long> p;

    omp_set_nested(true);

    Mers.push(3);
    p.push(2);
    #pragma omp parallel sections num_threads(2)
    {
        #pragma omp section
        {
            for (unsigned long long p_ = P_START; p_ < ULONG_MAX; p_ += 2) {
                if (!mp::miller_rabin_test(p_, 3)) continue;
                if ((p_ + 1) % 10 == 0) ui->F5Th(formatNumber(p_));
                
                Bint m = (Bint(1) << p_) - 1;
                if (mp::miller_rabin_test(m, 20)) {
                    #pragma omp critical
                    {
                        Mers.push(m);
                        p.push(p_);
                    }
                }
            }
            
        }
        #pragma omp section
        {
            Bint p_;
            Bint mer_;
            uint_fast32_t index = 1;
            while (true) {
                bool emp;
                #pragma omp critical
                {
                    emp = Mers.empty() || p.empty();
                }
                if (emp) {
                    std::this_thread::sleep_for(1s);
                    continue;
                }
                #pragma omp critical
                {
                    mer_ = Mers.front();
                    p_ = p.front();
                    Mers.pop();
                    p.pop();
                    string s = my::puts(mer_, p_, index);
                    ui->addLOG(s);
                }
                ++index;
            }
        }
    }
}

#endif