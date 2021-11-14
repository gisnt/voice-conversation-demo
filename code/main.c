/******************************************************
**  工程名称：UNV-01语音识别模块驱动程序
**	CPU: STC11L08XE
**	晶振：22.1184MHZ
**	波特率：9600 bit/S
**	配套产品信息：UNV-LD3320+STC11语音识别开发板
**  说明：口令模式： 即每次识别时都需要说“阿拉神灯”这个口令 ，才能够进行下一级的识别
***************************/
#include "config.h"
/************************************************************************************/
//	nAsrStatus 用来在main主程序中表示程序运行的状态，不是LD3320芯片内部的状态寄存器
//	LD_ASR_NONE:		表示没有在作ASR识别
//	LD_ASR_RUNING：		表示LD3320正在作ASR识别中
//	LD_ASR_FOUNDOK:		表示一次识别流程结束后，有一个识别结果
//	LD_ASR_FOUNDZERO:	表示一次识别流程结束后，没有识别结果
//	LD_ASR_ERROR:		表示一次识别流程中LD3320芯片内部出现不正确的状态
/***********************************************************************************/
uint8 idata nAsrStatus=0;	
void MCU_init(); 
void ProcessInt0(); //识别处理函数
void delay(unsigned long uldata);
void User_handle(uint8 dat);//用户执行操作函数
void Delay200ms();
void Led_test(void);//单片机工作指示
uint8_t G0_flag=DISABLE;//运行标志，ENABLE:运行。DISABLE:禁止运行 
sbit LED=P4^2;//信号指示灯
sbit P10=P1^0;
sbit P11=P1^1;
sbit P12=P1^2;
sbit P13=P1^3;
sbit P14=P1^4;
sbit P15=P1^5;
sbit P16=P1^6;
sbit P17=P1^7;
/***********************************************************
* 名    称： void  main(void)
* 功    能： 主函数	程序入口
* 入口参数：  
* 出口参数：
* 说    明： 					 
* 调用方法： 
**********************************************************/ 
void  main(void)
{
	uint8 idata nAsrRes;
	uint8 i=0;
	
	MCU_init();
	LD_Reset();
	UartIni(); /*串口初始化*/
	nAsrStatus = LD_ASR_NONE;		//	初始状态：没有在作ASR
	#ifdef TEST	
	SYN_FrameInfo(2, "[v5][m5][t4]主人你好，我回来了。");
	
	Led_test();
	#endif

	while(1)
	{
		switch(nAsrStatus)
		{
			case LD_ASR_RUNING:
			case LD_ASR_ERROR:		
				break;
			case LD_ASR_NONE:
			{
				nAsrStatus=LD_ASR_RUNING;
				if (RunASR()==0)	/*	启动一次ASR识别流程：ASR初始化，ASR添加关键词语，启动ASR运算*/
				{
					nAsrStatus = LD_ASR_ERROR;
				}
				break;
			}
			case LD_ASR_FOUNDOK: /*	一次ASR识别流程结束，去取ASR识别结果*/
			{				
				nAsrRes = LD_GetResult();		/*获取结果*/
				User_handle(nAsrRes);//用户执行函数 
				nAsrStatus = LD_ASR_NONE;
				break;
			}
			case LD_ASR_FOUNDZERO:
			default:
			{
				nAsrStatus = LD_ASR_NONE;
				break;
			}
		}// switch	 			
	}// while

}
/***********************************************************
* 名    称： 	 LED灯测试
* 功    能： 单片机是否工作指示
* 入口参数： 无 
* 出口参数：无
* 说    明： 					 
**********************************************************/
void Led_test(void)
{
	Delay200ms();
	Delay200ms();
	LED=~ LED;
	Delay200ms();
	LED=~ LED;
	Delay200ms();
	LED=~ LED;
	Delay200ms();
	LED=~ LED;
}
/***********************************************************
* 名    称： void MCU_init()
* 功    能： 单片机初始化
* 入口参数：  
* 出口参数：
* 说    明： 					 
* 调用方法： 
**********************************************************/ 
void MCU_init()
{
	P0 = 0xff;
	P1 = 0xff;
	P2 = 0xff;
	P3 = 0xff;
	P4 = 0xff;
	P1M1&=0X00;
	P1M0|=0XFF;
	P10=0;P11=0;P12=0;P13=0;
	LD_MODE = 0;		//	设置MD管脚为低，并行模式读写
	IE0=1;
	EX0=1;
	EA=1;
}
/***********************************************************
* 名    称：	延时函数
* 功    能：
* 入口参数：  
* 出口参数：
* 说    明： 					 
* 调用方法： 
**********************************************************/ 
void Delay200us()		//@22.1184MHz
{
	unsigned char i, j;
	_nop_();
	_nop_();
	i = 5;
	j = 73;
	do
	{
		while (--j);
	} while (--i);
}

