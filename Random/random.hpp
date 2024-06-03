#ifndef _RANDOM_H_
#define _RANDOM_H_

#include <random>

struct random_generator{
  private:
    static constexpr int BITLEN = 32;
    uint32_t x;
    int pos = 0;

  public:
    std::mt19937 mt;

    random_generator(const int seed = 0): mt(seed) { x = mt(); }
    
    // [0, 2^32)
    uint32_t operator ()() {
        return mt();
    }
    // [0, 2^b)
    uint32_t small(int b) {
        if (pos + b <= BITLEN) {
            pos += b;
        } else {
            pos = b;
            x = mt();
        }
        uint32_t res = x & ((1ULL << b) - 1);
        x >>= b;
        return res;
    }
} rng;
#endif