ORG 0000H
AJMP MAIN
ORG 0030H
MAIN:
    MOV    SP,#60H;
    MOV    A,#0FEH;�õ���	11111110
LOOP:
    JNB P3.1,MODE2LOOP1;
    JNB P3.0,ALLSTOP;
    RL    A;
    MOV P2, A;����P2��
    ACALL    DELAY;������ʱ��1S
    JNB P2.7,LOOP2;
    AJMP    LOOP;����LOOPѭ��
LOOP2:
    JNB P3.1,MODE2LOOP1;
    JNB P3.0,ALLSTOP;
    RR    A;
    MOV P2, A;����P2��
    ACALL    DELAY;������ʱ��1S
    JNB P2.0,LOOP;
    AJMP    LOOP2;����LOOPѭ��
DELAY:
    MOV R4,#20;1us  12mHZ ����ʱ������Ϊ1us
DEL1:
    MOV R6,#100;1us T=(1+1+1+((250*2+2)*100+2)*20+2)=1004050us  1s
DEL2:
    MOV    R7,#250;1us
    DJNZ    R7,$;2us   ѭ��250�� ���������2΢�룬һ����������
    DJNZ    R6,DEL2;2us   ÿ�μ�1��������0����ת������伴Ϊ��Del1����䣬ѭ��100��
    DJNZ    R4,DEL1;2us   ѭ��20��
    RETI     ;2us
ALLSTOP:
    NOP;1us  12mHZ ����ʱ������Ϊ1us
    JB P3.0,LOOP;
    AJMP ALLSTOP;
MODE2LOOP1:
    JB P3.1,LOOP;
    JNB P3.0,ALLSTOP;
    RL    A;
    MOV P2, A;����P2��
    ACALL    DELAY;������ʱ��1S
    AJMP    MODE2LOOP1;����LOOPѭ��
END