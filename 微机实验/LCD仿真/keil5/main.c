//ƽ̨��HJ-4WD/HJ-E/HL-1����С�� + Keil U4 + STC89C52
//����:11.0592MHZ

#include<AT89x52.H>
#include <intrins.h>   //����nop��ϵͳ����

//HL-1С���������߶���
#define Left_moto_go      {P1_2=1,P1_3=0;}    //��ߵ����ǰ��
#define Left_moto_back    {P1_2=0,P1_3=1;}    //��ߵ�����ת
#define Left_moto_Stop    {P1_2=0,P1_3=0;}    //��ߵ��ͣת
#define Right_moto_go     {P1_6=1,P1_7=0;}    //�ұߵ����ǰ��
#define Right_moto_back   {P1_6=0,P1_7=1;}    //�ұߵ�������
#define Right_moto_Stop   {P1_6=0,P1_7=0;}    //�ұߵ��ͣת

#define left     'L'//Left
#define right    'R'//Right
#define forward       'F'//Forward
#define backward     'B'//Backward
#define stop     'S'//Stop

#define LCM_Data  P0   //����Һ�������ݿ�
#define Busy    0x80   //���ڼ��LCM״̬���е�Busy��ʶ


sbit LCM_RW = P1 ^ 1 ; //����LCD����
sbit LCM_RS = P1 ^ 0 ;
sbit LCM_E = P2 ^ 5  ;

char code str_f[] =  "�յ�ָ���ǰ!\n";
char code str_b[] = "�յ�ָ����!\n";
char code str_l[] = "�յ�ָ�����!\n";
char code str_r[] = "�յ�ָ�����!\n";
char code str_s[] = "�յ�ָ�ֹͣ!\n";

unsigned char code header[] = "=SportSituation="; //LCD1602��ʾ��ʽ
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
unsigned char  buff[5] = 0; //���ջ����ֽ�


//��ʱ����
void delay(unsigned int k)
{
    unsigned int x, y;
    for (x = 0; x < k; x++)
        for (y = 0; y < 2000; y++);
}

void Delay10us(unsigned char i)     //10us��ʱ���� ����������ģ��ʱʹ��
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

/************************************LCD1602Һ������������************************************************/
//*******************��״̬*************************//
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
    while (LCM_Data & Busy); //���æ�ź�
    return (LCM_Data);
}

/****************д����************************/
void WriteDataLCM(unsigned char WDLCM)
{
    ReadStatusLCM(); //���æ
    LCM_Data = WDLCM;
    LCM_RS = 1;
    Delay10us(1);
    LCM_RW = 0;
    Delay10us(1);
    LCM_E = 0; //�������ٶ�̫�߿���������С����ʱ
    Delay10us(1);
    LCM_E = 0; //��ʱ
    Delay10us(1);
    LCM_E = 1;
    Delay10us(1);
}

//****************дָ��*************************//
void WriteCommandLCM(unsigned char WCLCM, BuysC) //BuysCΪ0ʱ����æ���
{
    if (BuysC) ReadStatusLCM(); //������Ҫ���æ
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



//*******************LCM��ʼ��**********************//
void LCMInit(void)
{
    LCM_Data = 0;
    WriteCommandLCM(0x38, 0); //������ʾģʽ���ã������æ�ź�
    delay(5);
    WriteCommandLCM(0x38, 0);
    delay(5);
    WriteCommandLCM(0x38, 0);
    delay(5);

    WriteCommandLCM(0x38, 1); //��ʾģʽ����,��ʼҪ��ÿ�μ��æ�ź�
    WriteCommandLCM(0x08, 1); //�ر���ʾ
    WriteCommandLCM(0x01, 1); //��ʾ����
    WriteCommandLCM(0x06, 1); // ��ʾ����ƶ�����
    WriteCommandLCM(0x0c, 1); // ��ʾ�����������
}

//*********************��ָ��λ����ʾһ���ַ�***********************//
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
    Y &= 0x1;
    X &= 0xF; //����X���ܴ���15��Y���ܴ���1
    if (Y) X |= 0x40; //��Ҫ��ʾ�ڶ���ʱ��ַ��+0x40;
    X |= 0x80; //���ָ����
    WriteCommandLCM(X, 1); //��������
    WriteDataLCM(DData); //������
}

//**********************��ָ��λ����ʾһ���ַ�*************************//
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData)
{
    unsigned char ListLength;

    ListLength = 0;
    Y &= 0x1;
    X &= 0xF; //����X���ܴ���15��Y���ܴ���1
    while (DData[ListLength] > 0x19) //�������ִ�β���˳�
    {
        if (X <= 0xF) //X����ӦС��0xF
        {
            DisplayOneChar(X, Y, DData[ListLength]); //��ʾ�����ַ�
            ListLength++;
            X++;
        }
    }
}


