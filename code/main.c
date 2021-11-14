/******************************************************
**  �������ƣ�UNV-01����ʶ��ģ����������
**	CPU: STC11L08XE
**	����22.1184MHZ
**	�����ʣ�9600 bit/S
**	���ײ�Ʒ��Ϣ��UNV-LD3320+STC11����ʶ�𿪷���
**  ˵��������ģʽ�� ��ÿ��ʶ��ʱ����Ҫ˵��������ơ�������� �����ܹ�������һ����ʶ��
***************************/
#include "config.h"
/************************************************************************************/
//	nAsrStatus ������main�������б�ʾ�������е�״̬������LD3320оƬ�ڲ���״̬�Ĵ���
//	LD_ASR_NONE:		��ʾû������ASRʶ��
//	LD_ASR_RUNING��		��ʾLD3320������ASRʶ����
//	LD_ASR_FOUNDOK:		��ʾһ��ʶ�����̽�������һ��ʶ����
//	LD_ASR_FOUNDZERO:	��ʾһ��ʶ�����̽�����û��ʶ����
//	LD_ASR_ERROR:		��ʾһ��ʶ��������LD3320оƬ�ڲ����ֲ���ȷ��״̬
/***********************************************************************************/
uint8 idata nAsrStatus=0;	
void MCU_init(); 
void ProcessInt0(); //ʶ������
void delay(unsigned long uldata);
void User_handle(uint8 dat);//�û�ִ�в�������
void Delay200ms();
void Led_test(void);//��Ƭ������ָʾ
uint8_t G0_flag=DISABLE;//���б�־��ENABLE:���С�DISABLE:��ֹ���� 
sbit LED=P4^2;//�ź�ָʾ��
sbit P10=P1^0;
sbit P11=P1^1;
sbit P12=P1^2;
sbit P13=P1^3;
sbit P14=P1^4;
sbit P15=P1^5;
sbit P16=P1^6;
sbit P17=P1^7;
/***********************************************************
* ��    �ƣ� void  main(void)
* ��    �ܣ� ������	�������
* ��ڲ�����  
* ���ڲ�����
* ˵    ���� 					 
* ���÷����� 
**********************************************************/ 
void  main(void)
{
	uint8 idata nAsrRes;
	uint8 i=0;
	
	MCU_init();
	LD_Reset();
	UartIni(); /*���ڳ�ʼ��*/
	nAsrStatus = LD_ASR_NONE;		//	��ʼ״̬��û������ASR
	#ifdef TEST	
	SYN_FrameInfo(2, "[v5][m5][t4]������ã��һ����ˡ�");
	
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
				if (RunASR()==0)	/*	����һ��ASRʶ�����̣�ASR��ʼ����ASR��ӹؼ��������ASR����*/
				{
					nAsrStatus = LD_ASR_ERROR;
				}
				break;
			}
			case LD_ASR_FOUNDOK: /*	һ��ASRʶ�����̽�����ȥȡASRʶ����*/
			{				
				nAsrRes = LD_GetResult();		/*��ȡ���*/
				User_handle(nAsrRes);//�û�ִ�к��� 
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
* ��    �ƣ� 	 LED�Ʋ���
* ��    �ܣ� ��Ƭ���Ƿ���ָʾ
* ��ڲ����� �� 
* ���ڲ�������
* ˵    ���� 					 
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
* ��    �ƣ� void MCU_init()
* ��    �ܣ� ��Ƭ����ʼ��
* ��ڲ�����  
* ���ڲ�����
* ˵    ���� 					 
* ���÷����� 
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
	LD_MODE = 0;		//	����MD�ܽ�Ϊ�ͣ�����ģʽ��д
	IE0=1;
	EX0=1;
	EA=1;
}
/***********************************************************
* ��    �ƣ�	��ʱ����
* ��    �ܣ�
* ��ڲ�����  
* ���ڲ�����
* ˵    ���� 					 
* ���÷����� 
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
* ��    �ƣ� �жϴ�����
* ��    �ܣ�
* ��ڲ�����  
* ���ڲ�����
* ˵    ���� 					 
* ���÷����� 
**********************************************************/ 
void ExtInt0Handler(void) interrupt 0  
{ 	
	ProcessInt0();				
}
/***********************************************************
* ��    �ƣ��û�ִ�к��� 
* ��    �ܣ�ʶ��ɹ���ִ�ж������ڴ˽����޸� 
* ��ڲ����� �� 
* ���ڲ�������
* ˵    ���� 					 
**********************************************************/
void 	User_handle(uint8 dat)
{
		PrintCom("\r\nCode: ");
     	//PrintCom(hex(dat));
		UARTSendByte(48+dat);//����ʶ���루ʮ�����ƣ�
		 if(0==dat)
		 {
		  	G0_flag=ENABLE;
			LED=0;
			//SYN_FrameInfo(0, "[v5][m5][t4] �������");
		 }
		 else if(ENABLE==G0_flag)
		 {	
	 		G0_flag=DISABLE;
			LED=1;
			//PrintCom("\nRecogn code: %i.", dat);
			//SYN_FrameInfo(0, "[v5][m5][t4] �õ�");
			// v ǰ����������0-16��m ������������0-16��t ��������0-5
			switch(dat)		   /*�Խ��ִ����ز���,�ͻ���ɾ��Printcom �����������滻Ϊ������Ҫ���ƵĴ���*/
			  {
				  case CODE_LUNCH_TIME:			
            			P10=1;						
					  	SYN_FrameInfo(0, "[v5][m5][t4][o0] �õģ���Ϊ��׼�����˷�ʢ����ͣ��������ò͡�");
						break;
					case CODE_TODAY_WEATHER:	
						P11=1;
						SYN_FrameInfo(0, "[v5][m5][t4] �õģ������磬�¶�25�ȣ�����һ�������ʺ�����������ԡ�");
						break;
					case CODE_TELL_STORY:			
            			P12=1;						
						SYN_FrameInfo(0, "[v5][m5][t4] �õģ��ܾ���ǰ���ڲ�˹һ��������ס�����ֵܣ��ϴ�п�ϣ�£��϶��а���Ͱ͡���ϣ�·ǳ����У�����Ͱͷǳ�ƶ����");
						break;					
					case CODE_PLAY_MUSIC:			
            			P12=1;						
						SYN_FrameInfo(0, "[v5][m10][x1]ringa");
						break;					
					case CODE_BYE:			
						P13=1;
					  	SYN_FrameInfo(0, "[v5][m5][t4] �õ����ˣ�����");
						break;
					default:
						SYN_FrameInfo(0, "[v5][m5][t4] ���ˣ���û�����壬���ظ�ָ��");
						PrintCom("\r\nUnkown command, please try again.");	
						break;
				}	
			}	
			else 	
			{
				SYN_FrameInfo(0, "[v5][m5][t4] ��˵��ƻ�����ƻ����ҡ�");
				PrintCom("\r\nPlease wake me first.");	
			}
}	 
