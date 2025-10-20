#include "CPU.h"
#include "util.h"
#include <cstddef>
#include <sstream>
#include <valarray>

CPU::CPU() {
  PC = 0;                        // set PC to 0
  for (int i = 0; i < 4096; i++) // copy instrMEM
  {
    dmemory[i] = (0);
  }
}

unsigned long CPU::readPC() { return PC; }
void CPU::incPC() { PC++; }

CPU::InstrFunc CPU::read_instr(bitset<32> bits) {
  bitset<7> opcode = sliceBits<7>(bits, 0, 7);
  bitset<5> rs1 = sliceBits<5>(bits, 15, 5);
  bitset<5> rs2 = sliceBits<5>(bits, 20, 5);
  bitset<5> rd = sliceBits<5>(bits, 7, 5);
  bitset<12> imm = sliceBits<12>(bits, 20, 12);

  // cout << "opcode: " << opcode << endl;
  // cout << "rs1:    " << rs1 << endl;
  // cout << "rs2:    " << rs2 << endl;
  // cout << "rd:     " << rd << endl;
  // cout << "imm:     " << imm << endl;

  int opcode_val = CPU::immediate_gen(opcode, true);
  InstrFunc func = nullptr;

  switch (opcode_val) {
  case 0b0010011: // ADDI, ORI, SLTIU
    func = &CPU::exec_Itype;
    break;
  case 0b0110111: // LUI
    func = &CPU::exec_LUI;
    break;
  case 0b0110011: // R-type (ADD, SUB, AND, SRA)
    func = &CPU::exec_Rtype;
    break;
  case 0b0000011: // LBU, LW
    func = &CPU::exec_Load;
    break;
  case 0b0100011: // SH, SW
    func = &CPU::exec_Store;
    break;
  case 0b1100011: // BNE
    func = &CPU::exec_Branch;
    break;
  case 0b1100111: // JALR
    func = &CPU::exec_JALR;
    break;
  case 0b0000000: // terminate 
    func = nullptr;
    break;
  default:
    cerr << "Unknown opcode: " << opcode_val << endl;
    break;
  }
  return func;
}

// In CPU.cpp

void CPU::exec_Itype(CPU *cpu, std::bitset<32> bits) {
  // placeholder for I-type instructions (ADDI, ORI, SLTIU)
  // ADDI -> 000
  // SLTIU -> 011
  // ORI -> 110
  bitset<3> b_func3 = sliceBits<3>(bits, 12, 3);
  bitset<12> b_immediate = sliceBits<12>(bits, 20, 12);
  bitset<5> b_rs1 = sliceBits<5>(bits, 15, 5);
  bitset<5> b_rd = sliceBits<5>(bits, 7, 5);
  int func3 = cpu->immediate_gen(b_func3, true);
  int immediate = cpu->immediate_gen(b_immediate, false);
  int rs1 = cpu->immediate_gen(b_rs1, true);
  int rd = cpu->immediate_gen(b_rd, true);
  int cur_val = cpu->registers[rs1].get_cur_val();
  switch (func3) {
  case 0b000: { // ADDI
    print_itype("addi", rd, rs1, immediate);
    cpu->registers[rd].set_next_val(cur_val + immediate);
    break;
  }
  case 0b011: { // SLTIU
    int uns_imm = cpu->immediate_gen(b_immediate, true);
    print_itype("sltiu", rd, rs1, uns_imm);
    if (cpu->registers[rs1].get_cur_val() < uns_imm) {
      cpu->registers[rd].set_next_val(1);
    } else {
      cpu->registers[rd].set_next_val(0);
    }
    break;
  }
  case 0b110: { // ORI
      print_itype("ori", rd, rs1, immediate);
    cpu->registers[rd].set_next_val(cur_val | immediate);
    break;
  }
  default:
    cerr << "Unknown opcode: " << func3 << endl;
    break;
  }
}

void CPU::exec_LUI(CPU *cpu, std::bitset<32> bits) {
  // placeholder for LUI instruction
  bitset<20> b_immediate = sliceBits<20>(bits, 12, 31);
  bitset<5> b_rd = sliceBits<5>(bits, 7, 5);
  int immediate = cpu->immediate_gen(b_immediate, false);
  int rd = cpu->immediate_gen(b_rd, true);
  print_itype("lui", rd, -1, immediate);
  immediate <<= 12;
  cpu->registers[rd].set_next_val(immediate);
}

void CPU::exec_Rtype(CPU *cpu, std::bitset<32> bits) {
  // placeholder for R-type instructions (SUB, AND, SRA)
  bitset<5> b_rs1 = sliceBits<5>(bits, 15, 5);
  bitset<5> b_rs2 = sliceBits<5>(bits, 20, 5);
  bitset<5> b_rd = sliceBits<5>(bits, 7, 5);
  bitset<3> b_func3 = sliceBits<3>(bits, 12, 3);
  // bitset<7> b_func7 = sliceBits<7>(bits, 12, 7);
  int rs1 = cpu->immediate_gen(b_rs1, true);
  int rs2 = cpu->immediate_gen(b_rs2, true);
  int rd = cpu->immediate_gen(b_rd, true);
  int func3 = cpu->immediate_gen(b_func3, true);
  int val1 = cpu->registers[rs1].get_cur_val();
  int val2 = cpu->registers[rs2].get_cur_val();
  switch (func3) {
    case 0b000: // SUB
      print_rtype("sub", rd, rs1, rs2);
      cpu->registers[rd].set_next_val(val1- val2);
      break;
    case 0b111: // AND
      print_rtype("and", rd, rs1, rs2);
      cpu->registers[rd].set_next_val(val1 & val2);
      break;
    case 0b101: //SRA
      print_rtype("sra", rd, rs1, rs2);
      cpu->registers[rd].set_next_val(val1 >> val2);
      break;
  }
}

void CPU::exec_Load(CPU *cpu, std::bitset<32> bits) {
  // placeholder for Load instructions (LBU, LW)
}

void CPU::exec_Store(CPU *cpu, std::bitset<32> bits) {
  // placeholder for Store instructions (SH, SW)
}

void CPU::exec_Branch(CPU *cpu, std::bitset<32> bits) {
  // placeholder for Branch instructions (BNE)
}

void CPU::exec_JALR(CPU *cpu, std::bitset<32> bits) {
  // placeholder for JALR instruction
}

Instruction::Instruction() {}

bitset<32> Instruction::fetch(char *&mem) {
  bitset<32> bits;
  size_t bitIndex = 0;

  for (int byteIdx = 0; byteIdx < 4; ++byteIdx) {
    char high = mem[0];
    char low = mem[1];

    unsigned int byteVal = 0;
    stringstream ss;
    ss << hex << high << low;
    ss >> byteVal;

    for (int bit = 0; bit < 8; ++bit) {
      bits.set(bitIndex + bit, (byteVal >> bit) & 1);
    }

    mem += 2;
    bitIndex += 8;
  }

  // cout << "Instruction: "; 
  // print_instr(bits);
  // cout << dec;
  return bits;
}

void Instruction::print_instr(bitset<32> instr) {
  unsigned long val = instr.to_ulong();
  cout << hex << val << endl;
  cout << dec;
}
