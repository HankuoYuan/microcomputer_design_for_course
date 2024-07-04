ORG 0000H
AJMP START
ORG 0030H
START:
    MOV TMOD,#20H;配置定时器
    MOV TL1,#0FDH
    MOV TH1,#0FDH
    MOV SCON,#0D0H;配置串口模式为方式3，并使能REN
    SETB TR1
LOOP:
    JBC RI,RECE;判断接收标志
    SJMP LOOP     ; 若未接收到数据则继续查询
RECE:
    MOV A, SBUF   ; 读取SBUF中的数据到累加器A
    JB PSW.0,ONEE;对接收到的数据进行奇偶校验
    JB RB8,ERROR;
    AJMP RIGHT
ONEE:
    JNB RB8,ERROR
    AJMP RIGHT
RIGHT:
    MOV P2,A     ; 将接收到的数据输出到P2口
    CLR RI        ;清除接收标志
    AJMP LOOP;跳回接收判断循环
ERROR:
    CPL P0.0;接收错误后取反p0.0口闪灭led
    AJMP LOOP;跳回接收循环再次接收
END