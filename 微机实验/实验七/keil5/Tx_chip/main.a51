ORG 0000H
AJMP START
ORG 0030H
START:
    MOV PSW,#00H
    MOV SP,#53H
    MOV P2,#0EH
    MOV P0,#0FFH
INIT:
    MOV TMOD,#20H;���ö�ʱ��
    MOV TL1,#0FDH
    MOV TH1,#0FDH
    MOV SCON,#0C0H;���ô���ģʽΪ��ʽ3
    SETB TR1
LOOP:
    MOV P2,#0EH;ɨ���һ��
    JNB P0.0,NUM0
    JNB P0.1,NUM4
    JNB P0.2,NUM8
    JNB P0.3,NUMC
    MOV P2,#0DH;ɨ��ڶ���
    JNB P0.0,NUM1
    JNB P0.1,NUM5
    JNB P0.2,NUM9
    JNB P0.3,NUMD
    MOV P2,#0BH;ɨ�������
    JNB P0.0,NUM2
    JNB P0.1,NUM6
    JNB P0.2,NUMA
    JNB P0.3,NUME
    MOV P2,#07H;ɨ�������
    JNB P0.0,NUM3
    JNB P0.1,NUM7
    JNB P0.2,NUMB
    JNB P0.3,NUMF
    AJMP LOOP
NUM0:;�����Ǽ�ֵ��ʾ�ͷ���
    MOV A,#0C0H
    MOV P1,A
    AJMP TX
NUM4:
    MOV A,#099H
    MOV P1,A
    AJMP TX
NUM8:
    MOV A,#080H
    MOV P1,A
    AJMP TX
NUMC:
    MOV A,#0C6H
    MOV P1,A
    AJMP TX
NUM1:
    MOV A,#0F9H
    MOV P1,A
    AJMP TX
NUM5:
    MOV A,#092H
    MOV P1,A
    AJMP TX
NUM9:
    MOV A,#090H
    MOV P1,A
    AJMP TX
NUMD:
    MOV A,#0A1H
    MOV P1,A
    AJMP TX
NUM2:
    MOV A,#0A4H
    MOV P1,A
    AJMP TX
NUM6:
    MOV A,#082H
    MOV P1,A
    AJMP TX
NUMA:
    MOV A,#088H
    MOV P1,A
    AJMP TX
NUME:
    MOV A,#086H
    MOV P1,A
    AJMP TX
NUM3:
    MOV A,#0B0H
    MOV P1,A
    AJMP TX
NUM7:
    MOV A,#0F8H
    MOV P1,A
    AJMP TX
NUMB:
    MOV A,#083H
    MOV P1,A
    AJMP TX
NUMF:
    MOV A,#08EH
    MOV P1,A
    AJMP TX
TX:;������ż���鸳ֵ
    MOV C,PSW.0
    MOV TB8,C
    MOV SBUF,A;��ֵ���ڻ�����
    AJMP CONT
CONT:
    JNB TI,CONT;�ж��Ƿ��ͳɹ�
    CLR TI;�����־
    AJMP LOOP;�����ж�ѭ��
END