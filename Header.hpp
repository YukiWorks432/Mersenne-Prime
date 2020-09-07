#ifndef PRIMENUMBER_HPP_
#define PRIMENUMBER_HPP_
#include "widget.h"

namespace mp = boost::multiprecision;
using Bint = mp::mpz_int;
using Real = mp::mpf_float;
using std::cout; using std::endl; using std::flush; using std::string; using std::to_string;
using namespace std::chrono_literals;
using uint = unsigned int;

constexpr auto INDEX_MAX = 10;
constexpr auto P_START = 3; //43,112,609;

class XorShiftBint {
    private:
        struct dictionary { Bint x; Bint y; Bint z; Bint w; };
        Bint x;
        Bint y;
        Bint z;
        Bint w;
        Bint t;

    public:
        static const struct dictionary defaults;
        Bint randCount = 0;
        struct dictionary seeds;

        XorShiftBint(
            Bint w = time(nullptr),
            Bint x = 0,
            Bint y = 0,
            Bint z = 0
        ) {
            x = x != 0 ? x : w << 13;
            y = y != 0 ? y : (w >> 9) ^ (x << 6);
            z = z != 0 ? z : y >> 7;
            seeds = { x,y,z,w };
            this->w = w; this->x = x; this->y = y; this->z = z;
        }

        Bint rand() {
            randCount++;
            Bint t = x ^ (x << 11);
            x = y;
            y = z;
            z = w;
            return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
        }

        Bint randInt(Bint min = 0, Bint max = UINT64_MAX) {
            return rand() % (max - min + 1) + min;
        }

        Real randFloat(Real min = 0, Real max = 1) {
            return Real(rand() % 0xFFFF) / 0xFFFF * (max - min) + min;
        }

        template<typename T>
        T* shuffle(const T* _arr, T* arr, int arrLength) {
            for (int i = 0; i < arrLength; i++) arr[i] = _arr[i];
            for (int i = 0; i <= arrLength - 2; i++) {
                int r = randInt(i, arrLength - 1);
                T tmp = arr[i];
                arr[i] = arr[r];
                arr[r] = tmp;
            }
            return arr;
        }

        template<typename T>
        std::vector<T> shuffle(std::vector<T> arr) {
            for (int i = 0; i <= arr.size() - 2; i++) {
                int r = randInt(i, arr.size() - 1);
                T tmp = arr[i];
                arr[i] = arr[r];
                arr[r] = tmp;
            }
            return arr;
        }

        static inline XorShiftBint defaultSeed() {
            return XorShiftBint(
                XorShiftBint::defaults.w,
                XorShiftBint::defaults.x,
                XorShiftBint::defaults.y,
                XorShiftBint::defaults.z
            );
        }
};

const struct XorShiftBint::dictionary XorShiftBint::defaults = {
    123456789,
    362436069,
    521288629,
    88675123
};

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

    inline bool IsPrimeMillerRabin(Bint p, Bint k = 10) noexcept {
        if (p == 2) return true;                    //  2は素数
        if (p < 2 || !( p & 1 ))  return false;     //  2よりも小さいまたは（2以外の）偶数なら計算するまでもなし

        //  p - 1 = pow( 2, s ) * d, s > 0 において、最初の奇数dを見つける。
        Bint d = p - 1;
        while (!( d & 1 )) {
            d >>= 1;
        }

        XorShiftBint ram(time(nullptr) ^ p);
        for (int i = 0; i < 20; ++i) ram.rand();
        for (int n = 0; n != k; ++n) {
            Bint a = ram.randInt(0, p - 2) + 1;
            Bint t = d; 
            Bint y = my::modPow(a, t, p);
            while(t != p - 1 && y != 1 && y != p - 1) {
                y = y * y % p;
                t <<= 1;
            }
            if (y != p - 1 && !(t & 1)) {
                return false;
            }
        }
        return true;
    }

    inline bool IsPrimeFast(const Bint& num) noexcept {
        if (num == 2) return true;
        if ((num < 2) || !(num & 1)) return false;

        Bint sqrtNum = mp::sqrt(num);
        for (Bint i = 3; i <= sqrtNum; i += 2) {
            if (num % i == 0)  return false;
        }

        return true;
    }

    inline bool IsPrime(const Bint& num) noexcept {
        if (num == 2) return true;
        if ((num < 2) || !(num & 1)) return false;

        Bint sqrtNum = mp::sqrt(num);
        for (Bint i = 3; i <= sqrtNum; i += 2) {
            if (num % i == 0)  return false;
        }
        return true;
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

void Lucas(Widget* ui) noexcept {
    std::queue<Bint> Mers;
    std::queue<unsigned long> p;

    omp_set_nested(true);

    Mers.push(3);
    p.push(2);
    #pragma omp parallel sections firstprivate(INDEX_MAX, P_START) num_threads(2)
    {
        #pragma omp section
        {
            for (unsigned long p_ = P_START; p_ < ULONG_MAX; p_ += 2) {
                if (!mp::miller_rabin_test(p_, 5)) continue;
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