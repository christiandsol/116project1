#include "CPU.h"
#include "util.h"
#include <bitset>
#include <cstddef>
#include <sstream>

CPU::CPU(char *mem) {
  PC.cur_pc = 0;
  PC.next_pc = 0;
  ctrl.jump = 0;
  ctrl.branch = 0;
  ctrl.mem_read = 0;
  ctrl.mem_to_reg = 0;
  ctrl.alu_op = 0;
  ctrl.mem_write = 0;
  ctrl.alu_src = 0;
  ctrl.reg_write = 0;
}


void CPU::set_controls(bitset<32> bits) {
  bitset<7> opcode = sliceBits<7>(bits, 0, 7);
  int opcode_val = to_int(opcode, false);
  switch (opcode_val) {
  case 0b0010011: // ADDI, ORI, SLTIU
    ctrl.jump = 0;
    ctrl.branch = 0;
    ctrl.mem_read = 0;
    ctrl.mem_to_reg = 0;
    ctrl.alu_op = 0b001;
    ctrl.mem_write = 0;
    ctrl.alu_src = 1; // 1 = immediate
    ctrl.reg_write = 1;
    break;
  case 0b0110111: // LUI
    ctrl.jump = 0;
    ctrl.branch = 0;
    ctrl.mem_read = 0;
    ctrl.mem_to_reg = 0;
    ctrl.alu_op = 0b010;
    ctrl.mem_write = 0;
    ctrl.alu_src = 1; // 1 = immediate
    ctrl.reg_write = 1;
    break;
  case 0b0110011: // R-type (ADD, SUB, AND, SRA)
    ctrl.jump = 0;
    ctrl.branch = 0;
    ctrl.mem_read = 0;
    ctrl.mem_to_reg = 0;
    ctrl.alu_op = 0b011;
    ctrl.mem_write = 0;
    ctrl.alu_src = 0; // 1 = immediate
    ctrl.reg_write = 1;
    break;
  case 0b0000011: // LBU, LW
    ctrl.jump = 0;
    ctrl.branch = 0;
    ctrl.mem_read = 1;
    ctrl.mem_to_reg = 1;
    ctrl.alu_op = 0b100;
    ctrl.mem_write = 0;
    ctrl.alu_src = 1; // 1 = immediate
    ctrl.reg_write = 1;
    break;
  case 0b0100011: // SH, SW
    ctrl.jump = 0;
    ctrl.branch = 0;
    ctrl.mem_read = 0;
    ctrl.mem_to_reg = 0;
    ctrl.alu_op = 0b101;
    ctrl.mem_write = 1;
    ctrl.alu_src = 1; // 1 = immediate
    ctrl.reg_write = 0;
    break;
  case 0b1100011: // BNE
    ctrl.jump = 0;
    ctrl.branch = 1;
    ctrl.mem_read = 0;
    ctrl.mem_to_reg = 0;
    ctrl.alu_op = 0b110;
    ctrl.mem_write = 0;
    ctrl.alu_src = 1;
    ctrl.reg_write = 0;
    break;
  case 0b1100111: // JALR
    ctrl.jump = 1;
    ctrl.branch = 0;
    ctrl.mem_read = 0;
    ctrl.mem_to_reg = 0;
    ctrl.alu_op = 0b111;
    ctrl.mem_write = 0;
    ctrl.alu_src = 1;
    ctrl.reg_write = 1;
    break;
  case 0b0000000: // terminate
    ctrl.jump = 0;
    ctrl.branch = 0;
    ctrl.mem_read = 0;
    ctrl.mem_to_reg = 0;
    ctrl.alu_op = 0b000;
    ctrl.mem_write = 0;
    ctrl.alu_src = 0;
    ctrl.reg_write = 0;
    break;
  default:
    cerr << "Unknown opcode: " << opcode_val << endl;
    break;
  }
}

CPU::ALU CPU::ALU_ctrl(bitset<3> func3) {
  if (ctrl.alu_op == 0b010) {
    return &CPU::exec_LUI;
  }
  ALU func = nullptr;
  int lower = to_int(func3, true);

  int val = (ctrl.alu_op << 3) | lower;
  cout << "VAL: " << val << endl;
  switch (val) {
  case 0b001000: // ADDI
    func = &CPU::exec_addi;
    break;
  case 0b001110: // ORI
    func = &CPU::exec_ori;
    break;
  case 0b001011: // SLTIU
    func = &CPU::exec_sltiu;
    break;
  case 0b011101: // SRA
    func = &CPU::exec_sra;
    break;
  case 0b011000: // SUB
    func = &CPU::exec_sub;
    break;
  case 0b011111: // AND
    func = &CPU::exec_and;
    break;
  // case 0x100:
  // case 0x101:
  // case 0x110:
  // case 0x111:
  case 0x00000:
    func = nullptr;
    break;
  default:
    func = &CPU::exec_addi;
    cout << "HERE 1" << endl;
    break;
  }
  return func;
}

