#include<iostream>
#include<fstream>
#include<String>
#include<windows.h>
#include <codecvt>
#include"employee.h"
using namespace std;


int main(int argc, char* argv[]) {
	string binFilename;
	int amountOfRecords;
    string reportFilename;
    double perHourPayment;

	cout << "Enter binFilename: ";
	cin >> binFilename; cout << endl;
	cout << "Enter amount of records ";
	cin >> amountOfRecords; cout << endl;
	if (amountOfRecords <= 0) {
		cout << "Minimum amount of records is 1";
		return -1;
	}

    STARTUPINFO si;
    PROCESS_INFORMATION piApp;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    ZeroMemory(&piApp, sizeof(piApp));

    if (CreateProcess(L"D:\\OperatingSystemsCpp\\Threads\\Creator\\Debug\\Creator.exe",
        (LPWSTR)(wstring_convert<codecvt_utf8<wchar_t>>().from_bytes("Creator.exe " + binFilename + " " + to_string(amountOfRecords)).c_str()),
        NULL,
        NULL,
        FALSE,
        NULL,
        NULL,
        NULL,
        &si,
        &piApp)) {
        WaitForSingleObject(piApp.hProcess, INFINITE);
        CloseHandle(piApp.hThread);
        CloseHandle(piApp.hProcess);
    }

    fstream binFile(binFilename, ios::binary | ios::in);
    employee emp;
    while (!binFile.eof()) {
        binFile.read((char*)&emp, sizeof(emp));
        if (!binFile.eof())
            cout << emp.id << " " << emp.name << " " << emp.hours << '\n';
    }
    cout << endl;

    cout << "Enter reportFilename: ";
    cin >> reportFilename; cout << endl;
    cout << "Enter perHour payment ";
    cin >> perHourPayment; cout << endl;
    if (perHourPayment <= 0) {
        cout << "PerHourPayment should be > 0 ";
        return -1;
    }

    if (CreateProcess(L"D:\\OperatingSystemsCpp\\Threads\\Reporter\\Debug\\Reporter.exe",
        (LPWSTR)(wstring_convert<codecvt_utf8<wchar_t>>().from_bytes("Reporter.exe " + binFilename + " " + reportFilename + " " + to_string(perHourPayment)).c_str()),
        NULL,
        NULL,
        FALSE,
        NULL,
        NULL,
        NULL,
        &si,
        &piApp)) {
        WaitForSingleObject(piApp.hProcess, INFINITE);
        CloseHandle(piApp.hThread);
        CloseHandle(piApp.hProcess);
    }

    binFile.close();

    ifstream reportReader(reportFilename);
    string str;
    while (getline(reportReader,str)) {
        cout << str << endl;
    }
    reportReader.close();

    return 0;


}