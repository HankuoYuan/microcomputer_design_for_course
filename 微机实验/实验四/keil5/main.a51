;���´������ʵ��һ����ʱ����������Ϊ0.2��1s��������������
ORG 0000H 
MOV A,#6;
START: 
    AJMP MAIN ;���������� 
    ORG 000BH 
    AJMP INT_T0 ;��ʱ�� 0 �жϳ��� 
    ORG 0030H 
MAIN: 
    MOV TMOD,#01H ;�趨ʱ�� T0 Ϊ 16 λ��ʱ�� 
    MOV TH0,#3CH ;�趨 T0 �Ķ�ʱֵ(0.1s) 
    MOV TL0,#0B0H 
    SETB TR0 ;������ʱ�� T0 
    SETB ET0 
    SETB EA 
MAIN_1: 
    SJMP MAIN_1 
INT_T0: 
    DEC A;
    JZ PWM2;
GETTIN:
    PUSH ACC ;�����ֳ� 
    PUSH PSW 
    MOV TH0,#3CH ;�趨 T0 �Ķ�ʱֵ(0.1s)
    MOV TL0,#0B0H
    CPL P2.5 
    POP PSW ;�ָ��ֳ� 
    POP ACC 
    RETI
PWM2:
    MOV A,#5;
    CPL P2.6;
    AJMP GETTIN;
END
;
;������˼����Ĵ��룬ͬʱ���38k��1k�����壬��38k��ⲻ���������η�����������5khz��
;ORG 0000H 
;START: 
    ;AJMP MAIN ;���������� 
    ;ORG 000BH 
    ;AJMP INT_T0 ;��ʱ�� 0 �жϳ���
    ;ORG 001BH
    ;AJMP INT_T1;
    ;ORG 0030H 
;MAIN: 
    ;MOV TMOD,#01H ;�趨ʱ�� T0 Ϊ 16 λ��ʱ�� 
    ;MOV TH0,#0FFH ;�趨 T0 �Ķ�ʱֵ(0.1s) 
    ;MOV TL0,#006H 
    ;SETB TR0 ;������ʱ�� T0 
    ;SETB ET0 
    ;SETB EA
    ;MOV TH1,#0FFH ;�趨 T0 �Ķ�ʱֵ(0.1s) 
    ;MOV TL1,#0FAH 
    ;SETB TR1 ;������ʱ�� T0 
    ;SETB ET1
;MAIN_1: 
    ;SJMP MAIN_1 
;INT_T0: 
    ;PUSH ACC ;�����ֳ� 
    ;PUSH PSW 
    ;MOV TH0,#0FFH ;�趨 T0 �Ķ�ʱֵ(0.1s)
    ;MOV TL0,#006H
    ;CPL P2.5 
    ;POP PSW ;�ָ��ֳ� 
    ;POP ACC 
    ;RETI
;INT_T1:
    ;PUSH ACC ;�����ֳ� 
    ;PUSH PSW
    ;MOV TH1,#0FFH
    ;MOV TL1,#0FAH
    ;CPL P2.6
    ;POP PSW ;�ָ��ֳ� 
    ;POP ACC 
    ;RETI
;END