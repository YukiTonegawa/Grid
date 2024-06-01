#ifndef _BITSET_H_
#define _BITSET_H_

#include <vector>
#include <cassert>
#include "bitoperation.hpp"

// operator [] でビットごとの参照を得るためのプロクシクラス
// https://qiita.com/voidhoge/items/383244bad2d728a18dbe
struct Bitref {
  private:
    using ull = uint64_t;
	uint64_t *ptr;
	size_t pos;

  public:
	Bitref(ull& n, size_t p) : ptr(&n), pos(p) {}

	Bitref& operator = (bool f) {
		if (f) {
			*(this->ptr) |= (ull)1 << this->pos;
        } else {
			*(this->ptr) &= ~((ull)1 << this->pos);
        }
		return *this;
	}

	Bitref& operator = (Bitref& ref) {
		if (*(ref.ptr) & (ull)1 << ref.pos) {
			*(this->ptr) |= (ull)1 << this->pos;
        } else {
			*(this->ptr) &= ~((ull)1 << this->pos);
        }
		return *this;
	}

	operator bool() const {
		return *(this->ptr) & ((ull)1 << this->pos);
	}
};
 
struct Bitset {
  private:
    using ull = uint64_t;
    static constexpr size_t BITLEN = 64;
    static constexpr size_t SHIFTDIV = 6; // (x >> SHIFTDIV) = x / BITLEN
    
    size_t N;
    std::vector<ull> _bit;

  public:

    // _N要素のf
    Bitset(size_t _N, bool f = false) : N(_N), _bit((N + BITLEN - 1) >> SHIFTDIV, (f ? ~ull(0) : ull(0))) {}
    
    // [l, r)の1の数
    // O((r - l) / w)
    int popcount(int l, int r) const {
        assert(0 <= l && l <= r && r <= N);
        if (l == r) return 0;
        int L = l >> SHIFTDIV;
        int R = (r + BITLEN - 1) >> SHIFTDIV;
        int res = 0;
        {
            int l2 = l & (BITLEN - 1);
            res += __builtin_popcountll(_bit[L] >> l2);
            L++;
        }
        {
            int r2 = r & (BITLEN - 1);
            if(r2) {
                res += __builtin_popcountll(_bit[R - 1] << (BITLEN - r2));
                R--;
            }
        }
        if (L > R) {
            int sm = __builtin_popcountll(_bit[R - 1]);
            return res - sm;
        }
        for (int i = L; i < R; i++) {
            res += __builtin_popcountll(_bit[i]);
        }
        return res;
    }

    // i以前(iを含む)に現れる最も右のfの位置 存在しない場合は-1
    // O(N / w)
    int prev(int i, const bool f) const {
        assert(0 <= i && i < N);
        int I = i >> SHIFTDIV;
        {
            int i2 = i & (BITLEN - 1);
            ull x = (f ? _bit[I] : ~_bit[I]) << (BITLEN - 1 - i2);
            if (x) return i - __builtin_clzll(x);
        }
        for (int j = I; j < (int)_bit.size(); j++) {
            ull x = (f ? _bit[j] : ~_bit[j]);
            if (x) return ((j + 1) << SHIFTDIV) - 1 - __builtin_clzll(x);
        }
        return -1;
    }
    // i以降(iを含む)に現れる最も左のfの位置 存在しない場合は-1
    // O(N / w)
    int next(int i, const bool f) const {
        assert(0 <= i && i < N);
        int I = i >> SHIFTDIV;
        {
            int i2 = i & (BITLEN - 1);
            ull x = (f ? _bit[I] : ~_bit[I]) >> i2;
            if (x) {
                int pos = __builtin_ctzll(x);
                if (pos + i < N) return pos + i;
            }
        }
        for (int j = I; j < (int)_bit.size(); j++) {
            ull x = (f ? _bit[j] : ~_bit[j]);
            if (x) {
                int pos = __builtin_ctzll(x);
                if ((j << SHIFTDIV) + pos < N) return (j << SHIFTDIV) + pos;
            }
        }
        return -1;
    }

    // k番目のfの場所 存在しない場合は-1
    // O(N / w)
    int select(int k, const bool f){
        for (int i = 0; i < (int)_bit.size(); i++) {
            ull x = (f ? _bit[i] : ~_bit[i]);
            int pop = __builtin_popcountll(x);
            if (pop > k) {
                int pos = (i << SHIFTDIV) + select_64bit(x, k);
                if(pos < N) return pos;
            } else {
                k -= pop;
            }
        }
        return -1;
    }

    Bitref operator [] (size_t i) {
        return Bitref(_bit[i >> SHIFTDIV], i & (BITLEN - 1));
    }
};

#endif