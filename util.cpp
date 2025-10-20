#include "util.h"

void print_rtype(std::string op, int rd, int rs1, int rs2){
    cout << op << " x" << rd << ", x" << rs1 << ", x" << rs2 << endl;
}

void print_itype(std::string op, int rd, int rs1, int imm) {
	if (rs1 == -1) {
    cout << op << " x" << rd << ", 0x" <<hex << imm << dec << endl;
	} else {
    cout << op << " x" << rd << ", x" << rs1 << ", " << imm << endl;
	}
}

