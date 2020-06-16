#define _CRT_SECURE_NO_WARNINGS 
#define USES_CONVERSION
#include "COM.h"
#include <iostream>
#include <locale.h>

void portClient();
int portServer(char* path);

int main(int argc, char* argv[])
{
	switch (argc)
	{
	case 1:
		portServer(argv[0]); // COM1
		break;

	default:
		portClient(); // COM2
		break;
	}
	return 0;
}

std::wstring str_wide(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

int portServer(char* path) 
{
	HANDLE readEnd = CreateEvent(NULL, FALSE, FALSE, (str_wide("readEnd").c_str()));
	HANDLE writeEnd = CreateEvent(NULL, FALSE, FALSE, (str_wide("writeEnd").c_str()));
	HANDLE hExit = CreateEvent(NULL, FALSE, FALSE, (str_wide("exit").c_str()));
	HANDLE hWork = CreateEvent(NULL, FALSE, FALSE, (str_wide("work").c_str()));
	HANDLE hFile = CreateFile(str_wide("COM1").c_str(), GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == NULL) 
	{
		puts("Error");
		exit(GetLastError());
	}

	STARTUPINFO si = {};
	si.cb = sizeof si;
	PROCESS_INFORMATION pi = {};
	if (!CreateProcess(str_wide(path).c_str(), (wchar_t*)std::string("COM2").c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) 
	{
		puts("Error! Process is not created");
		return 1;
	}

	puts("[ THIS IS PORT COM1 ]");
	COM1(readEnd, writeEnd, hFile, hExit, hWork);
	CloseHandle(hFile);
}

void portClient()
{
	HANDLE hFile = CreateFile((str_wide("COM2").c_str()), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == NULL) 
	{
		puts("Error");
		exit(GetLastError());
	}
	HANDLE readEnd = OpenEvent(EVENT_ALL_ACCESS, FALSE, (str_wide("readEnd").c_str())); // флаги доступа и флаги наследования
	HANDLE writeEnd = OpenEvent(EVENT_ALL_ACCESS, FALSE, (str_wide("writeEnd").c_str()));
	HANDLE hExit = OpenEvent(EVENT_ALL_ACCESS, FALSE, (str_wide("exit").c_str()));
	HANDLE hWork = OpenEvent(EVENT_ALL_ACCESS, FALSE, (str_wide("work").c_str()));

	puts("[ THIS IS PORT COM2 ]");
	COM2(readEnd, writeEnd, hFile, hExit, hWork);
	CloseHandle(hFile);
}