/***************************************************************************/

//�ַ������ͺ���
void send_str_f()
// �����ִ�
{
    unsigned char i = 0;
    while (str_f[i] != '\0')
    {
        SBUF = str_f[i];
        while (!TI);               // �������ݴ���
        TI = 0;                    // ������ݴ��ͱ�־
        i++;                    // ��һ���ַ�
    }
}

void send_str_b()
// �����ִ�
{
    unsigned char i = 0;
    while (str_b[i] != '\0')
    {
        SBUF = str_b[i];
        while (!TI);               // �������ݴ���
        TI = 0;                    // ������ݴ��ͱ�־
        i++;                    // ��һ���ַ�
    }
}

void send_str_l()
// �����ִ�
{
    unsigned char i = 0;
    while (str_l[i] != '\0')
    {
        SBUF = str_l[i];
        while (!TI);               // �������ݴ���
        TI = 0;                    // ������ݴ��ͱ�־
        i++;                    // ��һ���ַ�
    }
}

void send_str_r()
// �����ִ�
{
    unsigned char i = 0;
    while (str_r[i] != '\0')
    {
        SBUF = str_r[i];
        while (!TI);               // �������ݴ���
        TI = 0;                    // ������ݴ��ͱ�־
        i++;                    // ��һ���ַ�
    }
}

void send_str_s()
// �����ִ�
{
    unsigned char i = 0;
    while (str_s[i] != '\0')
    {
        SBUF = str_s[i];
        while (!TI);               // �������ݴ���
        TI = 0;                    // ������ݴ��ͱ�־
        i++;                    // ��һ���ַ�
    }
}
//ǰ��ǰ��
void  run(void)
{
    Left_moto_go ;   //������ǰ��
    Right_moto_go ;  //�ҵ����ǰ��
}

//ǰ�ٺ���
void  backrun(void)
{

    Left_moto_back ;   //����������
    Right_moto_back ;  //�ҵ��������
}

//��ת
void  leftrun(void)
{
    Left_moto_back ;   //������ǰ��
    Right_moto_go ;  //�ҵ����ǰ��
}

//��ת
void  rightrun(void)
{
    Left_moto_go ;   //������ǰ��
    Right_moto_back ;  //�ҵ����ǰ��
}
//STOP
void  stoprun(void)
{

    Left_moto_Stop ;   //������ǰ��
    Right_moto_Stop ;  //�ҵ����ǰ��
}
void sint() interrupt 4      //�жϽ���3���ֽ�
{
    if (RI)                    //�Ƿ�����ж�
    {
        RI = 0;
        dat = SBUF;
        if (dat == 'O' && (i == 0)) //�������ݵ�һ֡
        {
            buff[i] = dat;
            flag = 1;      //��ʼ��������
        }
        else if (flag == 1)
        {
            i++;
            buff[i] = dat;
            if (i >= 2)
            {
                i = 0;    // ֹͣ����
                flag = 0;
                flag_REC = 1 ;
            }
        }
    }
}
/*--������--*/
void main(void)
{
    delay(10); //�����ȴ�����LCM���빤��״̬
    LCMInit(); //LCM��ʼ��
    delay(5);//��ʱƬ��
    
    DisplayListChar(0, 0, header);
    DisplayListChar(0, 1, table_s);
    
    TMOD = 0x20;
    TH1 = 0xFd;           //11.0592M����9600������
    TL1 = 0xFd;
    SCON = 0x50;
    PCON = 0x00;
    TR1 = 1;
    ES = 1;
    EA = 1;

    while (1)                           /*����ѭ��*/
    {
        if (flag_REC == 1)
        {
            flag_REC = 0;
            if (buff[0] == 'O' && buff[1] == 'N') //��һ���ֽ�ΪO���ڶ����ֽ�ΪN���������ֽ�Ϊ������
                switch (buff[2])
                {
                case forward :                            // ǰ��
                    send_str_f();
                    run();
                    DisplayListChar(0, 1, clear);
                    DisplayListChar(0, 1, table_f);
                    break;
                case backward:                        // ����
                    send_str_b();
                    backrun();
                    DisplayListChar(0, 1, clear);
                    DisplayListChar(0, 1, table_b);
                    break;
                case left:                        // ��ת
                    send_str_l();
                    leftrun();
                    DisplayListChar(0, 1, clear);
                    DisplayListChar(0, 1, table_l);
                    break;
                case right:                        // ��ת
                    send_str_r();
                    rightrun();
                    DisplayListChar(0, 1, clear);
                    DisplayListChar(0, 1, table_r);
                    break;
                case stop:                        // ֹͣ
                    send_str_s();
                    stoprun();
                    DisplayListChar(0, 1, clear);
                    DisplayListChar(0, 1, table_s);
                    break;
                }
        }
    }
}