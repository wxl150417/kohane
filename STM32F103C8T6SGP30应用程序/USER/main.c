/*
1.��Ŀ���ƣ������콢��SGP30ģ��STM32F103C8T6���Գ���
2.��ʾģ�飺���ڷ�������,������9600
3.ʹ�������keil5 for ARM
4.������λ������
5.��Ŀ��ɣ�SGP30ģ��
6.��Ŀ���ܣ����ڷ��ز�����CO2,TVOC����
7.��Ҫԭ������ο�SGP30�����ֲ�
8.�����ַ��https://lssz.tmall.com ���Ա��������������콢�ꡱ
10.��Ȩ�����������콢�����г��������������Ȩ�����뱾���Ʒ���׳��ۣ��벻Ҫ����������׷���䷨�����Σ�
���߶���
	VCC--5V
	GND--GND
	SCL--PB0
	SDA--PB1
*/

#include "sys.h"
#include "delay.h"
#include "SGP30.h"
#include "usart.h"

u32 CO2Data,TVOCData;//����CO2Ũ�ȱ�����TVOCŨ�ȱ���
u32 CO2Data1[100],TVOCData1[100];//����CO2Ũ�ȱ�����TVOCŨ�ȱ���

	
u32 readsgp(void)
{
 u8 i =0;
	u32 sgp30_dat;
	 u32 co2sum=0;
	u32 tvocsum=0;
	 u32 co1;
	 u32 yv1;
//delay_init();
	//uart_init(9600);
	//SGP30_Init();   //��ʼ��SGP30
	delay_ms(100);
	
	SGP30_Write(0x20,0x08);
	sgp30_dat = SGP30_Read();//��ȡSGP30��ֵ
	CO2Data = (sgp30_dat & 0xffff0000) >> 16;
	TVOCData = sgp30_dat & 0x0000ffff;	
	//SGP30ģ�鿪����Ҫһ��ʱ���ʼ�����ڳ�ʼ���׶ζ�ȡ��CO2Ũ��Ϊ400ppm��TVOCΪ0ppd�Һ㶨���䣬����ϵ��ÿ��һ��ʱ���ȡһ��
	//SGP30ģ���ֵ�����CO2Ũ��Ϊ400ppm��TVOCΪ0ppd�����͡����ڼ����...����ֱ��SGP30ģ���ʼ����ɡ�
	//��ʼ����ɺ�տ�ʼ�������ݻᲨ���Ƚϴ�������������һ��ʱ�����������ȶ���
	//�����ഫ�����Ƚ������ܻ���Ӱ�죬�������ݳ��ֲ����������ģ�����������˲�������
	while(CO2Data == 400 && TVOCData == 0)
	{
		SGP30_Write(0x20,0x08);
		sgp30_dat = SGP30_Read();//��ȡSGP30��ֵ
		CO2Data = (sgp30_dat & 0xffff0000) >> 16;//ȡ��CO2Ũ��ֵ
		TVOCData = sgp30_dat & 0x0000ffff;			 //ȡ��TVOCֵ
		printf("���ڼ����...\r\n");
		delay_ms(500);
	}
		for(i=0;i<100;i++)
	{ 
		 CO2Data1[i] = (sgp30_dat & 0xffff0000) >> 16;//ȡ��CO2Ũ��ֵ
		 TVOCData1[i] = sgp30_dat & 0x0000ffff;       //ȡ��TVOCֵ
		 co2sum+=CO2Data1[i];
		 tvocsum+=TVOCData1[i];
	 }		
		co1 =(unsigned int)co2sum/100;
		yv1 =(unsigned int)tvocsum/100;
		printf("CO2:%d ppm \r\n TVOC:%d ppd\r\n",co1,yv1);
		delay_ms(500);
		return co1;
	}

int main(void)
{
  delay_init();
	uart_init(9600);
	SGP30_Init();   //��ʼ��SGP30
	delay_ms(100);
	while(1)
	{
	 u32 CO = readsgp();
	 if (CO>500)
	 {
	   printf("����ס\r\n");
	
	
	
	}
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
}
}