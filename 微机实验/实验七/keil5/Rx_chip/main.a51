ORG 0000H
AJMP START
ORG 0030H
START:
    MOV TMOD,#20H;���ö�ʱ��
    MOV TL1,#0FDH
    MOV TH1,#0FDH
    MOV SCON,#0D0H;���ô���ģʽΪ��ʽ3����ʹ��REN
    SETB TR1
LOOP:
    JBC RI,RECE;�жϽ��ձ�־
    SJMP LOOP     ; ��δ���յ������������ѯ
RECE:
    MOV A, SBUF   ; ��ȡSBUF�е����ݵ��ۼ���A
    JB PSW.0,ONEE;�Խ��յ������ݽ�����żУ��
    JB RB8,ERROR;
    AJMP RIGHT
ONEE:
    JNB RB8,ERROR
    AJMP RIGHT
RIGHT:
    MOV P2,A     ; �����յ������������P2��
    CLR RI        ;������ձ�־
    AJMP LOOP;���ؽ����ж�ѭ��
ERROR:
    CPL P0.0;���մ����ȡ��p0.0������led
    AJMP LOOP;���ؽ���ѭ���ٴν���
END