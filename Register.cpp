#include "Register.h"

Register::Register()
{
	cur_val = 0;
	next_val = 0;
}

int Register::get_cur_val() {
	cout << hex << uppercase << cur_val;
	return cur_val;
}
int Register::get_next_val() {
	cout << hex << uppercase << next_val;
	return next_val;
}

int Register::set_next_val(int new_val) {
	cout << "Previous value: " << cur_val << "Setting next value to: "<< new_val;
	cur_val = new_val;
	next_val = 0;
	return cur_val;
}
