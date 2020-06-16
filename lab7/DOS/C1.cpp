#include <dos.h>
#include <stdio.h>

int com_check_rhr(int base) {
    unsigned char lsr = inp(base + 0x05);                       // LSR - регистр состояния линии
    return (lsr & 0x01) == 0x01;                                // проверка на наличие данных в RHR
}

char com_receive(int base) {
    return inp(base);
}

char* com_receive_string(int base) {
    int i = 0;
    char buffer[512];
    do {

        while (!com_check_rhr(base));                  // проверка наличия символа в RHR
        buffer[i] = com_receive(base);                  // считываем байт
        
    } while (buffer[i++] != 0);                // пока не считаем '\0'

    return buffer;
}

int com_check_tbe(int base) {
    unsigned char LSR = inp(base + 0x05);           //  LSR - регистр состояния линии  
    return ((LSR & 0x20) >> 5) == 0x01;             // проверка бита TBE, если 1 - то регистр хранения данных пустой 
}

void com_send_string(char* string, int base) {

    do {

        unsigned int LCR;

        LCR = inp(base + 0x03);                     // LCR - регистр управления линией
        outp(base + 0x03, LCR | 0x80);              // устанавливаем DLAB бит в 1 => 0 и 1 порты служат для установки частоты

        outp(base, 0x0C);                       // установка частоты = 9600 бод
        outp(base + 0x01, 0x00);

        outp(base + 0x03, LCR & 0x7f);         // сброс DLAB бита

        outp(base + 0x01, 0x00);                // disable interrupts 

        outp(base + 0x03, 0x1b);            // 1b = 11 0 11   1) бит четности - четный 2) 1 стоп-бит 3) кол-во бит данных - 8
        outp(base + 0x04, 0x00);            // регистр управления модемом: DTR - 0, RTS - 0                   
        unsigned char LSR;                                              // регистр состояния линии
        unsigned char mcr = inp(base + 0x04);                           // регистр управления модемом 
        outp(base + 0x04, mcr | 0x02);                                  // сигнал запроса на передачу данных 

        do LSR = inp(base + 0x05) & 0x40;                               // статус передатчика 
        while (LSR != 0x40);                                            // [6] - TE = 1 => передатчик пустой 

        outp(base, *string);                                            // запись символа 

        while (com_check_tbe(base));                                    // ожидаем пока передатчик будет готов 
        outp(base + 0x04, mcr | 0xfd);                       // передача 1 символа
    } while (*(string++));
}

int sender();
int receiver();

int main() {

    puts("--------------------------");
    puts("ENTER 1 TO SEND MESSAGE");
    puts("ENTER 2 TO RECEIVE MESSAGE");
    puts("ENTER 0 TO EXIT");
    puts("--------------------------");

    while(1)
    {
       sender();
       receiver(); 
    }

    return 0;
}

int sender() 
{
        int base = 0x3f8;
        char str[20];
        unsigned int LCR;

        LCR = inp(base + 0x03);                     // LCR - регистр управления линией
        outp(base + 0x03, LCR | 0x80);              // устанавливаем DLAB бит в 1 => 0 и 1 порты служат для установки частоты

        outp(base, 0x0C);                       // установка частоты = 9600 бод 
        outp(base + 0x01, 0x00);                //3f9

        outp(base + 0x03, LCR & 0x7f);         // сброс DLAB бита (установка в 0)

        outp(base + 0x01, 0x00);                // disable interrupts 

        outp(base + 0x03, 0x1b);            // 1b = 11 0 11   1) бит четности - четный 2) стоп-бит устанавливаем единицей 3) кол-во бит данных - 8
        outp(base + 0x04, 0x00);            // регистр управления модемом: готовность терминала к передаче данных - 0, сигнал запроса на передачу данных - 0 

    while(1)
    {
        puts("Input string: ");
        fgets(str, 20, stdin);
        if(*str == '\n' && *(str + 1) == '\0')
        {
            com_send_string(str, base); 
            return 0;
        }

        com_send_string(str, base); 
    }
}

int receiver()
{
        int receiver = 0x3f8;
        char *rec;
        char str[20];

        unsigned int LCR;

        LCR = inp(receiver + 0x03);                                     // LCR - регистр управления линией
        outp(receiver + 0x03, LCR | 0x80);                              // устанавливаем DLAB бит в 1 => 0 и 1 порты служат для установи частоты

        outp(receiver, 0x0C);                                           // установка частоты = 9600 бод 
        outp(receiver + 0x01, 0x00);

        outp(receiver + 0x03, LCR & 0x7f);                              // сброс DLAB бита 

        outp(receiver + 0x01, 0x00);                                    // disable interrupts 

        outp(receiver + 0x03, 0x1b);                                    // 1b = 11 0 11   1) бит четности - четный 2) 1 стоп-бит 3) кол-во бит данных - 8
        outp(receiver + 0x04, 0x00);                                    // регистр управления модемом: DTR - 0, RTS - 0 

    while(1)
    {
        puts("Otput string:");
        rec = com_receive_string(receiver);
        if(*rec == '\n' && *(rec + 1) == '\0')
            return 0;
        printf("%s", rec); 
    } 
}