#include <windows.h>
#include <iostream>

using namespace std;

int* Arr;
int ArraySize;
HANDLE markerStart;
HANDLE** waiting;
HANDLE* hTupik;

void Massinit() { //��������� ������
	cout << "������� ���-�� ��������� �������: " << endl;
	cin >> ArraySize;
	cout << endl;
	Arr = new int[ArraySize];
	for (int i = 0; i < ArraySize; i++)
		Arr[i] = 0;
}

void Massout() { //����� �������
	cout << "�������� ������: " << endl;
	for (int j = 0; j < ArraySize; j++)
		cout << Arr[j] << " ";
}

int stop(int stoppedMarkers, int markerNum, HANDLE* hThread) {
	int markerToStop;
	cout << endl << "������� ����� ��� ���������: " << endl;
	cin >> markerToStop;

	if (markerToStop > 0) {
		SetEvent(waiting[markerToStop - 1][0]); // ������ � ���������� ������
		stoppedMarkers++;
		WaitForSingleObject(hThread[markerToStop - 1], INFINITE);
		SetEvent(hTupik[markerToStop - 1]);
		Massout();
		cout << endl;
		for (int j = 0; j < markerNum; j++)
			SetEvent(waiting[j][1]);
		Sleep(100);
	}
	else {
		for (int j = 0; j < markerNum; j++)
			SetEvent(waiting[j][1]);
	}
	return stoppedMarkers;
}


DWORD WINAPI marker(LPVOID parameters) {
	int markerNum = (int)parameters;
	int markedElements = 0;

	if (WaitForSingleObject(markerStart, INFINITE) == WAIT_OBJECT_0) {
		srand(markerNum);
		while (true) {
			int random = rand();

			random = random % ArraySize;

			if (Arr[random] == 0) {
				Sleep(5);
				Arr[random] = markerNum;
				markedElements++;
				Sleep(5);
			}
			else {
				cout << endl << "Thread: " << markerNum << endl;
				cout << "Marked elements: " << markedElements << endl;
				cout << "Unmarkable index: " << ArraySize - markedElements << endl << endl;

				SetEvent(hTupik[markerNum - 1]);

				int flag = WaitForMultipleObjects(2, waiting[markerNum - 1], FALSE, INFINITE); //���������������� �����, � ������� ������� �� ��� ���� ���� ��� ��� ���������� �� ������� (������� �� ���������� ����������) �� �������� � ���������� ���������
				if (flag == 0) {
					for (int i = 0; i < ArraySize; i++) {
						if (Arr[i] == markerNum) {
							Arr[i] = 0;
						}
					}
					break;
				}
				else {
					ResetEvent(hTupik[markerNum - 1]);
					continue;
				}
			}
		}
	}

	return 0;
}

int main() {

	setlocale(LC_ALL, "rus");
	Massinit();

	int markerNum;
	cout << "Amount of threads: " << endl;
	cin >> markerNum; //���-�� ��������

	HANDLE* hThread = new HANDLE[markerNum];
	DWORD* IDThread = new DWORD[markerNum];
	waiting = new HANDLE * [markerNum];

	for (int i = 0; i < markerNum; i++)
		waiting[i] = new HANDLE[2];

	hTupik = new HANDLE[markerNum];
	markerStart = CreateEvent(nullptr, TRUE, FALSE, nullptr); //�������� ������� (������� ������,��� ������ TRUE - ������, ��������� ��������� TRUE - ����������, ��� �������)
	for (int i = 0; i < markerNum; i++) {
		hThread[i] = CreateThread(nullptr, 0, marker, (void*)(i + 1), 0, &IDThread[i]);
		hTupik[i] = CreateEvent(nullptr, TRUE, FALSE, nullptr);
		waiting[i][0] = CreateEvent(nullptr, TRUE, FALSE, nullptr);
		waiting[i][1] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	}
	SetEvent(markerStart);
	int stoppedMarkers = 0;
	while (stoppedMarkers != markerNum) {
		WaitForMultipleObjects(markerNum, hTupik, TRUE, INFINITE);
		Massout();
		stoppedMarkers = stop(stoppedMarkers, markerNum, hThread);
	}

	return 0;
}