#include "includes.h"



/*
*********************************************************************************************************
*					                      宏定义
*********************************************************************************************************
*/
#define SAMP_COUNT	10		/* 样本个数，表示200ms内的采样数据求平均值 */

uint16_t g_usAdcValue;	/* ADC 采样值的平均值 */


/*
*********************************************************************************************************
*	函 数 名: ADC_Configuration
*	功能说明: 配置ADC, PA0作为ADC通道输入
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void ADC_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	__IO uint16_t ADCConvertedValue;

    /* 使能 ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);

	/* 配置PA0为模拟输入(ADC Channel14) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 配置ADC1, 不用DMA, 用软件触发 */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* 配置ADC1 规则通道14 channel14 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_55Cycles5);

	/* 使能ADC1 DMA功能 */
	ADC_DMACmd(ADC1, ENABLE);

	/* 使能 ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* 使能ADC1 复位校准寄存器 */
	ADC_ResetCalibration(ADC1);
	/* 检查ADC1的复位寄存器 */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* 启动ADC1校准 */
	ADC_StartCalibration(ADC1);
	/* 检查校准是否结束 */
	while(ADC_GetCalibrationStatus(ADC1));

	/* 软件启动ADC转换 */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void select_sort(uint16_t *a,int n)
{
    register int i,j,min,t;
    for(i = 0;i < n-1;i++)
    {
        min = i;//查找最小值
        for(j = i + 1;j < n;j++)
            if(a[min] > a[j])
                min = j;//交换
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
*	函 数 名: ADC_GetSampleAvgN
*	功能说明: 连续读取40次ADC值求平均
*	形    参: 无      	
*	返 回 值: 返回读取的ADC值
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
	//g_usAdcValue = sum / (SAMP_COUNT-4);	/* ADC采样值由若干次采样值平均 */

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	/* 软件启动下次ADC转换 */
		
	//return g_usAdcValue;
	
}

