#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <io.h>

int msCounter = 0;

void interrupt far (*oldInt70h)(void);		// Oeacaoaeu ia noa?ue ia?aaio?ee i?a?uaaiey ??A								 
void interrupt far (*oldInt4Ah)(void);		// Oeacaoaeu ia noa?ue ia?aaio?ee i?a?uaaiey aoaeeuieea
void interrupt far newInt4Ahandler(void);	// Iiaue ia?aaio?ee i?a?uaaiey ??A
void interrupt far newInt70handler(void);	// Iiaue ia?aaio?ee i?a?uaaiey aoaeeuieea

void freeze(void);
void unfreeze(void);

int BCDToInteger(int bcd);
unsigned char IntToBCD(int value);
void getTime(void);
void setTime(void);
void delay_time(void);
void wait(void);
void enableAlarm(void);
void disableAlarm(void);

void main() {
	char c, value;
	clrscr();
	printf("\n1. Show time\n2. Set time\n3. Delay time\n4. Enable alarm\n5. Disable alarm\n0. Exit\n\n");
	while (c != 0) {
		c = getch();
		switch (c) {
			case '1': getTime(); break;
			case '2': setTime(); break;
			case '3': delay_time(); break;
			case '4': enableAlarm(); break;
			case '5': disableAlarm(); break;
			case '0': return;
		}
	}
}
// I?eaaiea inaiai?aaiey ?ania
void wait(void) {
	do {								// I?eaaiea, iiea ?anu caiyou
		outp(0x70, 0x0A);
	} while (inp(0x71) & 0x80);			// 0x80 = 10000000 iiea 7-e aeo - 1, ?anu caiyou
}
// Iieo?aiea oaeouaai cia?aiey a?aiaie
void getTime(void) {
	int value;

	wait();
	outp(0x70, 0x04);					// Oaeouee ?an
	value = inp(0x71); 
	printf("%d:", BCDToInteger(value)); 
	
	wait();
	outp(0x70, 0x02);					// Oaeouay ieiooa
	value = inp(0x71); 
	printf("%d:", BCDToInteger(value)); 

	wait();
	outp(0x70, 0x00);					// Oaeouay naeoiaa
	value = inp(0x71); 
	printf("%d   ", BCDToInteger(value)); 
	
	wait();
	outp(0x70, 0x07);					// Oaeouee aaiu ianyoa
	value = inp(0x71); 
	printf("%d.", BCDToInteger(value)); 
	
	wait();
	outp(0x70, 0x08);					// Oaeouee ianyo
	value = inp(0x71); 
	printf("%d.", BCDToInteger(value)); 
	
	wait();
	outp(0x70, 0x09);					// Oaeouee aia
	value = inp(0x71); 
	printf("%d   ", BCDToInteger(value)); 
	
	wait();
	outp(0x70, 0x06);					// Oaeouee aaiu iaaaee
	value = inp(0x71);
	switch (BCDToInteger(value)) {
		case 2: printf("Monday\n"); break;
		case 3: printf("Tuesday\n"); break;
		case 4: printf("Wednesday\n"); break;
		case 5: printf("Thursday\n"); break;
		case 6: printf("Friday\n"); break;
		case 7: printf("Saturday\n"); break;
		case 1: printf("Sunday\n"); break;
		default: printf("Day of week is not set\n"); break;
	}
}
// Onoaiiaea a?aiaie
void setTime(void) {
	int value;
	freeze();							// Cai?aoeou iaiiaeaiea ?ania

	do {
		printf("Enter hour: ");
		fflush(stdin);
		scanf("%d", &value);
	} while (value > 23 || value < 0);
	outp(0x70, 0x04);
	outp(0x71, IntToBCD(value));		// Cia?aiea a ii?o 71h a BCD-oi?iaoa
 
	do {
		printf("Enter minute: ");
		fflush(stdin);
		scanf("%d", &value);
	} while (value > 59 || value < 0);
	outp(0x70, 0x02);
	outp(0x71, IntToBCD(value));

	do {
		printf("Enter second: ");
		fflush(stdin);
		scanf("%d", &value);
	} while (value > 59 || value < 0);
	outp(0x70, 0x00);
	outp(0x71, IntToBCD(value));
 
	do {
		printf("Enter week day number: ");
		fflush(stdin);
		scanf("%d", &value);
	} while (value > 7 || value < 1);
	outp(0x70, 0x06);
	outp(0x71, IntToBCD(value));

	do {
		printf("Enter day of month: ");
		fflush(stdin);
		scanf("%d", &value);
	} while (value > 31 || value < 1);
	outp(0x70, 0x07);
	outp(0x71, IntToBCD(value));

	do {
		printf("Enter mounth: ");
		fflush(stdin);
		scanf("%d", &value);
	} while (value > 12 || value < 1);
	outp(0x70, 0x08);
	outp(0x71, IntToBCD(value));

	do {
		printf("Enter year: ");
		fflush(stdin);
		scanf("%d", &value);
	} while (value > 99 || value < 0);
	outp(0x70, 0x09);
	outp(0x71, IntToBCD(value));

	unfreeze();							// ?ac?aoeou iaiiaeaiea ?ania
}
// Onoaiiaea aoaeeuieea
void enableAlarm(void) {
	int value, second, minute, hour;

	do {
		printf("Enter second: ");
		fflush(stdin);
		scanf("%d", &second);
	} while (second > 59 || second < 0);

	do{
		printf("Enter minute: ");
		fflush(stdin);
		scanf("%d", &minute);
	} while (minute > 59 || minute < 0);

	do {
		printf("Enter hour: ");
		fflush(stdin);
		scanf("%d", &hour);
	} while (hour > 23 || hour < 0);

	outp(0x70, 0x01);
	outp(0x71, IntToBCD(second));

	outp(0x70, 0x03);
	outp(0x71, IntToBCD(minute));

	outp(0x70, 0x05);
	outp(0x71, IntToBCD(hour));

	outp(0x70, 0xB);
	outp(0x71, inp(0x71) | 0x20);

	disable();
	oldInt4Ah = getvect(0x4A);
	setvect(0x4A, newInt4Ahandler);
	enable(); 			// 0xFE = 11111110, aeo 0 a 0, ?oiau ?ac?aoeou i?a?uaaiey io ??A 
}
// Na?in aoaeeuieea
void disableAlarm() {
	if (oldInt4Ah == NULL) {
		printf("Alarm is already disable or not set.\n");
		return;
	}
	_disable();
	setvect(0x4A, oldInt4Ah);
	outp(0xA1, (inp(0xA0) | 0x01));
	outp(0x70, 0x0B);
	outp(0xA1, inp(0x71) & 0xDF);		// 0xDF = 11011111, aeo 0 a 1, aeo 5 a 0, ?oiau cai?aoeou i?a?uaaiey io aoaeeuieea
	_enable();
	printf("Alarm is disable.\n");
}
// Cai?aoeou iaiiaeaiea ?ania
void freeze(void) {
	unsigned char value;
	wait();								// I?eaaiea, iiea ?anu caiyou
	outp(0x70, 0x0B);
	value = inp(0x71);					// ?eoaai ?aaeno? ninoiyiey B
	value |= 0x80;						// Caiaiyai 7-e aeo ia 1 aey cai?auaiey iaiiaeaiey ?ania
	outp(0x70, 0x0B);
	outp(0x71, value);					// Caienuaaai iiaia cia?aiea a ?aaeno? B, iaiiaeaiea ?ania cai?auaii 
}
// ?ac?aoeou iaiiaeaiea ?ania
void unfreeze(void) {
	unsigned char value;
	wait();								// I?eaaiea, iiea ?anu caiyou
	outp(0x70, 0x0B);
	value = inp(0x71);					// ?eoaai ?aaeno? ninoiyiey B
	value -= 0x80;						// Caiaiyai 7-e aeo ia 0 aey ?ac?aoaiey iaiiaeaiey ?ania
	outp(0x70, 0x0B);
	outp(0x71, value);					// Caienuaaai iiaia cia?aiea a ?aaeno? B, iaiiaeaiea ?ania ?ac?aoaii
}
// Iiaue ia?aaio?ee i?a?uaaiey ??A
void interrupt far newInt70handler(void) {
	msCounter++;						// N??o?ee ieeenaeoia
	outp(0x70, 0x0C);					// Anee ?aaeno? C ia aoaao i?i?eoai iinea IRQ 8, oiaaa i?a?uaaiea ia neo?eony niiaa
	inp(0x71);			 
	outp(0x20, 0x20);					// Iinueaai eiio?ieea?o i?a?uaaiee (master) neaiae EOI (end of interrupt)
	outp(0xA0, 0x20);					// Iinueaai aoi?iio eiio?ieea?o i?a?uaaiee (slave) neaiae EOI (end of interrupt) 
}
// Iiaue ia?aaio?ee i?a?uaaiey aoaeeuieea
void interrupt far newInt4Ahandler(void) {
	write(1, "Good morning!", 13);
	disableAlarm();
}
// Caaa??ea a?aiaie
void delay_time(void) {
	unsigned long delayPeriod;
	unsigned char value;

	do {
		printf("Enter delay time in milliseconds: ");
		fflush(stdin);
		scanf("%ld", &delayPeriod);
	} while (delayPeriod < 1);
	printf("Time before delay: ");
	getTime();
	printf("Delaying ...\n");

	_disable();							// Cai?aoeou i?a?uaaiey
	oldInt70h = getvect(0x70);
	setvect(0x70, newInt70handler);
	_enable();							// ?ac?aoeou i?a?uaaiey

	// ?acianee?iaaiea eeiee neaiaea cai?ina io ??A
	outp(0xA1, inp(0xA1) & 0xFE);			// 0xFE = 11111110, aeo 0 a 0, ?oiau ?ac?aoeou i?a?uaaiey io ??A

	// Aee??aiea ia?eiae?aneiai i?a?uaaiey
	outp(0x70, 0x0B);					// Auae?aai ?aaeno? B
	outp(0x71, inp(0x71) | 0x40);			// 0x40 = 01000000, 6-e aeo ?aaeno?a B onoaiaaeeaaai a 1 

	msCounter = 0;
	while (msCounter != delayPeriod);	// Caaa??ea ia caaaiiia eie-ai ieeeenaeoia 
	printf("End delay of %d ms\n", msCounter);
	_disable();
	setvect(0x70, oldInt70h);			// Ainnoaiaaeeaaai noa?ue ia?aaio?ee
	_enable();
	printf("Time after delay: ");
	getTime();
}
// Ia?aaia BCD a int
int BCDToInteger(int bcd) {
	return bcd % 16 + bcd / 16 * 10;
}
// Ia?aaia int a BCD
unsigned char IntToBCD(int value) {
	return (unsigned char)((value / 10) << 4) | (value % 10);
}
