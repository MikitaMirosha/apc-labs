#include <io.h>
#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <iostream.h>

int i;
int flag;
int temp;
char* arr;
int current = 0x60;  // запись конфигурационного байта контроллера
int previous = 0x60;
							  
void interrupt far newInt9(...);				 	// обработка прерывания
void interrupt far (*oldInt9)(...);				    // указатель на обработчик прерывания
int convertScanCodeToInt(char* arr);			    // конвертация скан-кода в десятичное число

void main() 
{
	unsigned far *pointer;	
	clrscr();
	disable();                      				
	oldInt9 = getvect(0x09);        				// сохранение указателя на старый обработчик
	setvect(0x09, newInt9);							// замена указателя на новый  
	enable();                       				

	FP_SEG(pointer) = _psp;							// получить сегмент
	FP_OFF(pointer) = 0x2c;							// получить смещение сегмента данных с помощью переменных окружения
	_dos_freemem(*pointer);							// освободить память
	_dos_keep(0, (_DS - _CS) + (_SP / 16) + 1);		// оставить программу резидентной 
	// 1й параметр - код завершения, 2й параметр - объем памяти,
	// которые должны быть зарезервированы для программы после завершения 
	return;
}

int convertScanCodeToInt(char* arr)
{
	int number = 0;
    for (i = 0; arr[i] !='\0'; ++i)
	{
		if(arr[i] < 0x01 && arr[i] > 0x0B) // если цифра не введена
			return -1;

		if(arr[i] == 0x0B) // если 0 (0x0B)
	 		number = number * 10;
		else
	   		number = 10 * number + (arr[i] - 1);
	}
	if(i == 0)
		return -1;

	return number;
}

// обработка прерывания
void interrupt far newInt9(...) 
{
	unsigned char value = 0;
	value = inp(0x60);	        // получение значения из порта 60h

	if(value == 0x1C && flag)   // 0x1C - enter
	{		
		clrscr();
		if(arr[0] == 0x2D && arr[1] == 0x2D)   // if [xx], 0x2D - x 
		{ 
			if(arr[2] == 0x20)                 // if [xxd], 0x20 - d
			{				
				current = convertScanCodeToInt(arr + 3);
				if(current > -1 && current < 4) // [0-3]
				{
					previous = previous % 32;  // замена значений (меняются биты 5-6)
					current = current * 32;
					current = current + previous;
					previous = current;
				}
				else
					current = -1;
			}
			else // if [xx]
			{							
				current = convertScanCodeToInt(arr + 2);
				if(current >= 0 && current < 32) // [0-31]
				{
					previous = previous / 32;    // замена значений (меняются биты 0-4)
					previous = previous * 32;
					current = current + previous;
					previous = current;
				}	
				else
					current = -1;
			}
		}
		else
			current = -1;

		for(i = 0; i < temp; i++) // обнуление
			arr[i] = '\0';
		temp = 0;
		flag = 0;

		if(current >= 0) // если введена команда
		{
			// 64h - чтение регистра состояния и запись в регистр комманд контроллера
			while ((inp(0x64) & 0x02) != 0x00);    // 0x02 - 0010 (бит 1: есть данные во входном буфере)
			outp(0x60, 0xF3);					   // автоповтор и задержка
			while ((inp(0x64) & 0x02) != 0x00);
			outp(0x60, current);                   // вывод current value
		}
	}

	if(flag && value < 0x80) // если клавиши отпущены (0x80)
	{
		if(value == 0x0E)	// if backspace (0x0E)
		{
			arr[temp] = '\0';  // temp обнуляется и становится меньше на 1 
			if (temp > 0)
				temp--;
		}
		else
		{
			arr[temp] = value; // в temp продолжает храниться насущное значение
			temp++;
			arr[temp] = '\0'; 
		}
	}

	if(value == 0x0C)   // если символ '-', то установка флага
		flag = 1;

	oldInt9(); // помечаем как старое совершившееся прерывание
}
