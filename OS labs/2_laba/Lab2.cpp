#include <iostream>
#include <Windows.h>

using namespace std;

DWORD WINAPI min_max(LPVOID array) { //(ук)
	int* arr = (int*)array;
	int size = arr[0];
	int min = arr[1], max = arr[1];
	for (int i = 2; i <= size; i++) {
		if (arr[i] < min) {
			min = arr[i];
		}
		Sleep(7);
		if (arr[i] > max) {
			max = arr[i];
		}
		Sleep(1000);
	}
	cout << min << "   " << max << endl << endl;
	arr[size + 1] = min;
	arr[size + 2] = max;
	return 0;
}

DWORD WINAPI average(LPVOID array) {
	int* arr = (int*)array;
	int size = arr[0];
	int sum = 0;
	for (int i = 1; i <= size; i++) {
		sum += arr[i];
		Sleep(200);
	}
	arr[size + 3] = sum / size;
	return 0;
}

int main() {
	int* array;
	int n;
	cin >> n;
	array = new int[n + 4];
	array[0] = n;
	for (int i = 1; i <= n; i++) {
		cin >> array[i];
	}
	HANDLE hMin_max, hAverage;
	DWORD IDMin_max, IDAverage;
	hMin_max = CreateThread(NULL, 0, min_max, (void*)array, NULL, &IDMin_max);
	hAverage = CreateThread(NULL, 0, average, (void*)array, NULL, &IDAverage);
	WaitForSingleObject(hMin_max, INFINITE); 
	WaitForSingleObject(hAverage, INFINITE);
	CloseHandle(hMin_max); 
	CloseHandle(hAverage);
	for (int i = 1; i <= n; i++) {
		if (array[i] == array[n + 1] || array[i] == array[n + 2]) {
			array[i] = array[n + 3];
		}
	}
	for (int i = 1; i <= n; i++) {
		cout << array[i] << " ";
	}
	return 0;
}