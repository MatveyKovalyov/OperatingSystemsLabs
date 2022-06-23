#include<windows.h>
#include <codecvt>
#include<iostream>
#include<string>
#include<fstream>
using namespace std;



int main() {

	char messagePoolFilename[20];
	int amountOfMessages;
	int amountOfSenderProcesses;

	cout << "Enter filename: "; cin >> messagePoolFilename; cout << endl;
	cout << "Enter amount of messages: "; cin >> amountOfMessages; cout << endl;
	cout << "Enter amount of sender processes: "; cin >> amountOfSenderProcesses; cout << endl;

	ofstream messagePoolFileCreation(messagePoolFilename, ios::binary);
	if (!messagePoolFileCreation.is_open()) {
		cout << "Error during opening file" << endl;
	}
	messagePoolFileCreation.close();


	HANDLE processesCreatedSemaphore = CreateSemaphore(NULL, -amountOfSenderProcesses, 0, L"processesCreated");
	HANDLE messagePollIsEmptySemaphore = CreateSemaphore(NULL, amountOfMessages, amountOfMessages, L"poollIsEmpty");
	HANDLE messagesReadedSemaphore = CreateSemaphore(NULL, amountOfMessages, amountOfMessages, L"messagesReaded");

	HANDLE mutex = CreateMutex(NULL, FALSE, L"mutex");

	if (mutex == NULL) {
		cout << "Open mutex failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}

	STARTUPINFO* si = new STARTUPINFO[amountOfSenderProcesses];
	PROCESS_INFORMATION* pi = new PROCESS_INFORMATION[amountOfSenderProcesses];

	for (int i = 0; i < amountOfSenderProcesses; i++) {
		ZeroMemory(&si[i], sizeof(STARTUPINFO));
		si[i].cb = sizeof(STARTUPINFO);

		if (!CreateProcess(L"D:\\Projects cpp\\Sender\\Debug\\Sender.exe",
			(LPWSTR)wstring_convert<codecvt_utf8<wchar_t>>().from_bytes("Sender.exe " + string(messagePoolFilename)).c_str(),
			NULL,
			NULL,
			FALSE,
			CREATE_NEW_CONSOLE,
			NULL,
			NULL,
			&(si[i]),
			&(pi[i]))) {
			cout << "Error occured while creating process" << endl;
			return -1;
		}
	}

	WaitForSingleObject(processesCreatedSemaphore, INFINITE);

	bool flag = true;
	int choice = 0;

	ifstream messagePoolFile(messagePoolFilename, ios::binary | ios::in);
	if (!messagePoolFile.is_open()) {
		cout << "Error during opening file" << endl;
	}

	char message[20];

	while (flag) {
		cout << "Enter any number to READ message or 0 for exit: "; cin >> choice;
		if (choice == 0) {
			messagePoolFile.close();
			flag = false;
		}
		else {
			if (WaitForSingleObject(messagePollIsEmptySemaphore, 0) != WAIT_OBJECT_0) {
				cout << "No messages yet" << endl;
			}
			else {
				if (WaitForSingleObject(messagesReadedSemaphore,0) != WAIT_OBJECT_0) {
					cout << "All messages read" << endl;
				}
				else {
					WaitForSingleObject(mutex, INFINITE);
					messagePoolFile.read(message, sizeof(message));
					cout << string(message) << endl;
					//ReleaseSemaphore(messagePollIsEmptySemaphore, 1, NULL);
					ReleaseMutex(mutex);
				}
			}
		}
	}

	return 0;
}