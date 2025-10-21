#pragma once
#include <bitset>
#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>

using namespace std;
class Memory {
private:
	int func3;
	int mem_write;
	int mem_read;
public:
  unordered_map<int, char> memory;
	int get_func3();
	int get_mem_write();
	int get_mem_read();
	void set_func3(int val);
	void set_mem_write(int val);
	void set_mem_read(int val);
	void store(int addr, int size, int value);
	int load(int addr, int size);
	int execute_mem(int addr, int value);
	void set_controls(bitset<32>instr);
};

