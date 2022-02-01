#include <windows.h>
#include <iostream>
#pragma warning(disable : 4996)

using namespace std;


//////////////////////////////////////////////////////////
char READ = 'r';
char MODIFY = 'm';
char ORDER_NOT_FOUND = 'n';
char ORDER_FOUND = 'f';
char EXIT = 'e';
char END_OPERATION = 'd';
char END_MODIFY = 'n';

//////////////////////////////////////////////////////////

struct Employee {
	int num;
	char name[10];
	int hours;
};

int main(int argc, char* argv[]) {
	char c;

	HANDLE readPipe;
	//char* readPipeName = argv[1];
	char readPipeName[50] = "\\\\.\\pipe\\ServerClientPipe";
	wchar_t* readPipeData = new wchar_t[50];
	swprintf(readPipeData, L"%S", readPipeName);

	readPipe = CreateFile(readPipeData, GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (readPipe == INVALID_HANDLE_VALUE)
	{
		cerr << "Connection with the named read pipe failed." << endl;
		cout << "Press any char to finish the client process: ";
		cin >> c;
		return 0;
	}

	HANDLE writePipe;
	//char* writePipeName = argv[2];
	char writePipeName[50] = "\\\\.\\pipe\\ClientServerPipe";
	wchar_t* writePipeData = new wchar_t[50];
	swprintf(writePipeData, L"%S", writePipeName);

	writePipe = CreateFile(writePipeData, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (writePipe == INVALID_HANDLE_VALUE)
	{
		cerr << "Connection with the named write pipe failed." << endl;
		cout << "Press any char to finish the client process: ";
		cin >> c;
		return 0;
	}

	bool doCycle = true;
	while (doCycle) {
		DWORD bytesWrite;
		DWORD bytesRead;

		cout << "1 - read\n2 - modify\n3 - exit\n";
		int answer;
		cin >> answer;

		if (answer == 1) {
			int employeeNumber;
			cout << "Enter employee number:\n";
			cin >> employeeNumber;

			WriteFile(writePipe, &READ, sizeof(READ), &bytesWrite, NULL);
			WriteFile(writePipe, &employeeNumber, sizeof(employeeNumber), &bytesWrite, NULL);

			char serverAnswer;
			ReadFile(readPipe, &serverAnswer, sizeof(serverAnswer), &bytesRead, NULL);

			if (serverAnswer == ORDER_FOUND) {
				Employee employee;
				ReadFile(readPipe, &employee, sizeof(employee), &bytesRead, NULL);
				cout << "\nEmployee's name:\n" << employee.name
					<< "\nEmployee's hours:\n" << employee.hours
					<< endl;
			}
			else {
				cout << "Employee not found.\n";
			}

			char c;
			cout << "Press any char to finish reading: ";
			cin >> c;

			WriteFile(writePipe, &END_OPERATION, sizeof(END_OPERATION), &bytesWrite, NULL);

		}
		else if (answer == 2) {
			int employeeNumber;
			cout << "Enter employee number:\n";
			cin >> employeeNumber;

			WriteFile(writePipe, &READ, sizeof(READ), &bytesWrite, NULL);
			WriteFile(writePipe, &employeeNumber, sizeof(employeeNumber), &bytesWrite, NULL);

			char serverAnswer;
			ReadFile(readPipe, &serverAnswer, sizeof(serverAnswer), &bytesRead, NULL);

			if (serverAnswer == ORDER_FOUND) {
				Employee employee;

				ReadFile(readPipe, &employee, sizeof(employee), &bytesRead, NULL);
				WriteFile(writePipe, &END_OPERATION, sizeof(END_OPERATION), &bytesWrite, NULL);

				cout << "\nEmployee's name:\n" << employee.name
					<< "\nEmployee's hours:\n" << employee.hours
					<< endl << endl;

				cout << "Enter employee's name:\n";
				cin >> employee.name;
				cout << "Enter employee's hours:\n";
				cin >> employee.hours;

				WriteFile(writePipe, &MODIFY, sizeof(MODIFY), &bytesWrite, NULL);
				WriteFile(writePipe, &employee, sizeof(employee), &bytesWrite, NULL);
			}
			else {
				cout << "Employee not found.\n";
			}

			char answer;
			ReadFile(readPipe, &answer, sizeof(answer), &bytesRead, NULL);

			char c;
			cout << "Press any char to finish modifying: ";
			cin >> c;

			WriteFile(writePipe, &END_OPERATION, sizeof(END_OPERATION), &bytesWrite, NULL);

		}
		else {
			WriteFile(writePipe, &EXIT, sizeof(EXIT), &bytesWrite, NULL);
			doCycle = false;
		}
	}

	CloseHandle(writePipe);
	CloseHandle(readPipe);
	return 0;
}
