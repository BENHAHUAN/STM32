#include "includes.h"



/*
*********************************************************************************************************
*					                      �궨��
*********************************************************************************************************
*/
#define SAMP_COUNT	10		/* ������������ʾ200ms�ڵĲ���������ƽ��ֵ */

uint16_t g_usAdcValue;	/* ADC ����ֵ��ƽ��ֵ */


/*
*********************************************************************************************************
*	�� �� ��: ADC_Configuration
*	����˵��: ����ADC, PA0��ΪADCͨ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ADC_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	__IO uint16_t ADCConvertedValue;

    /* ʹ�� ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);

	/* ����PA0Ϊģ������(ADC Channel14) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ����ADC1, ����DMA, ��������� */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ����ADC1 ����ͨ��14 channel14 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_55Cycles5);

	/* ʹ��ADC1 DMA���� */
	ADC_DMACmd(ADC1, ENABLE);

	/* ʹ�� ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* ʹ��ADC1 ��λУ׼�Ĵ��� */
	ADC_ResetCalibration(ADC1);
	/* ���ADC1�ĸ�λ�Ĵ��� */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* ����ADC1У׼ */
	ADC_StartCalibration(ADC1);
	/* ���У׼�Ƿ���� */
	while(ADC_GetCalibrationStatus(ADC1));

	/* �������ADCת�� */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void select_sort(uint16_t *a,int n)
{
    register int i,j,min,t;
    for(i = 0;i < n-1;i++)
    {
        min = i;//������Сֵ
        for(j = i + 1;j < n;j++)
            if(a[min] > a[j])
                min = j;//����
        if(min != i)
        {
            t = a[min];
            a[min] = a[i];
            a[i] = t;
        }
    }
}
/*
*********************************************************************************************************
*	�� �� ��: ADC_GetSampleAvgN
*	����˵��: ������ȡ40��ADCֵ��ƽ��
*	��    ��: ��      	
*	�� �� ֵ: ���ض�ȡ��ADCֵ
*********************************************************************************************************
*/

uint16_t ADC_GetSampleAvgN(void)
{
	static uint16_t buf[SAMP_COUNT];
	static uint8_t write;
	uint32_t sum;
	uint8_t i;

	buf[write] = ADC_GetConversionValue(ADC1);
	if (++write >= SAMP_COUNT)
	{
		write = 0;
	}
		select_sort(buf,SAMP_COUNT);
	sum = 0;
	for (i = 2; i < SAMP_COUNT-2; i++)
	{
		sum += buf[i];
	}
	g_usAdcValue = ADC_GetConversionValue(ADC1);
	//g_usAdcValue = sum / (SAMP_COUNT-4);	/* ADC����ֵ�����ɴβ���ֵƽ�� */

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	/* ��������´�ADCת�� */
		
	//return g_usAdcValue;
	
}

