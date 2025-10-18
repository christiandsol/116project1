#pragma once
#include "Register.h"

#include <iostream>
#include <bitset>
#include <stdio.h>
#include<stdlib.h>
#include <string>
using namespace std;


class Instruction {
public:
	bitset<32> instr;//instruction
	Instruction(); // constructor
	bitset<32> fetch(char *&mem);
	void print_instr(bitset<32> instr);
};

class CPU {
private:
	int dmemory[4096]; //data memory byte addressable in little endian fashion;
	unsigned long PC; //pc 
	Register registers[30];
public:
	CPU();
	unsigned long readPC();
	void incPC();
    using InstrFunc = void (*)(CPU*, std::bitset<32>);

    InstrFunc read_instr(std::bitset<32> bits);

    static void exec_Itype(CPU* cpu, std::bitset<32> bits);
    static void exec_LUI(CPU* cpu, std::bitset<32> bits);
    static void exec_Rtype(CPU* cpu, std::bitset<32> bits);
    static void exec_Load(CPU* cpu, std::bitset<32> bits);
    static void exec_Store(CPU* cpu, std::bitset<32> bits);
    static void exec_Branch(CPU* cpu, std::bitset<32> bits);
    static void exec_JALR(CPU* cpu, std::bitset<32> bits);
};

// add other functions and objects here
