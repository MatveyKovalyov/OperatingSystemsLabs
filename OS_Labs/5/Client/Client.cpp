#include "ClientHeader.h"
#include <windows.h>
#include <conio.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {

	HANDLE writePipe = (HANDLE)atoi(argv[1]);
	HANDLE readPipe = (HANDLE)atoi(argv[2]);

	bool doCycle = true;
	while (doCycle) {
		DWORD bytesWrite;
		DWORD bytesRead;
		cout << "1 -- Read" << endl;
		cout << "2 -- Modify" << endl;
		cout << "3 -- Exit" << endl;
		cout << "Choose option ";
		int answer;
		cin >> answer;
		system("cls");
		if (answer == 1) {
			int employeeID;
			cout << "Enter employee ID: ";
			cin >> employeeID;
			cout << endl;
			WriteFile(writePipe, &READ, sizeof(READ), &bytesWrite, NULL);
			WriteFile(writePipe, &employeeID, sizeof(employeeID), &bytesWrite, NULL);

			char serverAnswer;
			ReadFile(readPipe, &serverAnswer, sizeof(serverAnswer), &bytesRead, NULL);
			if (serverAnswer == EMPLOYEE_FOUND) {
				employee emp;
				ReadFile(readPipe, &emp, sizeof(emp), &bytesRead, NULL);
				cout << "Employee name: " << emp.name << endl;
				cout << "Hours of work: " << emp.hours << endl;
			}
			else {
				cout << "Employee with ID = " << employeeID << " not found." << endl;
			}

			char c;
			cout << "Enter any key to continue: ";
			cin >> c;

			WriteFile(writePipe, &END_OPERATION, sizeof(END_OPERATION), &bytesWrite, NULL);
		}
		else if (answer == 2) {
			int employeeID;
			cout << "Enter employee ID: ";
			cin >> employeeID;
			cout << endl;

			WriteFile(writePipe, &READ, sizeof(READ), &bytesWrite, NULL);
			WriteFile(writePipe, &employeeID, sizeof(employeeID), &bytesWrite, NULL);

			char serverAnswer;
			ReadFile(readPipe, &serverAnswer, sizeof(serverAnswer), &bytesRead, NULL);

			if (serverAnswer == EMPLOYEE_FOUND) {
				employee emp;

				ReadFile(readPipe, &emp, sizeof(emp), &bytesRead, NULL);

				WriteFile(writePipe, &END_OPERATION, sizeof(END_OPERATION), &bytesWrite, NULL);

				cout << "Current employee name: " << emp.name << endl;
				cout << "Current hours of work: " << emp.hours << endl;
				cout << endl;

				cout << "Enter new employee name: ";
				cin >> emp.name;

				cout << "Enter new hours of work: ";
				cin >> emp.hours;

				WriteFile(writePipe, &MODIFY, sizeof(MODIFY), &bytesWrite, NULL);
				WriteFile(writePipe, &emp, sizeof(emp), &bytesWrite, NULL);
			}
			else {
				cout << "Employee with ID = " << employeeID << " not found." << endl;
			}

			char answer;
			ReadFile(readPipe, &answer, sizeof(answer), &bytesRead, NULL);

			char c;
			cout << "Enter any key to continue: ";
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

