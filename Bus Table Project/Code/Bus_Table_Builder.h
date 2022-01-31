#pragma once
#include "Bus_Table.h"

class Bus_Table_Builder
{
public:
	Bus_Table_Builder();
	~Bus_Table_Builder();

	Bus_Table fullfil(char* Company_name, char* depart_time_h, char* depart_time_m,char* arrival_time_h, char* arrival_time_m);
};