#include "CPU.h"
#include "Register.h"
#include "util.h"
#include "Memory.h"

#include <iostream>
#include <bitset>
#include <stdio.h>
#include<stdlib.h>
#include <string>
#include<fstream>
#include <sstream>
#include <valarray>
using namespace std;

/*
Add all the required standard and developed libraries here
*/


/*
Put/Define any helper function/definitions you need here
*/
int main(int argc, char* argv[])
{
	/* This is the front end of your project.
	You need to first read the instructions that are stored in a file and load them into an instruction memory.
	*/

	/* Each cell should store 1 byte. You can define the memory either dynamically, or define it as a fixed size with size 4KB (i.e., 4096 lines). Each instruction is 32 bits (i.e., 4 lines, saved in little-endian mode).
	Each line in the input file is stored as an hex and is 1 byte (each four lines are one instruction). You need to read the file line by line and store it into the memory. You may need a mechanism to convert these values to bits so that you can read opcodes, operands, etc.
	*/

	char instMem[4096];


	if (argc < 2) {
		//cout << "No file name entered. Exiting...";
		return -1;
	}

	ifstream infile(argv[1]); //open the file
	if (!(infile.is_open() && infile.good())) {
		cout<<"error opening file\n";
		return 0; 
	}
	string line; 
	int i = 0;
	while (infile) {
			infile>>line;
			stringstream line2(line);
			char x; 
			line2>>x;
			instMem[i] = x; // be careful about hex
			i++;
			line2>>x;
			instMem[i] = x; // be careful about hex
			// cout<<instMem[i]<<endl;
			i++;
		}
	int maxPC= i/4; 

	/* Instantiate your CPU object here.  CPU class is the main class in this project that defines different components of the processor.
	CPU class also has different functions for each stage (e.g., fetching an instruction, decoding, etc.).
	*/

	CPU myCPU(instMem);  // call the approriate constructor here to initialize the processor...  
	// make sure to create a variable for PC and resets it to zero (e.g., unsigned int PC = 0); 
	Instruction myInst; 
	char* mem = instMem;


	//Memory
	Memory myMemory;
	
	bool done = true;
	while (done == true) // processor's main loop. Each iteration is equal to one clock cycle.  
	{
		//get instruction
		unsigned long prev_PC = myCPU.PC.cur_pc;
		bitset<32> instr = myInst.fetch(mem, myCPU.PC.next_pc, myCPU.PC.cur_pc);
		
		// cout << "Instruction: " << hex << to_int(instr, false) << endl;
		unsigned long PC_4 = myCPU.PC.cur_pc + 8;

		// set control bits for whole CPU
		myCPU.set_controls(instr); 
		myMemory.set_controls(instr);
		// extract and store information for immediate gen
		bitset<5> b_rs1 = sliceBits<5>(instr, 15, 5);
		bitset<5> b_rs2 = sliceBits<5>(instr, 20, 5);
		bitset<5> b_rd = sliceBits<5>(instr, 7, 5);
		bitset<3> b_func3= sliceBits<3>(instr, 12, 3);
		unsigned long rs1 = static_cast<int>(b_rs1.to_ulong());
		unsigned long rs2 = static_cast<int>(b_rs2.to_ulong());
		unsigned long rd = static_cast<int>(b_rd.to_ulong());
		unsigned long func3 = static_cast<int>(b_func3.to_ulong());
		int read1 = myCPU.registers[rs1].get_cur_val();
		int read2 = myCPU.registers[rs2].get_cur_val();
		int imm = myCPU.immediate_gen(instr);

		//Perform ALU operation After multiplexing 
		CPU::ALU operation = myCPU.ALU_ctrl(b_func3);
		int val2 = MUX(imm, read2, myCPU.ctrl.alu_src);
		if (operation== nullptr)
			break;
		(myCPU.*operation)(rd, read1, val2); //sets the next val's of the CPU internally
		cout << "x" << rd << " x" << rs1 << " x" << rs2 << " imm: " << hex << imm << dec <<endl;
		int ne = SUB(read1, val2) != 0 ? 1 : 0;
		//set up memory
		myMemory.set_controls(instr);

		// cout << hex << "Branch jump: " << imm + myCPU.PC.cur_pc << dec << endl;
		// cout << hex << "CURPC : " << myCPU.PC.cur_pc << dec << endl;
		unsigned long branch_PC = MUX(imm * 2 + myCPU.PC.cur_pc, myCPU.PC.next_pc, AND(myCPU.ctrl.branch, ne));

		myCPU.PC.next_pc = MUX((imm + myCPU.registers[rs1].get_cur_val()) * 2, branch_PC, myCPU.ctrl.jump);

		int reg_write_mem = myMemory.execute_mem(imm + myCPU.registers[rs1].get_cur_val(), myCPU.registers[rs2].get_cur_val());

		// write to registers if flag set
		int reg_commit = MUX(reg_write_mem, myCPU.registers[rd].get_next_val(), myCPU.ctrl.mem_to_reg);
		myCPU.registers[rd].set_next_val(reg_commit);
		// next_val 

		myCPU.registers[rd].set_next_val(MUX(PC_4 / 2, myCPU.registers[rd].get_next_val(), myCPU.ctrl.jump));

		//commit nextPC
		if (myCPU.ctrl.reg_write && rd != 0) {
			myCPU.registers[rd].commit_next_val(rd); 
		}
		myInst.commit_next_PC(myCPU.PC.next_pc, myCPU.PC.cur_pc);
	}
	int a0 =myCPU.registers[10].get_cur_val();
	int a1 =myCPU.registers[11].get_cur_val();
	// print the results (you should replace a0 and a1 with your own variables that point to a0 and a1)
	  cout << "(" << a0 << "," << a1 << ")" << endl;
	
	return 0;

}
