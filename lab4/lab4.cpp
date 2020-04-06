#include <stdio.h>
#include <conio.h>
#include <dos.h>

#define COUNT 28
#define COUNTER 8
#define DELAY 10
#define GO 65536

void BlackSabbath();
void sound();
void div();
void statusw();
void random();

void main() 
{
    clrscr();
	char command;
	do 
	{
        printf("\n----------------------------\n");
		printf("1. Black Sabbath - Iron Man\n");
		printf("2. Play another sound\n");
        printf("3. Division factor\n");
        printf("4. Status word\n");
        printf("5. Random number\n");
        printf("0. Exit the program\n");
        printf("----------------------------\n");
        printf("\nSelect command: ");

		fflush(stdin);
		scanf("%s", &command);
		switch (command) 
        {
			case '1': BlackSabbath(); break;
			case '2': sound(); break;
			case '3': div(); break;
			case '4': statusw(); break;
			case '5': random(); break;
		}
	} while (command != '0');
}

void BlackSabbath()
{
	// B, D, D, E, E, G, F#, G, F#, G, D, D, E, E
	int countHZ, byte;
	int HZ[COUNT] = { 73.43, 73.43, 73.43, 82.41, 82.41, 98.00, 92.50, 98.00, 92.50, 98.00, 73.43, 73.43, 82.41, 82.41, 
	                73.43, 73.43, 73.43, 82.41, 82.41, 98.00, 92.50, 98.00, 92.50, 98.00, 73.43, 73.43, 82.41, 82.41 };
	int MS[COUNT] = { 430, 430, 180, 180, 210, 95, 95, 95, 95, 250, 250, 220, 240, 240, 
	                430, 430, 180, 180, 210, 95, 95, 95, 95, 250, 250, 220, 240, 240 };
	int countDelay[COUNT] = { 100, 100, 90, 90, 90, 100, 100, 100, 100, 100, 100, 100, 100, 150, 
	                        100, 100, 90, 90, 90, 100, 100, 100, 100, 100, 100, 100, 100, 100 };
	long unsigned base = 1193180; // IRQ 18.2 times per second (1.19 MHz)

	for (countHZ = 0; countHZ < COUNT; countHZ++)
	{
        // 2th channel setting:
        // port 42h (system timer, channel 2, speaker sound)
        // port 43h (command register)
		outp(0x43, 0xB6);                   // 0xB6 - configure 2 channel by port 43h 
		byte = base / HZ[countHZ];
		outp(0x42, byte % 256);             // (low) the smallest byte of the frequency divider 
		outp(0x42, byte /= 256);            // (high) the highest byte of the frequency divider

		outp(0x61, inp(0x61) | 3);          // turn ON
		delay(MS[countHZ]);                 // wait
		outp(0x61, inp(0x61) & 0xFC);       // turn OFF
		delay(countDelay[countHZ]);         // wait
	}
}

void sound()
{
	int countHZ, byte;
	int HZ[COUNTER] = { 784, 392, 440, 440, 784, 440, 493, 493 };
	int MS[COUNTER] = { 200, 200, 200, 200, 200, 200, 200, 200 };
	int countDelay[COUNTER] = { 100, 100, 100, 100, 100, 100, 100, 100 };
	long unsigned base = 1193180; // IRQ 18.2 times per second

	for (countHZ = 0; countHZ < COUNTER; countHZ++)
	{
        // 2th channel setting:
        // port 42h (system timer, channel 2, speaker sound)
        // port 43h (command register)
		outp(0x43, 0xB6);                   // 0xB6 - configure 2 channel by port 43h 
		byte = base / HZ[countHZ];
		outp(0x42, byte % 256);             // (low) the smallest byte of the frequency divider 
		outp(0x42, byte /= 256);            // (high) the highest byte of the frequency divider

		outp(0x61, inp(0x61) | 3);          // turn ON
		delay(MS[countHZ]);                 // wait
		outp(0x61, inp(0x61) & 0xFC);       // turn OFF
		delay(countDelay[countHZ]);         // wait
	}
}


void div() 
{
    // port 40h (channel 0, system clock interrupion)
    // port 41h (channel 1, memory regeneration)
    // port 42h (channel 2, speaker sound)
    int channel;
	int ports[] = { 0x40, 0x41, 0x42 };
	int controlWord[] = { 0x0, 0x40, 0x80 };     // CLC commands: for reading of current state of register counter of channel
	unsigned byte, lowByte, highByte, maxByte;

	printf("\nDivision factor: \n");

	for (channel = 0; channel < 3; channel++) 
    {
		byte = 0;
		maxByte = 0;

		for (unsigned long i = 0; i < GO; i++) 
        {
			outp(0x43, controlWord[channel]);     // select channel
			lowByte = inp(ports[channel]);        // read the smallest byte
			highByte = inp(ports[channel]);       // read the highest byte
			byte = highByte * 256 + lowByte;	  // generate byte

			if (byte > maxByte) 
				maxByte = byte;
		}

		printf("\nChannel %d: %4X\n", channel, maxByte);
	}
}

void statusw() 
{
	// port 40h (channel 0, system clock interrupion)
    // port 41h (channel 1, memory regeneration)
    // port 42h (channel 2, speaker sound)
	int channel, state;
	char stateWord[8];
	int ports[] = {0x40, 0x41, 0x42};
	int controlWord[] = {226, 228, 232};     // 11 10 001 0, 11 10 010 0, 11 10 100 0	 
											 //       --1          -1-          1--
	printf("\nStatus word: \n");

	for (channel = 0; channel < 3; channel++)
    {
		outp(0x43, controlWord[channel]);    // select channel (CLC commands)
		state = inp(ports[channel]);         // read state
	
		for (int i = 7; i >= 0; i--)         // make binary
        {
			stateWord[i] = state % 2 + '0';
			state /= 2;
		}

		printf("\nChannel %d: ", channel);
		for (i = 0; i < 8;  i++) 
			printf("%c", stateWord[i]);
		printf("\n");
	}
}

void random() 
{
	char command;
	unsigned int limit = GO - 1;
    unsigned int num, numLow, numHigh;

	do 
    {
        printf("\n1. Set a limit\n");
        printf("2. Get a number\n");
        printf("0. Exit\n");

		fflush(stdin);
		printf("\nSelect command: ");
		scanf("%s", &command);

		switch (command) 
        {
		case '1':
			do 
            {
				printf("\nEnter a limit in range [1...65536]\n");
				fflush(stdin);
			} while (!scanf("%d", &limit) || limit < 1);

			outp(0x43, 0xB4);           // set regime 3 for 2nd channel, output compare register B
			outp(0x42, limit % 256);    // the smallest byte of the frequency divider
			outp(0x42, limit /= 256);   // the highest byte of the frequency divider      
			outp(0x61, inp(0x61) | 1);  // allow the channel to work (turn ON)
			break;

		case '2':
			outp(0x43, 128);            // port 43h defines 2th channel (128)
			numLow = inp(0x42);         // read the smallest num
			numHigh = inp(0x42);        // read the highest num
			num = numHigh * 256 + numLow; // generate num
			printf("\nRandom number: %u\n", num);
			break;
		}
	} while (command != '0');
	outp(0x61, inp(0x61) & 0xFC);   // turn OFF
}