#include"ServerHeader.h"
#include <iostream>
#include <fstream>
#include <Windows.h>

#pragma warning(disable : 4996)

using namespace std;

char filename[20];

volatile int readerCount = 0;
CRITICAL_SECTION cs;
HANDLE semaphore;

employee* findEmployee(int empID) {
	ifstream in(filename, ios::binary);
	while (in.peek() != EOF){
		employee* emp = new employee;
		in.read((char*)emp, sizeof(employee));
		if (emp->ID == empID) {
			in.close();
			return emp;
		}
	}
	in.close();
	return nullptr;
}

void modify(employee emp) {
	fstream f(filename, ios::binary | ios::in | ios::out);
	int pos = 0;
	int orderSize = sizeof(employee);
	while (f.peek() != EOF)
	{
		employee tempEmp;
		f.read((char*)&tempEmp, sizeof(employee));
		if (emp.ID == tempEmp.ID) {
			f.seekp(pos * orderSize, ios::beg);
			f.write((char*)&emp, sizeof(employee));
			f.close();
			return;
		}
		else {
			pos++;
		}
	}
}

DWORD WINAPI client(LPVOID data) {
	HANDLE writePipe;
	HANDLE readPipe;
	HANDLE clientReadPipe;
	HANDLE clientWritePipe;
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	if (!CreatePipe(&readPipe, &clientWritePipe, &sa, 0)) {
		cout << "Create pipe failed." << endl;
		system("pause");
		return 0;
	}

	if (!CreatePipe(&clientReadPipe, &writePipe, &sa, 0)) {
		cout << "Create pipe failed." << endl;
		system("pause");
		return 0;
	}

	char comLine[333];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wsprintf(comLine, "Lab_5_client.exe %d %d", (int)clientWritePipe, (int)clientReadPipe);
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	if (!CreateProcess(NULL, comLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		cout << "The new process is not created." << endl;
		return 0;
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

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

			int employeeID;
			ReadFile(readPipe, &employeeID, sizeof(int), &bytesRead, NULL);
			employee* emp = findEmployee(employeeID);

			if (emp != nullptr) {
				WriteFile(writePipe, &EMPLOYEE_FOUND, sizeof(EMPLOYEE_FOUND), &bytesWrite, NULL);
				WriteFile(writePipe, emp, sizeof(employee), &bytesWrite, NULL);
			}
			else {
				WriteFile(writePipe, &EMPLOYEE_NOT_FOUND, sizeof(EMPLOYEE_NOT_FOUND), &bytesWrite, NULL);
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

			employee emp;
			ReadFile(readPipe, &emp, sizeof(employee), &bytesRead, NULL);
			modify(emp);
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

void createBinaryFile() {
	ofstream out(filename, ios::binary);

	cout << "Enter amount of notes in file: ";
	int num;
	cin >> num;

	for (int i = 0; i < num; i++){
		system("cls");
		employee emp;
		cout << "Enter employee ID: ";
		cin >> emp.ID;

		cout << "Enter employee name: ";
		cin >> emp.name;

		cout << "Enter hours of work: ";
		cin >> emp.hours;

		out.write((char*)&emp, sizeof(struct employee));
	}

	out.close();
}

void consoleDataOut() {
	ifstream in(filename, ios::binary);
	system("cls");
	while (in.peek() != EOF) {
		employee emp;
		in.read((char*)&emp, sizeof(employee));
		cout << "Employee ID: " << emp.ID << endl;
		cout << "Employee name: " << emp.name << endl;
		cout << "Hours: " << emp.hours << endl << endl;
	}
	in.close();
}

void main() {
	InitializeCriticalSection(&cs);
	semaphore = CreateSemaphore(NULL, 1, 1, NULL);

	cout << "Enter filename: ";
	cin >> filename;

	createBinaryFile();
	consoleDataOut();

	cout << "Enter amount of clients: ";
	int clientCount;
	cin >> clientCount;
	system("cls");
	HANDLE* handles = new HANDLE[clientCount];
	DWORD* ID = new DWORD[clientCount];
	for (int i = 0; i < clientCount; i++) {
		handles[i] = CreateThread(NULL, NULL, client, (void*)i, 0, &ID[i]);
	}

	WaitForMultipleObjects(clientCount, handles, TRUE, INFINITE);
	system("cls");
	consoleDataOut();

	char c;
	cout << "Enter any key to finish: ";
	cin >> c;
}