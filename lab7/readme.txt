Задание:
Реализовать передачу текста через COM-порт между двумя приложениями в
обе стороны. Организовать передачу данных при помощи портов ввода-вывода в
DOS в одном приложении и при помощи прерываний или API Windows по работе
с COM портом - в другом.

Лабораторная реализована в 2 проекта: отдельно для Windows, отдельно для DOS.

Для эмуляции работы COM-портов следует установить одну из программ:
Configure Virtual Serial Port Driver или VSPE

В программе создать эмуляцию пары COM-портов.

Компиляция и запуск в Windows:

Компиляция в Visual Studio. Запуск одного приложения через CMD 2 раза.
Пример: 1) lab7.exe; 2) lab7.exe 1
При втором запуске после имени приложения нужно указать любой символ в 
аргументах для идентификации второго приложения как независимой единицы.

Компиляция и запуск в DOS:

Компиляция объектного и исполняемого файла происходит в DOSBox.
Далее исполняемые файлы поочередно запускаются в DOSBox.
При запуске от разных COM-портов нужно перейти к расположению файла
DOSBox и открыть файл DOSBox Options. В этом файле найти подобные строки:

serial1=directserial realport:COM1
serial2=dummy
#serial1=dummy
#serial2=directserial realport:COM2

Перед запуском первого приложения оставляем:

serial1=directserial realport:COM1
serial2=dummy
#serial1=dummy
#serial2=directserial realport:COM2

Перед запуском второго приложения оставляем:

#serial1=directserial realport:COM1
#serial2=dummy
serial1=dummy
serial2=directserial realport:COM2