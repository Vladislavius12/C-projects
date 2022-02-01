#include <iostream>
#include <fstream>
#include <Windows.h>

#pragma warning(disable : 4996)

using namespace std;

//////////////////////////////////////////////////////////
//Глобальные переменные))
char READ = 'r';
char MODIFY = 'm';
char ORDER_NOT_FOUND = 'n';
char ORDER_FOUND = 'f';
char EXIT = 'e';
char END_OPERATION = 'd';
char END_MODIFY = 'n';
char filename[20];

volatile int readerCount = 0;
volatile int clientsCount;
CRITICAL_SECTION cs;
HANDLE semaphore;
//////////////////////////////////////////////////////////

struct Employee {
	int num;
	char name[10];
	int hours;
};

Employee* findEmployee(int employeeNumber) {
	ifstream in(filename, ios::binary);
	while (in.peek() != EOF) //проверяет, достигнут ли конец потока входного файла
	{
		Employee* employee = new Employee;
		in.read((char*)employee, sizeof(Employee)); //читаем 9 байт
		if (employee->num == employeeNumber) {
			in.close();
			return employee;
		}
	}
	in.close();

	return nullptr;
}

void modify(Employee employee) {
	fstream f(filename, ios::binary | ios::in | ios::out);
	int pos = 0;

	while (f.peek() != EOF)
	{
		Employee tempEmployee;
		f.read((char*)&tempEmployee, sizeof(Employee));
		if (employee.num == tempEmployee.num) {
			f.seekp(pos * sizeof(Employee), ios::beg); //Устанавливает относительную позицию записи
			f.write((char*)&employee, sizeof(Employee));
			f.close();
			return;
		}
		else {
			pos++;
		}
	}
}

