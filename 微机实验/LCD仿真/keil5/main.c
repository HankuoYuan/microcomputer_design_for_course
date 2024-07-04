//平台：HJ-4WD/HJ-E/HL-1智能小车 + Keil U4 + STC89C52
//晶振:11.0592MHZ

#include<AT89x52.H>
#include <intrins.h>   //包含nop等系统函数

//HL-1小车驱动接线定义
#define Left_moto_go      {P1_2=1,P1_3=0;}    //左边电机向前走
#define Left_moto_back    {P1_2=0,P1_3=1;}    //左边电机向后转
#define Left_moto_Stop    {P1_2=0,P1_3=0;}    //左边电机停转
#define Right_moto_go     {P1_6=1,P1_7=0;}    //右边电机向前走
#define Right_moto_back   {P1_6=0,P1_7=1;}    //右边电机向后走
#define Right_moto_Stop   {P1_6=0,P1_7=0;}    //右边电机停转

#define left     'L'//Left
#define right    'R'//Right
#define forward       'F'//Forward
#define backward     'B'//Backward
#define stop     'S'//Stop

#define LCM_Data  P0   //定义液晶屏数据口
#define Busy    0x80   //用于检测LCM状态字中的Busy标识


sbit LCM_RW = P1 ^ 1 ; //定义LCD引脚
sbit LCM_RS = P1 ^ 0 ;
sbit LCM_E = P2 ^ 5  ;

char code str_f[] =  "收到指令，向前!\n";
char code str_b[] = "收到指令，向后!\n";
char code str_l[] = "收到指令，向左!\n";
char code str_r[] = "收到指令，向右!\n";
char code str_s[] = "收到指令，停止!\n";

unsigned char code header[] = "=SportSituation="; //LCD1602显示格式
unsigned char code table_f[] = "    Forward    ";
unsigned char code table_b[] = "    Backward    ";
unsigned char code table_l[] = "      Left      ";
unsigned char code table_r[] = "     Right     ";
unsigned char code table_s[] = "      Stop      ";
unsigned char code clear[] = "                ";


bit  flag_REC = 0;
bit  flag    = 0;


unsigned char  i = 0;
unsigned char  dat = 0;
unsigned char  buff[5] = 0; //接收缓冲字节


//延时函数
void delay(unsigned int k)
{
    unsigned int x, y;
    for (x = 0; x < k; x++)
        for (y = 0; y < 2000; y++);
}

void Delay10us(unsigned char i)     //10us延时函数 启动超声波模块时使用
{
    unsigned char j;
    do
    {
        j = 10;
        do
        {
            _nop_();
        }
        while (--j);
    }
    while (--i);
}

/************************************LCD1602液晶屏驱动函数************************************************/
//*******************读状态*************************//
unsigned char ReadStatusLCM(void)
{
    LCM_Data = 0xFF;
    LCM_RS = 0;
    Delay10us(1);
    LCM_RW = 1;
    Delay10us(1);
    do
    {
        LCM_E = 0;
        Delay10us(1);
        LCM_E = 0;
        Delay10us(1);
        LCM_E = 1;
        Delay10us(1);
    }
    while (LCM_Data & Busy); //检测忙信号
    return (LCM_Data);
}

/****************写数据************************/
void WriteDataLCM(unsigned char WDLCM)
{
    ReadStatusLCM(); //检测忙
    LCM_Data = WDLCM;
    LCM_RS = 1;
    Delay10us(1);
    LCM_RW = 0;
    Delay10us(1);
    LCM_E = 0; //若晶振速度太高可以在这后加小的延时
    Delay10us(1);
    LCM_E = 0; //延时
    Delay10us(1);
    LCM_E = 1;
    Delay10us(1);
}

//****************写指令*************************//
void WriteCommandLCM(unsigned char WCLCM, BuysC) //BuysC为0时忽略忙检测
{
    if (BuysC) ReadStatusLCM(); //根据需要检测忙
    LCM_Data = WCLCM;
    LCM_RS = 0;
    Delay10us(1);
    LCM_RW = 0;
    Delay10us(1);
    LCM_E = 0;
    Delay10us(1);
    LCM_E = 0;
    Delay10us(1);
    LCM_E = 1;
    Delay10us(1);
}



//*******************LCM初始化**********************//
void LCMInit(void)
{
    LCM_Data = 0;
    WriteCommandLCM(0x38, 0); //三次显示模式设置，不检测忙信号
    delay(5);
    WriteCommandLCM(0x38, 0);
    delay(5);
    WriteCommandLCM(0x38, 0);
    delay(5);

    WriteCommandLCM(0x38, 1); //显示模式设置,开始要求每次检测忙信号
    WriteCommandLCM(0x08, 1); //关闭显示
    WriteCommandLCM(0x01, 1); //显示清屏
    WriteCommandLCM(0x06, 1); // 显示光标移动设置
    WriteCommandLCM(0x0c, 1); // 显示开及光标设置
}

//*********************按指定位置显示一个字符***********************//
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
    Y &= 0x1;
    X &= 0xF; //限制X不能大于15，Y不能大于1
    if (Y) X |= 0x40; //当要显示第二行时地址码+0x40;
    X |= 0x80; //算出指令码
    WriteCommandLCM(X, 1); //发命令字
    WriteDataLCM(DData); //发数据
}

