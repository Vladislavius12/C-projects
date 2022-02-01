#include <iostream>
#include <fstream>
#include <string>
#include "../Employee.h"
using namespace std;

int main(int argc, char* argv[]) {
	ifstream bin(argv[0], ios::binary);
	ofstream report(argv[1]);
	int moneyPerHour = atoi(argv[2]);

	report << "����� �� ����� " << argv[0] << endl;
	report << "����� ����������, ��� ����������, ����, ��������." << endl;

	employee record;
	while (bin.read((char*)&record, sizeof(employee))) {
		report << record.num << "\t" << record.name << "\t" << record.hours << "\t" << record.hours * moneyPerHour << endl;
	}


	bin.close();
	report.close();
}