ORG 0000H
AJMP MAIN
ORG 0030H
MAIN:
    MOV    SP,#60H;
    MOV    A,#0FEH;置灯亮	11111110
LOOP:
    JNB P3.1,MODE2LOOP1;
    JNB P3.0,ALLSTOP;
    RL    A;
    MOV P2, A;激活P2口
    ACALL    DELAY;进入延时，1S
    JNB P2.7,LOOP2;
    AJMP    LOOP;返回LOOP循环
LOOP2:
    JNB P3.1,MODE2LOOP1;
    JNB P3.0,ALLSTOP;
    RR    A;
    MOV P2, A;激活P2口
    ACALL    DELAY;进入延时，1S
    JNB P2.0,LOOP;
    AJMP    LOOP2;返回LOOP循环
DELAY:
    MOV R4,#20;1us  12mHZ 机器时间周期为1us
DEL1:
    MOV R6,#100;1us T=(1+1+1+((250*2+2)*100+2)*20+2)=1004050us  1s
DEL2:
    MOV    R7,#250;1us
    DJNZ    R7,$;2us   循环250次 该语句消耗2微秒，一个机器周期
    DJNZ    R6,DEL2;2us   每次减1，不等于0则跳转，该语句即为从Del1到这句，循环100次
    DJNZ    R4,DEL1;2us   循环20次
    RETI     ;2us
ALLSTOP:
    NOP;1us  12mHZ 机器时间周期为1us
    JB P3.0,LOOP;
    AJMP ALLSTOP;
MODE2LOOP1:
    JB P3.1,LOOP;
    JNB P3.0,ALLSTOP;
    RL    A;
    MOV P2, A;激活P2口
    ACALL    DELAY;进入延时，1S
    AJMP    MODE2LOOP1;返回LOOP循环
END