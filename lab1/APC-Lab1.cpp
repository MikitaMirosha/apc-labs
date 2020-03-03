// 13. F(x) = sin(x) * cos(x2)
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <iostream> 

int main()
{
	printf("Welcome!\n");
	printf("Press 1 to compute\n");
	printf("Press 0 to exit\n");
	clock_t begin_c, end_c, begin_asm, end_asm;
	while (1)
	{
		switch (_getch())
		{
		case '1':
		{
			double a, b, f, x, step, check;
			do
			{
				printf("Enter a:\n");
				rewind(stdin);
				check = scanf_s("%lf", &a);
			} while (check < 1);

			do
			{
				printf("Enter b:\n");
				rewind(stdin);
				check = scanf_s("%lf", &b);
			} while (check < 1);

			do
			{
				printf("Enter step:\n");
				rewind(stdin);
				check = scanf_s("%lf", &step);
			} while (check < 1);

			x = a;
			f = 0;

			begin_c = clock();
			while (x <= b)
			{
				f += sin(x) * cos(pow(x, 2));
				x += step;
			};
			end_c = clock();

			printf("\nComputing using C:\n");
			printf("value: %lf\n", f);
			printf("time: %f\n", ((float)(end_c - begin_c) / CLOCKS_PER_SEC));
			
			x = a;
			f = 0;

			begin_asm = clock();
			_asm
			{
				FINIT	// инициализация сопроцессора

				FLD	b	// загрузка в ST(0) верхней границы интервала
				FLD	x	// загрузка значения функции
				LOOP_START :	// метка начала цикла
				FCOM	// проверка границы интервала, сравнивает содержимое ST(0) с вещ операндом и ставит флаг в регистр состояния FPU
					FSTSW AX  // запись текущего значения в регистр AX
					AND AH, 01000101B
					JZ LOOP_END // переход цикла в конец

					FLDZ	// загрузка нуля
					FADD x	// прибавка x 
					FSIN	// нахождение синуса

					FLD	x	// загрузка х
					FMUL x  // умножение х на х
					FCOS	// нахождение косинуса

					FMUL	// умножение всего
					FADD f  // прибавка значения функции
					FSTP f  // сохранение значения в памяти, достаем число из стека

					FADD step // прибавка шага
					FST x	  // сохранение х в памяти, копирование числа из стека
					JMP LOOP_START // переход к метке начала

					LOOP_END : // метка окончания работы
				FWAIT  // синхронизация работы с ЦП
			}
			end_asm = clock();

			printf("\nComputing using ASM:\n");
			printf("value: %lf\n", f);
			printf("time: %f\n", ((float)(end_asm - begin_asm) / CLOCKS_PER_SEC));
			break;
		}
		case '0':
		{
			printf("Exit...\n");
			return 0;
		}
		default:
		{
			printf("Wrong command. Try again.\n");
			system("pause");
			system("cls");
			break;
		}
		}
	}
	return 0;
}