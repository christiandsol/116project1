#pragma once
#include <stdlib.h>
#include <bitset>

template <size_t N, size_t M>
std::bitset<N> sliceBits(const std::bitset<M>& bits, size_t start, size_t length) {
    std::bitset<N> result;
    for (size_t i = 0; i < length; ++i) {
        result[i] = bits[start + i];
    }
    return result;
}

