#include<iostream>
#include <codecvt>
#include<windows.h>
#include<fstream>
using namespace std;


int main(int argc, char* argv[]) {

	HANDLE processesCreatedSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"processesCreated");
	HANDLE messagePoolIsEmptySemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"poolIsEmpty");
	HANDLE mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"mutex");

	if (mutex == NULL) {
		cout << "Open mutex failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}

	ReleaseSemaphore(processesCreatedSemaphore, 1, NULL);


	bool flag = true;
	int choice = 0;
	char message[20];


	while (flag){
		cout << "Enter any number to SEND message or 0 for exit: "; cin >> choice;
		if (choice == 0) {
			flag = false;
		}else{
			if (WaitForSingleObject(messagePoolIsEmptySemaphore, 0) == WAIT_OBJECT_0) {
				cout << "Reached limit of messages" << endl;
			}
			else {
				WaitForSingleObject(messagePoolIsEmptySemaphore, INFINITE);
				WaitForSingleObject(mutex, INFINITE);

				ofstream messagePoolOut(argv[1], ios::binary | ios::app | ios::out);

				cout << "Enter your message(20 symbols): ";
				cin >> message;
				messagePoolOut.write(message, sizeof(message));
				messagePoolOut.close();
				ReleaseMutex(mutex);
			}
		}
	}

	return 0;
}