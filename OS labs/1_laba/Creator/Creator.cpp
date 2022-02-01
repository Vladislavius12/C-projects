#include <iostream>
#include <fstream>
#include "../Employee.h"
using namespace std;

int main(int argc, char* argv[]) {
	ofstream out(argv[0], ios::binary);
	int countOfRec = atoi(argv[1]);
	for (int i = 0; i < countOfRec; i++) {
		employee empl;
		cout << "Input employee id: ";
		cin >> empl.num;
		cout << "Input employee name: ";
		cin >> empl.name;
		cout << "Input worked hours: ";
		cin >> empl.hours;
		out.write((char*)&empl, sizeof(employee));
	}
	out.close();
}