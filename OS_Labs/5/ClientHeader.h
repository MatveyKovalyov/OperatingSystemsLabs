#pragma once
const char READ = 'r';
const char MODIFY = 'm';
const char EMPLOYEE_NOT_FOUND = 'n';
const char EMPLOYEE_FOUND = 'f';
const char EXIT = 'e';
const char END_OPERATION = 'd';
const char END_MODIFY = 'n';

struct employee {
	int ID;
	char name[10];
	double hours;
};