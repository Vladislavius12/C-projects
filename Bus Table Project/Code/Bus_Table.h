#pragma once
#include <string>

class Bus_Table
{
private:
	std::string Company_name;
	int depart_time; //Departure time in minutes
	int arrival_time; //Arrival time in minutes
public:
	Bus_Table();
	Bus_Table(std::string Company_name, int depart_time, int arrival_time);
	friend bool operator <(const Bus_Table& bt1, const Bus_Table& bt2);
	int getDepart_time() { return depart_time; }
	int getnArrival_time() { return arrival_time; }
	std::string getCompany_name() { return Company_name; }
	std::string toString();
	~Bus_Table();
};