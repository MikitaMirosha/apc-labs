�������:
����������� �������� ������ ����� COM-���� ����� ����� ������������ �
��� �������. ������������ �������� ������ ��� ������ ������ �����-������ �
DOS � ����� ���������� � ��� ������ ���������� ��� API Windows �� ������
� COM ������ - � ������.

������������ ����������� � 2 �������: �������� ��� Windows, �������� ��� DOS.

��� �������� ������ COM-������ ������� ���������� ���� �� ��������:
Configure Virtual Serial Port Driver ��� VSPE

� ��������� ������� �������� ���� COM-������.

���������� � ������ � Windows:

���������� � Visual Studio. ������ ������ ���������� ����� CMD 2 ����.
������: 1) lab7.exe; 2) lab7.exe 1
��� ������ ������� ����� ����� ���������� ����� ������� ����� ������ � 
���������� ��� ������������� ������� ���������� ��� ����������� �������.

���������� � ������ � DOS:

���������� ���������� � ������������ ����� ���������� � DOSBox.
����� ����������� ����� ���������� ����������� � DOSBox.
��� ������� �� ������ COM-������ ����� ������� � ������������ �����
DOSBox � ������� ���� DOSBox Options. � ���� ����� ����� �������� ������:

serial1=directserial realport:COM1
serial2=dummy
#serial1=dummy
#serial2=directserial realport:COM2

����� �������� ������� ���������� ���������:

serial1=directserial realport:COM1
serial2=dummy
#serial1=dummy
#serial2=directserial realport:COM2

����� �������� ������� ���������� ���������:

#serial1=directserial realport:COM1
#serial2=dummy
serial1=dummy
serial2=directserial realport:COM2