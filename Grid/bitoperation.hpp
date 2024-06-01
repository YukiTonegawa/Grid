#ifndef _BITOPERATION_H_
#define _BITOPERATION_H_
#include <array>

namespace bit_numbers {
    static constexpr uint64_t mask_0_32  = 0x00000000FFFFFFFF;
    static constexpr uint64_t mask_48_64 = 0xFFFF000000000000;
    static constexpr uint64_t mask_32_48 = 0x0000FFFF00000000;
    static constexpr uint64_t mask_16_32 = 0x00000000FFFF0000;
    static constexpr uint64_t mask_0_16  = 0x000000000000FFFF;
};

// 64bit整数xのk番目の1の場所 存在しない場合-1
int select_64bit(uint64_t x, int k) {
    static constexpr size_t sz = 16;
    static bool built = false;
    static std::array<std::array<int8_t, sz>, 1 << sz> table;
    if (!built) {
        built = true;
        for (int i = 0; i < (1 << sz); i++) {
            table[i].fill(-1);
            int pop = 0;
            for (int j = 0; j < sz; j++) {
                if ((i >> j) & 1) {
                    table[i][pop++] = j;
                }
            }
        }
    }
    int left_pop = __builtin_popcount(x & bit_numbers::mask_0_32);
    if (left_pop > k) {
        int left_left_pop = __builtin_popcount(x & bit_numbers::mask_0_16);
        if (left_left_pop > k) {
            return table[x & bit_numbers::mask_0_16][k];
        } else {
            return sz + table[(x & bit_numbers::mask_16_32) >> sz][k - left_left_pop];
        }
    } else {
        k -= left_pop;
        int right_left_pop = __builtin_popcountll(x & bit_numbers::mask_32_48);
        if (right_left_pop > k) {
            return 2 * sz + table[(x & bit_numbers::mask_32_48) >> (2 * sz)][k];
        } else {
            return 3 * sz + table[(x & bit_numbers::mask_48_64) >> (3 * sz)][k - right_left_pop];
        }
    }
}

#endif