#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <conio.h>
#include <Windows.h>

int getSymbol();
int readMessage(HANDLE hFile, char* buffer);
bool writeMessage(HANDLE hFile, char* buffer);
void COM1(HANDLE readEnd, HANDLE writeEnd, HANDLE hFile, HANDLE hExit, HANDLE hWork);
void COM2(HANDLE readEnd, HANDLE writeEnd, HANDLE hFile, HANDLE hExit, HANDLE hWork);