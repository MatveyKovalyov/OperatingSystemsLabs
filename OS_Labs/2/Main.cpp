#include <windows.h>
#include <iostream>
using namespace std;

int n;
int maxIndex;
int minIndex;
int averageVal;
int* arr;

void WINAPI min_max() {
    minIndex = 0;
    maxIndex = 0;
    for (int i = 1; i < n; i++) {
        if (arr[i] > arr[maxIndex]) {
            maxIndex = i;
        }
        Sleep(7);
        if (arr[i] < arr[minIndex]) {
            minIndex = i;
        }
        Sleep(7);
    }
    cout << "min element = " << arr[minIndex] << endl;
    cout << "max element = " << arr[maxIndex] << endl;
}

void WINAPI average() {
    int arrSum = 0;
    for (int i = 0; i < n; i++) {
        arrSum += arr[i];
        Sleep(12);
    }
    averageVal = arrSum / n;

    cout << "average value = " << averageVal << endl;
    cout << endl;
}

int main() {
    HANDLE handleMin_max;
    DWORD IDmin_max;
    HANDLE handleAverage;
    DWORD IDaverage;
    cout << "n = ";
    cin >> n;
    arr = new int[n];
    cout << endl;

    int inputVal;
    for (int i = 0; i < n; i++) {
        cout << i << "->";
        cin >> inputVal;
        arr[i] = inputVal;
    }
    cout << endl;

    handleMin_max = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)min_max, NULL, 0, &IDmin_max);
    if (handleMin_max == NULL) {
        cout << "erroe in creating thread";
        return 0;
    }
    handleAverage = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)average, NULL, 0, &IDaverage);
    if (handleAverage == NULL) {
        cout << "erroe in creating thread";
        return 0;
    }

    WaitForSingleObject(handleMin_max, INFINITE);
    WaitForSingleObject(handleAverage, INFINITE);
    CloseHandle(handleMin_max);
    CloseHandle(handleAverage);

    arr[minIndex] = averageVal;
    arr[maxIndex] = averageVal;

    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

    return 0;
}