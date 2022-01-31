#include "Bus_Table_Builder.h"

Bus_Table_Builder::Bus_Table_Builder() {};
Bus_Table_Builder::~Bus_Table_Builder() {};
Bus_Table Bus_Table_Builder::fullfil(char* Company_name, char* depart_time_h, char* depart_time_m, char* arrival_time_h, char* arrival_time_m) {
    int depart_time_h_numb = atoi(depart_time_h);
    int depart_time_m_numb = atoi(depart_time_m);
    int arrival_time_h_numb = atoi(arrival_time_h);
    int arrival_time_m_numb = atoi(arrival_time_m);

    int depart_time = depart_time_h_numb * 60 + depart_time_m_numb;
    int arrival_time = arrival_time_h_numb * 60 + arrival_time_m_numb;

    std::string Company_name_str = std::string(Company_name);

   
    return Bus_Table(Company_name_str,depart_time,arrival_time);
};