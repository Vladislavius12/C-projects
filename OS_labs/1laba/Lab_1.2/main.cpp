#include <iostream>																										//��������� ��� ����������� �����(���������� ��� �������� �������� �������(cin, cout))
#include <Windows.h>																									//���� ������, ������ � �������, ������ � ����� � ������
#include <string>
#include <fstream>
#include "..\\Employee.h"

using namespace std;																									//������� ���� � ������� ������������� ������������

void run_creator(string fileName, int numOfRec) { //����� ���������
	const char* appName = "..\\Release\\Creator.exe"; //��������� �� char Creator
	string name_with_params = fileName + " " + to_string(numOfRec);

	STARTUPINFO si;// ���������
	PROCESS_INFORMATION piApp;//����������� �reateProcess ����������� � ��������� ���� (���������� ������� ���������� ��������, ���������� ���������� ������ ������� ���������� ��������, ��� �������� ��� �������������)

	ZeroMemory(&si, sizeof(STARTUPINFO));//���������� ����� ������ ������ ����� ������������ ������ memset(������ �������)
	si.cb = sizeof(STARTUPINFO);// �������� �������

	if (!CreateProcess(appName, (LPSTR)name_with_params.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp)) {
		cout << "Error: process Creator hasn't been created!\n";
	}

	WaitForSingleObject(piApp.hProcess, INFINITE);

	CloseHandle(piApp.hThread); //���
	CloseHandle(piApp.hProcess); //����
}

void run_reporter(string fileName, string reportFileName, int moneyPerHour) {
	const char* appName = "..\\Release\\Reporter.exe";
	string name_with_params = fileName + " " + reportFileName + " " + to_string(moneyPerHour);

	STARTUPINFO si;
	PROCESS_INFORMATION piApp;

	ZeroMemory(&si, sizeof(STARTUPINFO)); //���������� ����� ������ ������ ����� ������������ ������ memset(������ �������)
	si.cb = sizeof(STARTUPINFO);

	if (!CreateProcess(appName, (LPSTR)name_with_params.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp)) {
		cout << "Error: process Reporter hasn't been created!\n";
	}

	WaitForSingleObject(piApp.hProcess, INFINITE);

	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);
}

int main()
{
	setlocale(LC_ALL, "Russian");
	cout << "Input name of binary file: ";
	string fileName;
	cin >> fileName;

	cout << "Enter the number of records: ";
	int numOfRec;
	cin >> numOfRec;

	run_creator(fileName, numOfRec);

	ifstream binFile(fileName, ios::binary);
	employee record;
	while (binFile.read((char*)&record, sizeof(employee))) {
		cout << record.num << "\t" << record.name << "\t" << record.hours << endl;
	}
	binFile.close();

	string reportFileName;
	cout << "Input name of report file: ";
	cin >> reportFileName;

	int moneyPerHour;
	cout << "Input money per hour: ";
	cin >> moneyPerHour;


	run_reporter(fileName, reportFileName, moneyPerHour);

	ifstream reportFile(reportFileName);
	string recordFromReport;
	while(getline(reportFile, recordFromReport)){
		cout << recordFromReport << endl;
	}
	reportFile.close();
	system("pause");
	return 0;
}