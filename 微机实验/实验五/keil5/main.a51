ORG 0000H 
AJMP 0030H 
ORG 0003H;�ж�0������ת
LJMP INTRUPT0
ORG 0013H ;�ж�1������ת
LJMP HA2S3 
ORG 0030H ;�������������
HA2S: 
    MOV P1,#0fFH ;����P1��ֵ
    ORL P3,#00H ;��ʼ��P3�ڵ�ֵ
    MOV PSW,#00H 
    MOV SP,#53H
    MOV A,#0FFH
    MOV R1,#05;���õƵ����Ĵ���
HA2S1: 
    JB P3.4,HA2S1;�жϰ���p3.4��״̬������������ˮ��
    ORL IP,#04H ;�����ж����ȼ�
    ORL IE,#85H ;ʹ���ⲿ�ж�
HA2S2: ;��ˮ��ѭ��
    MOV P1,#081H
    ACALL HA2S7 
    MOV P1,#082H 
    ACALL HA2S7 
    MOV P1,#084H 
    ACALL HA2S7 
    MOV P1,#088H 
    ACALL HA2S7 
    MOV P1,#090H 
    ACALL HA2S7 
    MOV P1,#0A0H 
    ACALL HA2S7 
    MOV P1,#0C0H 
    ACALL HA2S7 
    SJMP HA2S2 
HA2S3: MOV B,R2 ;�ж�1���ִ���
HA2S5: SETB P1.7 ;����p1.7Ϊ1
    ACALL HA2S6 
    CPL P1.7 ;ȡ��P1.7
    ACALL HA2S6 
    JNB P3.3,HA2S5 
    MOV R2,B 
    RETI
INTRUPT0:;�ж�0���ִ���
    MOV P1,A
    ACALL HA2S7
    MOV P1,#80H
    ACALL HA2S7
    DJNZ R1,INTRUPT0
    MOV R1,#05;���õƵ����Ĵ���
    AJMP OVERSTOP
OVERSTOP:
    JNB P3.2,OVERSTOP
    RETI
HA2S6: MOV R2,#06H ;����ʱ
    ACALL DELAY 
    RET 
HA2S7: MOV R2,#30H ;����ʱ
    ACALL DELAY 
    RET 
DELAY: PUSH 02H ;��ʱ�ӳ���
DELAY1: PUSH 02H 
DELAY2: PUSH 02H 
DELAY3: DJNZ R2,DELAY3 
    POP 02H 
    DJNZ R2,DELAY2 
    POP 02H 
    DJNZ R2,DELAY1 
    POP 02H 
    DJNZ R2,DELAY 
    RET
END