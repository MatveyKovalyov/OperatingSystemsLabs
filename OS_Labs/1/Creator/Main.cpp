#include<iostream>
#include"D:\OperatingSystemsCpp\Threads\OS_Lab1_Threads\OS_Lab1_Threads\employee.h"
#include<string>
#include<fstream>
#include<windows.h>
using namespace std;

employee getEmployeeFromConsole() {
	employee employee{};
	cout << "Enter id: ";
	cin >> employee.id; cout << endl;
	cout << "Enter name: ";
	cin >> employee.name; cout << endl;
	cout << "Enter working hours: ";
	cin >> employee.hours; cout << endl;
	return employee;
}

int main(int argc, char* argv[]) {




	string filename = argv[1];
	int amountOfRecords = atoi(argv[2]);
	
	fstream binFile;
	binFile.open(filename, ios::out | ios::binary);
	if (binFile.is_open()) {
		for (int i = 0; i < amountOfRecords; i++) {
			employee employee = getEmployeeFromConsole();
			binFile.write((char*)&employee, sizeof(employee));
		}
	}else {
		cout << "Error occured: " << filename << " not opened";
		return -1;
	}
	binFile.close();
	return 0;
}