#include <iostream>
#include <array>

extern "C" {
#include "include/decls.h"
#include "include/cephes.h"
#include "include/utilities.h"
#include "include/stat_fncs.h"
}


static std::string bin(uint64_t n) {
    static std::string s_;
    s_.resize(64);
    for (int i = 0; i < 64; ++i) {
        s_[63 - i] = '0' + (n & 1UL);
        n >>= 1;
    }
    return s_;
}

static inline uint64_t rdtsc(void) {
    uint32_t eax, edx;
    uint64_t tsc_val;
    /*
     * The lfence is to wait (on Intel CPUs) until all previous
     * instructions have been executed. If software requires RDTSC to be
     * executed prior to execution of any subsequent instruction, it can
     * execute LFENCE immediately after RDTSC
     */
    __asm__ __volatile__("lfence; rdtsc; lfence" : "=a"(eax), "=d"(edx));
    tsc_val = ((uint64_t) edx) << 32 | eax;
    return tsc_val;
}

template<int i> using serial = std::array<unsigned char, i>;

template<int l>
void test(serial<l> *a) {
    for (int i = 0; i <= NUMOFTESTS; ++i) {
        stats[i] = stdout;
        results[i] = stdout;
    }
    epsilon = a->data();
    std::cout << "=== 1 ===\n";
    Frequency(l);
    std::cout << "=== 2 ===\n";
    BlockFrequency(3, l);
    std::cout << "=== 3 ===\n";
    CumulativeSums(l);
    std::cout << "=== 4 ===\n";
    Runs(l);
    std::cout << "=== 5 ===\n";
    LongestRunOfOnes(l);
    std::cout << "=== 6 ===\n";
    Rank(l);
    std::cout << "=== 7 ===\n";
    DiscreteFourierTransform(l);
    std::cout << "=== 8 ===\n";
    NonOverlappingTemplateMatchings(8, l);
    std::cout << "=== 9 ===\n";
    OverlappingTemplateMatchings(8, l);
    std::cout << "=== 10 ===\n";
    Universal(l);
    std::cout << "=== 11 ===\n";
    ApproximateEntropy(5, l);
    std::cout << "=== 12 ===\n";
    RandomExcursions(l);
    std::cout << "=== 13 ===\n";
    RandomExcursionsVariant(l);
    std::cout << "=== 14 ===\n";
    LinearComplexity(20, l);
    std::cout << "=== 15 ===\n";
    Serial(8, l);
}

template<int l, typename F>
void set_(serial<l> *a_, uint64_t a, F f) {
    uint64_t n = 0;
    do {
        a = f(a);
        (*a_)[n++] = a & 1UL;
    } while (a && n < l);
}

template<int l>
void set(serial<l> *a_, uint8_t c, uint8_t d) {
    uint64_t a = 371;
    uint64_t n = 0;
    uint64_t c1 = (1UL << c) - 1UL;
    uint64_t c2 = (1UL << (c - 1UL)) - 1UL;
    uint8_t c3 = c - 1;
    uint64_t ts = rdtsc();
    do {
        a = ((a << 1) & c1) | ((1UL ^ a ^ (a >> c3) ^ (a >> d)) & 1UL);
        (*a_)[n++] = a & 1UL;
    } while (a && n < l);
    uint64_t te = rdtsc();
    printf("c=%2d %0.2f %12lu %2.3f\tc1=%lu c2=%lu c3=%d n=%lu(%lX) %c\n", c, (double) n / (double) (1UL << c), te - ts, (double) (te - ts) / (double) n, c1, c2, c3, n, n, n == 1UL << c ? '*' : ' ');
}

template<int l>
void print(serial<l> &a_) {
    for (int i = 0; i < 100; ++i) {
        std::cout << static_cast<char>(a_[i] + '.') << " ";
    }
    std::cout << "\n";
}

#define l 2000000

void example1() {
    std::cout << "NIST!" << std::endl;
    serial<l> a;
    set<l>(&a, 37, 8);//(37,8) (42,8)
    test<l>(&a);
}

void example2() {
    std::cout << "GENERATOR TEST 1!" << std::endl;
    serial<l> a;
    int shift_ = 4;
    auto r = [shift_](uint64_t a) -> uint64_t {
        return (a << 1) | ((1UL ^ a ^ (a >> shift_)) & 1UL);
    };
    set_<l>(&a, 0, r);
    print<l>(a);
}

//test start value
void example3() {
    std::cout << "GENERATOR TEST 2!" << std::endl;
    serial<l> a;
    int shift_ = 6;
    auto r = [shift_](uint64_t a) -> uint64_t {
        return (a << 1) | ((1UL ^ (a >> shift_)) & 1UL);
    };
    for (int i = 0; i < 1 << shift_; ++i) {
        set_<l>(&a, i, r);
        print<l>(a);
    }
}

int main() {
    example1();
    example2();
    example3();
    return 0;
}
