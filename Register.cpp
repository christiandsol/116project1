#include "Register.h"

Register::Register()
{
	cur_val = 0;
	next_val = 0;
}

int Register::get_cur_val() {
	// cout << "Cur val: " << hex << uppercase << cur_val << endl;
	// cout << dec;
	return cur_val;
}
int Register::get_next_val() {
	// cout << "Next val: " << hex << uppercase << next_val;
	// cout << dec;
	return next_val;
}



int Register::set_next_val(int new_val) {
	// cout << "Setting register's next_val to " << hex << new_val << dec << endl;
	next_val = new_val;
	return new_val;
}

void Register::commit_next_val() {
	cout << "Commiting register to " << hex << next_val << dec << endl;
	cur_val = next_val;
	next_val = 0;
}


