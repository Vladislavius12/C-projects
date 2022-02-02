#pragma once
#include "Bus_Table.h"

class Bus_Table_Builder
{
public:
	Bus_Table_Builder();
	~Bus_Table_Builder();

	Bus_Table fullfil(char* Company_name, int depart_time, int arrival_time);
};