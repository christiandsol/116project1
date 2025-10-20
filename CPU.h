#pragma once
#include "Register.h"
#include "util.h"

#include <bitset>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_map>
using namespace std;

class Instruction {
public:
  bitset<32> instr; // instruction
  Instruction();    // constructor
  bitset<32> fetch(char *&mem);
  void print_instr(bitset<32> instr);
};

class CPU {
private:
  unordered_map<uint32_t, char> memory_map;
  char dmemory[4096]; // data memory byte addressable in little endian fashion;
  unsigned long PC;   // pc

public:
  struct Control {
    unsigned int jump;
    unsigned int branch;
    unsigned int mem_read;
    unsigned int mem_to_reg;
    unsigned int alu_op;
    unsigned int mem_write;
    unsigned int alu_src;
    unsigned int reg_write;
  };
  Control ctrl;
  CPU(char *mem);
  unsigned long readPC();
  Register registers[30];
  using ALU = void (CPU::*)(int, int, int); // rd, read1, read2, func3
  ALU read_instr(std::bitset<32> bits);

  void set_controls(std::bitset<32> bits);
  void incPC();
  ALU ALU_ctrl(bitset<3> func3);

  void exec_sub(int rd, int r1, int r2);
  void exec_and(int rd, int r1, int r2);
  void exec_sra(int rd, int r1, int r2);
  void exec_addi(int rd, int r1, int r2);
  void exec_sltiu(int rd, int r1, int r2);
  void exec_ori(int rd, int r1, int r2);
  void exec_Itype(int rd, int r1, int r2);
  void exec_LUI(int rd, int r1, int r2);
  void exec_Rtype(int rd, int r1, int r2);
  // void exec_Load(int rd, int r1, int r2, int func3);
  // void exec_Store(int rd, int r1, int r2, int func3);
  // void exec_Branch(int rd, int r1, int r2, int func3);
  // void exec_JALR(int rd, int r1, int r2, int func3);
  template <size_t N> int to_int(std::bitset<N> bits);
  int immediate_gen(std::bitset<32> bits);
  // add other functions and objects here
};

template <size_t N> int CPU::to_int(std::bitset<N> bits) {
  if (bits.test(N - 1)) {
    unsigned long value = bits.to_ulong();
    long signed_value = static_cast<long>(value | (~((1UL << N) - 1)));
    return static_cast<int>(signed_value);
  } else {
    return static_cast<int>(bits.to_ulong());
  }
}
