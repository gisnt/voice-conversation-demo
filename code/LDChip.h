/*************************************************
**  �������ƣ�UNV-LD3320+STC11����ʶ��ģ����������
**	CPU: STC11
**	����22.1184MHZ
**	�����ʣ�9600 bit/S
**	���ײ�Ʒ��Ϣ��UNV-LD3320+STC11����ʶ�𿪷���
**  ˵��������ģʽ�� ��ÿ��ʶ��ʱ����Ҫ˵��Сӥ��������� �����ܹ�������һ����ʶ��
/****************************************************/


#ifndef LD_CHIP_H
#define LD_CHIP_H

#define uint8 unsigned char
#define uint16 unsigned int
#define uint32 unsigned long

//	�������״̬����������¼������������ASRʶ������е��ĸ�״̬
#define LD_ASR_NONE				0x00	 /*	��ʾû������ASRʶ��*/
#define LD_ASR_RUNING			0x01	/*	��ʾLD3320������ASRʶ����*/
#define LD_ASR_FOUNDOK			0x10	/*��ʾһ��ʶ�����̽�������һ��ʶ����*/
#define LD_ASR_FOUNDZERO 		0x11	/*��ʾһ��ʶ�����̽�����û��ʶ����*/
#define LD_ASR_ERROR	 		0x31	/*��ʾһ��ʶ��������LD3320оƬ�ڲ����ֲ���ȷ��״̬*/


#define CLK_IN   		22.1184	/* �û�ע���޸�����ľ���ʱ�Ӵ�С */
#define LD_PLL_11			(uint8)((CLK_IN/2.0)-1)
#define LD_PLL_MP3_19		0x0f
#define LD_PLL_MP3_1B		0x18
#define LD_PLL_MP3_1D   	(uint8)(((90.0*((LD_PLL_11)+1))/(CLK_IN))-1)

#define LD_PLL_ASR_19 		(uint8)(CLK_IN*32.0/(LD_PLL_11+1) - 0.51)
#define LD_PLL_ASR_1B 		0x48
#define LD_PLL_ASR_1D 		0x1f

//��������
void LD_Reset();
void LD_Init_Common();
void LD_Init_ASR();
uint8 RunASR(void);
void LD_AsrStart();
uint8 LD_AsrRun();
uint8 LD_AsrAddFixed();
uint8 LD_GetResult();


//ʶ����ͻ��޸Ĵ� 
#define CODE_CMD  0x00   			// ���Ѵʣ���������0x00�û����ɽ����޸ġ�
#define CODE_LUNCH_TIME 0x01		// ���
#define CODE_TODAY_WEATHER	0x02	// ��������	
#define CODE_TELL_STORY 0x03		// ����
#define CODE_PLAY_MUSIC 0x04		// ����	
#define CODE_IGNORE 0x05			// ����������������ʶ������
#define CODE_BYE 0x10				// �ټ�

//��ֵԽ��Խ����ʶ�����ԽԶ������ʶ���ʾ�Խ�� �����Լ���ʵ��������ڡ�
#define MIC_VOL 0x55	 //��ͷ���棨�����ȵ��ڣ� ��Χ��00-7f 
#endif