// In CPU.cpp

void CPU::exec_addi(int rd, int r1, int r2) {
  cout << "addi ";
  // print_itype("addi", rd, r1, r2);
  registers[rd].set_next_val(r1 + r2);
}
void CPU::exec_sltiu(int rd, int r1, int r2) {
  cout << "sltiu ";
  if (r1< r2) {
    registers[rd].set_next_val(1);
  } else {
    registers[rd].set_next_val(0);
  }
}
void CPU::exec_ori(int rd, int r1, int r2) {
  cout << "ori ";
  // print_itype("ori", rd, r1, r2);
  registers[rd].set_next_val(r1 | r2);
}

void CPU::exec_LUI(int rd, int r1, int r2) {
  cout << "lui ";
  // print_itype("lui", rd, -1, r2);
  r2 <<= 12;
  registers[rd].set_next_val(r2);
}

void CPU::exec_sub(int rd, int r1, int r2) {
  cout << "sub ";
  // print_rtype("sub", rd, r1, r2);
  registers[rd].set_next_val(r1 - r2);
}
void CPU::exec_and(int rd, int r1, int r2) {
  cout << "and ";
  // print_rtype("and", rd, r1, r2);
  registers[rd].set_next_val(r1 & r2);
}
void CPU::exec_sra(int rd, int r1, int r2) {
  cout << "sra ";
  // print_rtype("sra", rd, r1, r2);
  registers[rd].set_next_val(r1 >> r2);
}

Instruction::Instruction() {}

bitset<32> Instruction::fetch(char *&mem, unsigned long &next_PC) {
    bitset<32> bits;
    size_t bitIndex = 0;

    for (int byteIdx = 0; byteIdx < 4; ++byteIdx) {
        char high = mem[next_PC];
        char low  = mem[next_PC + 1];

        unsigned int byteVal = 0;
        stringstream ss;
        ss << hex << high << low;
        ss >> byteVal;

        for (int bit = 0; bit < 8; ++bit) {
            bits.set(bitIndex + bit, (byteVal >> bit) & 1);
        }

        next_PC += 2;        // increment next_PC by 2 chars (1 byte)
        bitIndex += 8;  // increment bit position by 8 bits (1 byte)
    }

    return bits;
}

void Instruction::print_instr(bitset<32> instr) {
  unsigned long val = instr.to_ulong();
  cout << hex << val << endl;
  cout << dec;
}

int CPU::immediate_gen(std::bitset<32> bits) {
  bitset<7> opcode = sliceBits<7>(bits, 0, 7);
  bitset<12> b_imm_i = sliceBits<12>(bits, 20, 12);
  bitset<20> b_imm_lui = sliceBits<20>(bits, 12, 20);
  bitset<5> b_imm_s_lower = sliceBits<5>(bits, 7, 7);
  bitset<7> b_imm_s_upper = sliceBits<7>(bits, 25, 7);
  int opcode_val = to_int(opcode, false);
  int imm_i = to_int(b_imm_i, false);
  int imm_lui = to_int(b_imm_lui, false);
  int imm_s_upper = to_int(b_imm_s_upper, false);
  int imm_s_lower = to_int(b_imm_s_lower, true);
  int imm_s = (imm_s_upper << 5 | imm_s_lower);
  int imm_12 = (bits[31] ? -1 : 0) << 12;              // sign bit
  int imm_10_5 = sliceBits<6>(bits, 25, 6).to_ulong(); // bits 30-25
  int imm_4_1 = sliceBits<4>(bits, 8, 4).to_ulong();   // bits 11-8
  int imm_11 = bits[7] ? 1 : 0;                        // bit 7
  int imm_b = (imm_12) | (imm_11 << 11) | (imm_10_5 << 5) | (imm_4_1 << 1);
  if (imm_b & (1 << 12)) {
    imm_b |= 0xFFFFE000;
  }

  switch (opcode_val) {
  case 0b0010011: // ADDI, ORI, SLTIU
    return imm_i;
    break;
  case 0b0110111: // LUI
    return imm_lui;
    break;
  case 0b0000011: // LBU, LW
    return imm_i;
    break;
  case 0b0100011: // SH, SW
    return imm_s;
    break;
  case 0b1100011: // BNE
    return imm_b;
    break;
  case 0b1100111: // JALR
    return imm_i;
    break;
  default:
    cerr << "Not immediate type: " << opcode_val << endl;
    return -1;
    break;
  }
}
