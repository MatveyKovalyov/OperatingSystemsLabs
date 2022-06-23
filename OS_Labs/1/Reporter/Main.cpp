#include<iostream>
#include<string>
#include<fstream>
#include"D:\OperatingSystemsCpp\Threads\OS_Lab1_Threads\OS_Lab1_Threads\employee.h"
using namespace std;

int main(int argc, char* argv[]) {
	string inputBinFilename = argv[1];
	string outputTextFilename = argv[2];
	double hourlyPayment = atof(argv[3]);

	ifstream readBinFile;
	ofstream writeTextFile;

	readBinFile.open(inputBinFilename, ios::in | ios::binary);

	writeTextFile.open(outputTextFilename);

	writeTextFile << "Report from file: " << inputBinFilename << "\n";

	employee employee;
	while(!readBinFile.eof()) {
		readBinFile.read((char*)&employee, sizeof(employee));
		if (!readBinFile.eof()) {
			writeTextFile << employee.id << " " << employee.name << " " << employee.hours
				<< " " << (employee.hours * hourlyPayment) << "\n";
		}
	}

	readBinFile.close();
	writeTextFile.close();

	return 0;
}