void  delay(unsigned long uldata)
{
	unsigned int j  =  0;
	unsigned int g  =  0;
	while(uldata--)
	Delay200us();
}

void Delay200ms()		//@22.1184MHz
{
	unsigned char i, j, k;

	i = 17;
	j = 208;
	k = 27;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

/***********************************************************
* 名    称： 中断处理函数
* 功    能：
* 入口参数：  
* 出口参数：
* 说    明： 					 
* 调用方法： 
**********************************************************/ 
void ExtInt0Handler(void) interrupt 0  
{ 	
	ProcessInt0();				
}
/***********************************************************
* 名    称：用户执行函数 
* 功    能：识别成功后，执行动作可在此进行修改 
* 入口参数： 无 
* 出口参数：无
* 说    明： 					 
**********************************************************/
void 	User_handle(uint8 dat)
{
		PrintCom("\r\nCode: ");
     	//PrintCom(hex(dat));
		UARTSendByte(48+dat);//串口识别码（十六进制）
		 if(0==dat)
		 {
		  	G0_flag=ENABLE;
			LED=0;
			//SYN_FrameInfo(0, "[v5][m5][t4] 你好主人");
		 }
		 else if(ENABLE==G0_flag)
		 {	
	 		G0_flag=DISABLE;
			LED=1;
			//PrintCom("\nRecogn code: %i.", dat);
			//SYN_FrameInfo(0, "[v5][m5][t4] 好的");
			// v 前景播放音量0-16，m 背景音乐音量0-16，t 词语语速0-5
			switch(dat)		   /*对结果执行相关操作,客户可删除Printcom 串口输出语句替换为其他需要控制的代码*/
			  {
				  case CODE_LUNCH_TIME:			
            			P10=1;						
					  	SYN_FrameInfo(0, "[v5][m5][t4][o0] 好的，已为你准备好了丰盛的午餐，请主人用餐。");
						break;
					case CODE_TODAY_WEATHER:	
						P11=1;
						SYN_FrameInfo(0, "[v5][m5][t4] 好的，今天晴，温度25度，风力一二级，适合主人外出打猎。");
						break;
					case CODE_TELL_STORY:			
            			P12=1;						
						SYN_FrameInfo(0, "[v5][m5][t4] 好的，很久以前，在波斯一个城市里住着两兄弟，老大叫卡希穆，老二叫阿里巴巴。卡希穆非常富有，阿里巴巴非常贫困。");
						break;					
					case CODE_PLAY_MUSIC:			
            			P12=1;						
						SYN_FrameInfo(0, "[v5][m10][x1]ringa");
						break;					
					case CODE_BYE:			
						P13=1;
					  	SYN_FrameInfo(0, "[v5][m5][t4] 好的主人，晚安。");
						break;
					default:
						SYN_FrameInfo(0, "[v5][m5][t4] 主人，我没有听清，请重复指令");
						PrintCom("\r\nUnkown command, please try again.");	
						break;
				}	
			}	
			else 	
			{
				SYN_FrameInfo(0, "[v5][m5][t4] 请说神灯或阿拉神灯唤醒我。");
				PrintCom("\r\nPlease wake me first.");	
			}
}	 
