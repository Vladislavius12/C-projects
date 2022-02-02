#include "Bus_Table_Builder.h"

Bus_Table_Builder::Bus_Table_Builder() {};
Bus_Table_Builder::~Bus_Table_Builder() {};
Bus_Table Bus_Table_Builder::fullfil(char* Company_name ,int depart_time, int arrival_time) {
    std::string Company_name_str = std::string(Company_name);
    return Bus_Table(Company_name_str,depart_time,arrival_time);
};