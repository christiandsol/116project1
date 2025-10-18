#include "CPU.h"
#include "util.h"
#include <cstddef>
#include <sstream>


CPU::CPU()
{
	PC = 0; //set PC to 0
	for (int i = 0; i < 4096; i++) //copy instrMEM
	{
		dmemory[i] = (0);
	}
}


unsigned long CPU::readPC()
{
	return PC;
}
void CPU::incPC()
{
	PC++;
}


CPU::InstrFunc CPU::read_instr(bitset<32> bits) {
    bitset<7> opcode = sliceBits<7>(bits, 0, 7);
    bitset<5> rs1 = sliceBits<5>(bits, 15, 5);
    bitset<5> rs2 = sliceBits<5>(bits, 20, 5);
    bitset<5> rd  = sliceBits<5>(bits, 7, 5);

    cout << "opcode: " << opcode << endl;
    cout << "rs1:    " << rs1 << endl;
    cout << "rs2:    " << rs2 << endl;
    cout << "rd:     " << rd  << endl;

    unsigned long opcode_val = opcode.to_ulong();
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
        default:
            cerr << "Unknown opcode: " << opcode << endl;
            break;
    }
    return func;
}


// In CPU.cpp

void CPU::exec_Itype(CPU* cpu, std::bitset<32> bits) {
    // placeholder for I-type instructions (ADDI, ORI, SLTIU)
}

void CPU::exec_LUI(CPU* cpu, std::bitset<32> bits) {
    // placeholder for LUI instruction
}

void CPU::exec_Rtype(CPU* cpu, std::bitset<32> bits) {
    // placeholder for R-type instructions (ADD, SUB, AND, SRA)
}

void CPU::exec_Load(CPU* cpu, std::bitset<32> bits) {
    // placeholder for Load instructions (LBU, LW)
}

void CPU::exec_Store(CPU* cpu, std::bitset<32> bits) {
    // placeholder for Store instructions (SH, SW)
}

void CPU::exec_Branch(CPU* cpu, std::bitset<32> bits) {
    // placeholder for Branch instructions (BNE)
}

void CPU::exec_JALR(CPU* cpu, std::bitset<32> bits) {
    // placeholder for JALR instruction
}


Instruction::Instruction() {
}

bitset<32> Instruction::fetch(char *&mem) {
    bitset<32> bits;
    size_t bitIndex = 0;

    for (int byteIdx = 0; byteIdx < 2; ++byteIdx) {
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

    cout << "bits: " << bits << endl;
    return bits;
}


void Instruction::print_instr(bitset<32> instr){
	unsigned long val = instr.to_ulong();
    cout << hex << val << endl;
}
