#define _CRT_SECURE_NO_WARNINGS 
#include "COM.h"

int readMessage(HANDLE hFile, char* buffer)
{
	int size;
	DWORD numberOfBytesRead;
	ReadFile(hFile, &size, sizeof(int) * 1, &numberOfBytesRead, NULL);	     // размер считывания
	ReadFile(hFile, buffer, sizeof(char) * size, &numberOfBytesRead, NULL);	 // информация считывания
	return size;
}

bool writeMessage(HANDLE hFile, char* buffer)
{
	int i = 0;
	char symbol;
	DWORD numberOfBytesWrite;

	while (true)
	{
		scanf("%c", &symbol);
		if (symbol == '\n')
		{
			buffer[i] = '\0';
			WriteFile(hFile, &i, sizeof(int) * 1, &numberOfBytesWrite, NULL);	    // размер записи
			WriteFile(hFile, buffer, sizeof(char) * i, &numberOfBytesWrite, NULL);	// информация записи
			if (!strcmp(buffer, "Exit\0")) 
				return false;
			return true;
		}
		else 
		{
			buffer[i] = symbol;
		}
		i++;
	}
}

int getSymbol() { return _kbhit() ? _getch() : -1; }

void COM1(HANDLE readEnd, HANDLE writeEnd, HANDLE hFile, HANDLE hExit, HANDLE hWork)
{
	int size;
	char buffer[100];
	while (true)
	{
		if (WaitForSingleObject(hWork, 1) != WAIT_TIMEOUT)	// ожидание сообщения от другого порта
		{
			WaitForSingleObject(writeEnd, INFINITE);
			size = readMessage(hFile, buffer);
			printf("NEW MESSAGE FROM COM2: ");
			for (int i = 0; i < size; i++) 
				printf("%c", buffer[i]);
			puts(" ");
			SetEvent(readEnd);
		}

		if (getSymbol() == 13) // enter
		{
			SetEvent(hWork);
			printf("SEND MESSAGE: ");
			if (!writeMessage(hFile, buffer))
			{
				SetEvent(hExit);
				SetEvent(writeEnd);
				return;
			}
			else
			{
				SetEvent(writeEnd);
				WaitForSingleObject(readEnd, INFINITE);
				puts("MESSAGE IS SENT");
			}
		}
	}
}

void COM2(HANDLE readEnd, HANDLE writeEnd, HANDLE hFile, HANDLE hExit, HANDLE hWork)
{
	int size;
	char buffer[100];

	while (WaitForSingleObject(hExit, 1) == WAIT_TIMEOUT)
	{
		if (WaitForSingleObject(hWork, 1) != WAIT_TIMEOUT)	// ожидание сообщения от другого порта
		{
			WaitForSingleObject(writeEnd, INFINITE);
			if (WaitForSingleObject(hExit, 1) != WAIT_TIMEOUT) return;
			size = readMessage(hFile, buffer);
			printf("NEW MESSAGE FROM COM1: ");
			for (int i = 0; i < size; i++) 
				printf("%c", buffer[i]);
			puts(" ");
			SetEvent(readEnd);
		}

		if (getSymbol() == 13) // enter
		{
			SetEvent(hWork);
			printf("SEND MESSAGE: ");
			writeMessage(hFile, buffer);
			SetEvent(writeEnd);
			WaitForSingleObject(readEnd, INFINITE);
			puts("MESSAGE IS SENT");
		}
	}
}


