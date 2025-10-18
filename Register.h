#pragma once
#include <iostream>
#include <stdio.h>
#include<stdlib.h>

using namespace std;

class Register {
private:
	int cur_val;
	int next_val;
public:
	Register();
	int get_cur_val();
	int get_next_val();
	int set_next_val(int next_val);
};
