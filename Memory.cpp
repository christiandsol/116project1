#include "Memory.h"
#include "util.h"
#include <atomic>
#include <bitset>
#include <functional>
using namespace std;

int Memory::get_func3() { return func3; }
int Memory::get_mem_write() { return mem_write; }
int Memory::get_mem_read() { return mem_read; }
void Memory::set_func3(int val) { func3 = val; }
void Memory::set_mem_write(int val) { mem_write = val; }
void Memory::set_mem_read(int val) { mem_read = val; }

void Memory::store(int addr, int size, int value) {
  for (int i = 0; i < size; i++) {
    unsigned char byte = (value >> (8 * i)) & 0xFF;
    memory[addr + i] = byte;
  }
}

int Memory::load(int addr, int size) {
  int value = 0;
  for (int i = 0; i < size; i++) {
    unsigned char byte = memory[addr + i];
    value |= (byte << (8 * i));
  }
  cout << "Loaded value: " << dec << value << endl;
  return value;
}

int Memory::execute_mem(int addr, int value) {
  int ret_val = -1;
  unsigned int test =  static_cast<unsigned int>(func3);
  if (get_mem_write()) {
    switch (test) {
    case 0b001: // SH
      store(addr, 2, value);
      break;
    case 0b010: // SW
      store(addr, 4, value);
      break;
    default:
      break;
    }
  } else if (get_mem_read()) {
    switch (test) {
    case 0b100: // LBU
      ret_val = load(addr, 1);
      break;
    case 0b010: // LW
      ret_val = load(addr, 4);
      break;
    }
  }
  return ret_val;
}

void Memory::set_controls(bitset<32> instr) {
  bitset<7> opcode = sliceBits<7>(instr, 0, 7);
  int opcode_val = to_int(opcode, true);
  bitset<3> b_func3 = sliceBits<3>(instr, 12, 3);
  int _func3 = to_int(b_func3, true);
  func3 = _func3;

  switch (opcode_val) {
  case 0b0010011: // ADDI, ORI, SLTIU
    mem_read = 0;
    mem_write = 0;
    break;
  case 0b0110111: // LUI
    mem_read = 0;
    mem_write = 0;
    break;
  case 0b0110011: // R-type (ADD, SUB, AND, SRA)
    mem_read = 0;
    mem_write = 0;
    break;
  case 0b0000011: // LBU, LW
    mem_read = 1;
    mem_write = 0;
    break;
  case 0b0100011: // SH, SW
    mem_read = 0;
    mem_write = 1;
    break;
  case 0b1100011: // BNE
    mem_read = 0;
    mem_write = 0;
    break;
  case 0b1100111: // JALR
    mem_read = 0;
    mem_write = 0;
    break;
  case 0b0000000: // terminate
    mem_read = 0;
    mem_write = 0;
    break;
  default:
    cout << "Unknown opcode: " << opcode_val << endl;
    break;
  }
}
