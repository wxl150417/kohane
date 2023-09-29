/*
1.项目名称：绿深旗舰店SGP30模块STM32F103C8T6测试程序
2.显示模块：串口返回数据,波特率9600
3.使用软件：keil5 for ARM
4.配套上位机：无
5.项目组成：SGP30模块
6.项目功能：串口返回测量的CO2,TVOC数据
7.主要原理：具体参考SGP30数据手册
8.购买地址：https://lssz.tmall.com 或淘宝上搜索“绿深旗舰店”
10.版权声明：绿深旗舰店所有程序都申请软件著作权。均与本店产品配套出售，请不要传播，以免追究其法律责任！
接线定义
	VCC--5V
	GND--GND
	SCL--PB0
	SDA--PB1
*/

#include "sys.h"
#include "delay.h"
#include "SGP30.h"
#include "usart.h"

u32 CO2Data,TVOCData;//定义CO2浓度变量与TVOC浓度变量
u32 CO2Data1[100],TVOCData1[100];//定义CO2浓度变量与TVOC浓度变量

	
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
	//SGP30_Init();   //初始化SGP30
	delay_ms(100);
	
	SGP30_Write(0x20,0x08);
	sgp30_dat = SGP30_Read();//读取SGP30的值
	CO2Data = (sgp30_dat & 0xffff0000) >> 16;
	TVOCData = sgp30_dat & 0x0000ffff;	
	//SGP30模块开机需要一定时间初始化，在初始化阶段读取的CO2浓度为400ppm，TVOC为0ppd且恒定不变，因此上电后每隔一段时间读取一次
	//SGP30模块的值，如果CO2浓度为400ppm，TVOC为0ppd，发送“正在检测中...”，直到SGP30模块初始化完成。
	//初始化完成后刚开始读出数据会波动比较大，属于正常现象，一段时间后会逐渐趋于稳定。
	//气体类传感器比较容易受环境影响，测量数据出现波动是正常的，可自行添加滤波函数。
	while(CO2Data == 400 && TVOCData == 0)
	{
		SGP30_Write(0x20,0x08);
		sgp30_dat = SGP30_Read();//读取SGP30的值
		CO2Data = (sgp30_dat & 0xffff0000) >> 16;//取出CO2浓度值
		TVOCData = sgp30_dat & 0x0000ffff;			 //取出TVOC值
		printf("正在检测中...\r\n");
		delay_ms(500);
	}
		for(i=0;i<100;i++)
	{ 
		 CO2Data1[i] = (sgp30_dat & 0xffff0000) >> 16;//取出CO2浓度值
		 TVOCData1[i] = sgp30_dat & 0x0000ffff;       //取出TVOC值
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
	SGP30_Init();   //初始化SGP30
	delay_ms(100);
	while(1)
	{
	 u32 CO = readsgp();
	 if (CO>500)
	 {
	   printf("你是住\r\n");
	
	
	
	}
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
}
}