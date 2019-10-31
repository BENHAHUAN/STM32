#include "bsp.h"

uint8_t g_spi_busy = 0;		/* SPI ���߹����־ */

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitSPI
*	����˵��: ����SPI���ߡ� ֻ���� SCK�� MOSI�� MISO���ߵ����á�������ƬѡCS��Ҳ����������оƬ���е�INT��BUSY��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitSPI(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	//ʹ��SPIʱ��
    RCC_APB2PeriphClockCmd(SPI_CLK, ENABLE);
    //ʹ��GPIOʱ��
	SPI_GPIO_APBxClock_Cmd(SPI_SCK_CLK | SPI_MISO_CLK | SPI_MOSI_CLK, ENABLE);
      /* ����SCK����*/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(SPI_SCK_PORT, &GPIO_InitStructure);

    /* ����MISO����*/
    GPIO_InitStructure.GPIO_Pin = SPI_MISO_PIN;
    GPIO_Init(SPI_MISO_PORT, &GPIO_InitStructure);

    /* ����MOSI����*/
    GPIO_InitStructure.GPIO_Pin = SPI_MOSI_PIN;
    GPIO_Init(SPI_MOSI_PORT, &GPIO_InitStructure);
    
    /* SPI ģʽ���� */
	/*
	����SPIģʽ0��ģʽ3
	SPIģʽ		����ʱCLK��ƽ		������Ե
		0							��				������Ե
		1							��				ż����Ե
		2							��				������Ե
		3							��				ż����Ե
	*/
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//˫��ȫ˫��ģʽ
    SPI_InitStructure.SPI_Mode      = SPI_Mode_Master;		            //SPI����ģʽ
    SPI_InitStructure.SPI_DataSize  = SPI_DataSize_8b;		            //8λ����֡����
    SPI_InitStructure.SPI_CPOL      = SPI_CPOL_Low;		            //ʱ�Ӽ���CPOL��SPI����ʱCLK��ƽ�����ߵ�ƽ
    SPI_InitStructure.SPI_CPHA      = SPI_CPHA_1Edge;	                //ʱ����λCPHA����/ż����Ե��������ż����Ե
    SPI_InitStructure.SPI_NSS       = SPI_NSS_Soft;		                //�������CS��
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;	//SPIʱ�ӷ�Ƶ 4.5M
    SPI_InitStructure.SPI_FirstBit  = SPI_FirstBit_MSB;	                //��λ����
    //SPI_InitStructure.SPI_CRCPolynomial = 7;                          //CRCУ����ʽ��ͨ�����ã�����
    SPI_Init(SPI1 , &SPI_InitStructure);

    SPI_Cmd(SPI1, DISABLE);			/* �Ƚ�ֹSPI   */
    
    SPI_Cmd(SPI1 , ENABLE);         /* ��ʹ�� SPI  */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_spiWrite0
*	����˵��: ��SPI���߷���һ���ֽڡ�SCK�����زɼ�����, SCK����ʱΪ�͵�ƽ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_spiWrite0(uint8_t _ucByte)
{
    	/* �ȴ����ͻ������� */
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	/* ����һ���ֽ� */
	SPI_I2S_SendData(SPI1, _ucByte);

	/* �ȴ����ݽ������ */
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	/* ��ȡ���յ������� */
	SPI_I2S_ReceiveData(SPI1);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_spiRead0
*	����˵��: ��SPI���߽���8��bit���ݡ� SCK�����زɼ�����, SCK����ʱΪ�͵�ƽ��
*	��    ��: ��
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t bsp_spiRead0(void)
{
   	uint8_t read;

	/* �ȴ����ͻ������� */
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	/* ����һ���ֽ� */
	SPI_I2S_SendData(SPI1, 0);

	/* �ȴ����ݽ������ */
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	/* ��ȡ���յ������� */
	read = SPI_I2S_ReceiveData(SPI1);

	/* ���ض��������� */
	return read; 
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_spiWrite1
*	����˵��: ��SPI���߷���һ���ֽڡ�  SCK�����زɼ�����, SCK����ʱΪ�ߵ�ƽ
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_spiWrite1(uint8_t _ucByte)
{
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	/* ����һ���ֽ� */
	SPI_I2S_SendData(SPI1, _ucByte);

	/* �ȴ����ݽ������ */
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	/* ��ȡ���յ������� */
	SPI_I2S_ReceiveData(SPI1);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_spiRead1
*	����˵��: ��SPI���߽���8��bit���ݡ�  SCK�����زɼ�����, SCK����ʱΪ�ߵ�ƽ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t bsp_spiRead1(void)
{
    	uint8_t read;

	/* �ȴ����ͻ������� */
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	/* ����һ���ֽ� */
	SPI_I2S_SendData(SPI1, 0);

	/* �ȴ����ݽ������ */
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	/* ��ȡ���յ������� */
	read = SPI_I2S_ReceiveData(SPI1);

	/* ���ض��������� */
	return read;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SpiBusEnter
*	����˵��: ռ��SPI����
*	��    ��: ��
*	�� �� ֵ: 0 ��ʾ��æ  1��ʾæ
*********************************************************************************************************
*/
void bsp_SpiBusEnter(void)
{
	g_spi_busy = 1;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SpiBusExit
*	����˵��: �ͷ�ռ�õ�SPI����
*	��    ��: ��
*	�� �� ֵ: 0 ��ʾ��æ  1��ʾæ
*********************************************************************************************************
*/
void bsp_SpiBusExit(void)
{
	g_spi_busy = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SpiBusBusy
*	����˵��: �ж�SPI����æ�������Ǽ������SPIоƬ��Ƭѡ�ź��Ƿ�Ϊ1
*	��    ��: ��
*	�� �� ֵ: 0 ��ʾ��æ  1��ʾæ
*********************************************************************************************************
*/
uint8_t bsp_SpiBusBusy(void)
{
	return g_spi_busy;
}