DWORD WINAPI client(LPVOID data)
{
	int pipeNumber = (int)data; //Количество каналов
	HANDLE writePipe;
	HANDLE readPipe;
	char c;
	SECURITY_ATTRIBUTES sa; // атрибуты безопасности для объекта. 
	SECURITY_DESCRIPTOR sd; // дескриптор безопасности безопасности объекта.
	char lpszInMessage[80];
	DWORD dwBytesRead; //для количества прочитаных байтов
	DWORD dwBytesWrite; //для количества записанных байтов
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE; //дескриптор канала не наследуемый
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION); //Инициализация дискриптора безопаснгости
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE); // Разрешаем доступ к именованному каналу всем пользователям
	sa.lpSecurityDescriptor = &sd;

	char writePipeName[50] = "\\\\.\\pipe\\ServerClientPipe"; //создаём именнованный канал
	char numString[2];
	sprintf(numString, "%d", pipeNumber);
	strcat(writePipeName, numString);
	wchar_t* writePipeData = new wchar_t[50];
	swprintf(writePipeData, L"%S", writePipeName);

	writePipe = CreateNamedPipe(writePipeData, PIPE_ACCESS_OUTBOUND, PIPE_TYPE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 0, 0, INFINITE, &sa);
	if (writePipe == INVALID_HANDLE_VALUE)
	{
		cerr << "Creation of the named write pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		cout << "Press any char to finish server: ";
		cin >> c;
		return 0;
	}

	char readPipeName[50] = "\\\\.\\pipe\\ClientServerPipe";
	strcat(readPipeName, numString);
	wchar_t* readPipeData = new wchar_t[50];
	swprintf(readPipeData, L"%S", readPipeName);

	readPipe = CreateNamedPipe(readPipeData, PIPE_ACCESS_INBOUND, PIPE_TYPE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 0, 0, INFINITE, &sa);
	if (readPipe == INVALID_HANDLE_VALUE)
	{
		cerr << "Creation of the named read pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		cout << "Press any char to finish server: ";
		cin >> c;
		return 0;
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	char tempData[150] = "C\\Users\\vladi\\Documents\\OS_Lab5_Client ";
	strcat(tempData, writePipeName);
	strcat(tempData, " ");
	strcat(tempData, readPipeName);
	strcat(tempData, " ");
	wchar_t* processData = new wchar_t[150];
	swprintf(processData, L"%S", tempData);

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	if (!CreateProcess(NULL, processData, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		cout << "New process was not created.\n";
		return 0;
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	cout << "The server is waiting for connection with a client." << endl;
	if (!ConnectNamedPipe(writePipe, (LPOVERLAPPED)NULL))
	{
		cerr << "The write pipe connection failed." << endl << "The last error code: " << GetLastError() << endl;
		CloseHandle(writePipe);
		cout << "Press any char to finish the server: ";
		cin >> c;
		return 0;
	}
	cout << "Connected\n";

	bool doCycle = true;
	while (doCycle) {
		DWORD bytesRead;
		DWORD bytesWrite;

		char clientWant;
		ReadFile(readPipe, &clientWant, sizeof(clientWant), &bytesRead, NULL);

		if (clientWant == READ) {
			EnterCriticalSection(&cs);
			++readerCount;
			if (readerCount == 1) {
				WaitForSingleObject(semaphore, INFINITE);
			}
			LeaveCriticalSection(&cs);

			int employeeNumber;
			ReadFile(readPipe, &employeeNumber, sizeof(int), &bytesRead, NULL);
			Employee* employee = findEmployee(employeeNumber);

			if (employee != nullptr) {
				WriteFile(writePipe, &ORDER_FOUND, sizeof(ORDER_FOUND), &bytesWrite, NULL);
				WriteFile(writePipe, employee, sizeof(Employee), &bytesWrite, NULL);
			}
			else {
				WriteFile(writePipe, &ORDER_NOT_FOUND, sizeof(ORDER_NOT_FOUND), &bytesWrite, NULL);
			}

			char end;
			ReadFile(readPipe, &end, sizeof(end), &bytesRead, NULL);

			EnterCriticalSection(&cs);
			--readerCount;
			if (readerCount == 0) {
				ReleaseSemaphore(semaphore, 1, NULL);
			}
			LeaveCriticalSection(&cs);
		}
		else if (clientWant == MODIFY) {
			WaitForSingleObject(semaphore, INFINITE);

			Employee employee;
			ReadFile(readPipe, &employee, sizeof(Employee), &bytesRead, NULL);
			modify(employee);
			WriteFile(writePipe, &END_MODIFY, sizeof(END_MODIFY), &bytesWrite, NULL);

			char end;
			ReadFile(readPipe, &end, sizeof(end), &bytesRead, NULL);

			ReleaseSemaphore(semaphore, 1, NULL);
		}
		else {
			doCycle = false;
		}
	}
	return 0;
}

void createBinaryFile() { //Создание бинарника
	ofstream out(filename, ios::binary);

	cout << endl;
	cout << "How many employee's records do you want to write?\n";
	int count;
	cin >> count;
	cout << endl;

	for (int i = 0; i < count; i++)
	{
		Employee employee;
		cout << "Enter employee's number:\n";
		cin >> employee.num;

		cout << "Enter employee's name:\n";
		cin >> employee.name;

		cout << "Enter employee's hours:\n";
		cin >> employee.hours;

		cout << endl;

		out.write((char*)&employee, sizeof(Employee));
	}
	out.close();
}

void checkDataInBinaryFile() //Считывание с бинарника
{
	ifstream in(filename, ios::binary);
	system("cls");
	cout << "Employees:\n";
	while (in.peek() != EOF)
	{
		Employee employee;
		in.read((char*)&employee, sizeof(Employee));
		cout << "Employee's number:\n" << employee.num
			<< "\nEmployee's name:\n" << employee.name
			<< "\nEmployee's hours:\n" << employee.hours
			<< endl << endl;
	}
	in.close();
}

int main() {
	InitializeCriticalSection(&cs);
	semaphore = CreateSemaphore(NULL, 1, 1, NULL);

	cout << "Enter file name:\n";
	cin >> filename;

	createBinaryFile();
	checkDataInBinaryFile();

	int clientCount;
	cout << "Enter client proccesses number:\n";
	cin >> clientCount;

	clientsCount = clientCount;

	HANDLE* handles = new HANDLE[clientsCount];
	DWORD* ID = new DWORD[clientsCount];
	for (int i = 0; i < clientsCount; i++)
	{
		handles[i] = CreateThread(NULL, NULL, client, (void*)i, 0, &ID[i]);
	}

	WaitForMultipleObjects(clientCount, handles, TRUE, INFINITE);
	checkDataInBinaryFile();

	system("pause");
	return 0;
}