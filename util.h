#pragma once
#include <bitset>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

template <size_t N, size_t M>
std::bitset<N> sliceBits(const std::bitset<M> &bits, size_t start,
                         size_t length) {
  std::bitset<N> result;
  for (size_t i = 0; i < length; ++i) {
    result[i] = bits[start + i];
  }

  return result;
}

void print_rtype(std::string op, int rd, int rs1, int rs2);
void print_itype(std::string op, int rd, int rs1, int imm);
