#pragma once
#include "Register.h"

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
  unsigned long PC;  // pc
  
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
  CPU(char* mem);
  unsigned long readPC();
  Register registers[30];
  void incPC();
  using InstrFunc = void (*)(CPU *, std::bitset<32>);

  InstrFunc read_instr(std::bitset<32> bits);
  void set_controls(std::bitset<32> bits);

  static void exec_Itype(CPU *cpu, std::bitset<32> bits);
  static void exec_LUI(CPU *cpu, std::bitset<32> bits);
  static void exec_Rtype(CPU *cpu, std::bitset<32> bits);
  static void exec_Load(CPU *cpu, std::bitset<32> bits);
  static void exec_Store(CPU *cpu, std::bitset<32> bits);
  static void exec_Branch(CPU *cpu, std::bitset<32> bits);
  static void exec_JALR(CPU *cpu, std::bitset<32> bits);

  template <size_t N> int immediate_gen(std::bitset<N> bits, bool uns);
};

// add other functions and objects here

template <size_t N> int CPU::immediate_gen(std::bitset<N> bits, bool uns) {
  if (uns) {
    return static_cast<int>(bits.to_ulong());
  }
  if (bits.test(N - 1)) {
    unsigned long value = bits.to_ulong();
    long signed_value = static_cast<long>(value | (~((1UL << N) - 1)));
    return static_cast<int>(signed_value);
  } else {
    return static_cast<int>(bits.to_ulong());
  }
}
