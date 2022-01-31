#include <iostream>
#include "Bus_Table.h"

Bus_Table::Bus_Table(std::string Company_name, int depart_time, int arrival_time) {
	this->Company_name = Company_name;
	this->depart_time = depart_time;
	this->arrival_time = arrival_time;
};
Bus_Table::Bus_Table() {};
std::string Bus_Table::toString() {
	std::string Dth = std::to_string(depart_time / 60);
	std::string Dtm = std::to_string(depart_time % 60);
	std::string Ath = std::to_string(arrival_time / 60);
	std::string Atm = std::to_string(arrival_time % 60);
	if (arrival_time % 60 < 10) {
		Atm = "0" + std::to_string(arrival_time % 60);
	}
	if (depart_time % 60 < 10) {
		Dtm = "0" + std::to_string(depart_time % 60);
	}
	std::string str = Company_name + " " + Dth + ":" + Dtm + " " + Ath + ":" + Atm;
	return str;
}
Bus_Table::~Bus_Table() {};