//**********************按指定位置显示一串字符*************************//
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData)
{
    unsigned char ListLength;

    ListLength = 0;
    Y &= 0x1;
    X &= 0xF; //限制X不能大于15，Y不能大于1
    while (DData[ListLength] > 0x19) //若到达字串尾则退出
    {
        if (X <= 0xF) //X坐标应小于0xF
        {
            DisplayOneChar(X, Y, DData[ListLength]); //显示单个字符
            ListLength++;
            X++;
        }
    }
}


/***************************************************************************/

//字符串发送函数
void send_str_f()
// 传送字串
{
    unsigned char i = 0;
    while (str_f[i] != '\0')
    {
        SBUF = str_f[i];
        while (!TI);               // 等特数据传送
        TI = 0;                    // 清除数据传送标志
        i++;                    // 下一个字符
    }
}

void send_str_b()
// 传送字串
{
    unsigned char i = 0;
    while (str_b[i] != '\0')
    {
        SBUF = str_b[i];
        while (!TI);               // 等特数据传送
        TI = 0;                    // 清除数据传送标志
        i++;                    // 下一个字符
    }
}

void send_str_l()
// 传送字串
{
    unsigned char i = 0;
    while (str_l[i] != '\0')
    {
        SBUF = str_l[i];
        while (!TI);               // 等特数据传送
        TI = 0;                    // 清除数据传送标志
        i++;                    // 下一个字符
    }
}

void send_str_r()
// 传送字串
{
    unsigned char i = 0;
    while (str_r[i] != '\0')
    {
        SBUF = str_r[i];
        while (!TI);               // 等特数据传送
        TI = 0;                    // 清除数据传送标志
        i++;                    // 下一个字符
    }
}

void send_str_s()
// 传送字串
{
    unsigned char i = 0;
    while (str_s[i] != '\0')
    {
        SBUF = str_s[i];
        while (!TI);               // 等特数据传送
        TI = 0;                    // 清除数据传送标志
        i++;                    // 下一个字符
    }
}
//前速前进
void  run(void)
{
    Left_moto_go ;   //左电机往前走
    Right_moto_go ;  //右电机往前走
}

//前速后退
void  backrun(void)
{

    Left_moto_back ;   //左电机往后走
    Right_moto_back ;  //右电机往后走
}

//左转
void  leftrun(void)
{
    Left_moto_back ;   //左电机往前走
    Right_moto_go ;  //右电机往前走
}

//右转
void  rightrun(void)
{
    Left_moto_go ;   //左电机往前走
    Right_moto_back ;  //右电机往前走
}
//STOP
void  stoprun(void)
{

    Left_moto_Stop ;   //左电机往前走
    Right_moto_Stop ;  //右电机往前走
}
void sint() interrupt 4      //中断接收3个字节
{
    if (RI)                    //是否接收中断
    {
        RI = 0;
        dat = SBUF;
        if (dat == 'O' && (i == 0)) //接收数据第一帧
        {
            buff[i] = dat;
            flag = 1;      //开始接收数据
        }
        else if (flag == 1)
        {
            i++;
            buff[i] = dat;
            if (i >= 2)
            {
                i = 0;    // 停止接收
                flag = 0;
                flag_REC = 1 ;
            }
        }
    }
}
/*--主函数--*/
void main(void)
{
    delay(10); //启动等待，等LCM讲入工作状态
    LCMInit(); //LCM初始化
    delay(5);//延时片刻
    
    DisplayListChar(0, 0, header);
    DisplayListChar(0, 1, table_s);
    
    TMOD = 0x20;
    TH1 = 0xFd;           //11.0592M晶振，9600波特率
    TL1 = 0xFd;
    SCON = 0x50;
    PCON = 0x00;
    TR1 = 1;
    ES = 1;
    EA = 1;

    while (1)                           /*无限循环*/
    {
        if (flag_REC == 1)
        {
            flag_REC = 0;
            if (buff[0] == 'O' && buff[1] == 'N') //第一个字节为O，第二个字节为N，第三个字节为控制码
                switch (buff[2])
                {
                case forward :                            // 前进
                    send_str_f();
                    run();
                    DisplayListChar(0, 1, clear);
                    DisplayListChar(0, 1, table_f);
                    break;
                case backward:                        // 后退
                    send_str_b();
                    backrun();
                    DisplayListChar(0, 1, clear);
                    DisplayListChar(0, 1, table_b);
                    break;
                case left:                        // 左转
                    send_str_l();
                    leftrun();
                    DisplayListChar(0, 1, clear);
                    DisplayListChar(0, 1, table_l);
                    break;
                case right:                        // 右转
                    send_str_r();
                    rightrun();
                    DisplayListChar(0, 1, clear);
                    DisplayListChar(0, 1, table_r);
                    break;
                case stop:                        // 停止
                    send_str_s();
                    stoprun();
                    DisplayListChar(0, 1, clear);
                    DisplayListChar(0, 1, table_s);
                    break;
                }
        }
    }
}