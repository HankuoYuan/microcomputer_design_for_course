;以下代码可以实现一个定时器产生周期为0.2和1s的两个方波脉冲
ORG 0000H 
MOV A,#6;
START: 
    AJMP MAIN ;跳入主程序 
    ORG 000BH 
    AJMP INT_T0 ;定时器 0 中断程序 
    ORG 0030H 
MAIN: 
    MOV TMOD,#01H ;设定时器 T0 为 16 位定时器 
    MOV TH0,#3CH ;设定 T0 的定时值(0.1s) 
    MOV TL0,#0B0H 
    SETB TR0 ;启动定时器 T0 
    SETB ET0 
    SETB EA 
MAIN_1: 
    SJMP MAIN_1 
INT_T0: 
    DEC A;
    JZ PWM2;
GETTIN:
    PUSH ACC ;保护现场 
    PUSH PSW 
    MOV TH0,#3CH ;设定 T0 的定时值(0.1s)
    MOV TL0,#0B0H
    CPL P2.5 
    POP PSW ;恢复现场 
    POP ACC 
    RETI
PWM2:
    MOV A,#5;
    CPL P2.6;
    AJMP GETTIN;
END
;
;下面是思考题的代码，同时输出38k和1k的脉冲，但38k检测不出来，波形仿真好像最高是5khz。
;ORG 0000H 
;START: 
    ;AJMP MAIN ;跳入主程序 
    ;ORG 000BH 
    ;AJMP INT_T0 ;定时器 0 中断程序
    ;ORG 001BH
    ;AJMP INT_T1;
    ;ORG 0030H 
;MAIN: 
    ;MOV TMOD,#01H ;设定时器 T0 为 16 位定时器 
    ;MOV TH0,#0FFH ;设定 T0 的定时值(0.1s) 
    ;MOV TL0,#006H 
    ;SETB TR0 ;启动定时器 T0 
    ;SETB ET0 
    ;SETB EA
    ;MOV TH1,#0FFH ;设定 T0 的定时值(0.1s) 
    ;MOV TL1,#0FAH 
    ;SETB TR1 ;启动定时器 T0 
    ;SETB ET1
;MAIN_1: 
    ;SJMP MAIN_1 
;INT_T0: 
    ;PUSH ACC ;保护现场 
    ;PUSH PSW 
    ;MOV TH0,#0FFH ;设定 T0 的定时值(0.1s)
    ;MOV TL0,#006H
    ;CPL P2.5 
    ;POP PSW ;恢复现场 
    ;POP ACC 
    ;RETI
;INT_T1:
    ;PUSH ACC ;保护现场 
    ;PUSH PSW
    ;MOV TH1,#0FFH
    ;MOV TL1,#0FAH
    ;CPL P2.6
    ;POP PSW ;恢复现场 
    ;POP ACC 
    ;RETI
;END