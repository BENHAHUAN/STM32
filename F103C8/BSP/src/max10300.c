#include "bsp.h"


//#define SAMPLE_50   //�������˺����50������   ������100


extern void test_max30100_fun(void);
extern u8 max10300_Bus_Read(u8 Register_Address);
extern void max10300_init(void);

#define max10300_WR_address 0xAE

u8 max10300_Bus_Write(u8 Register_Address, u8 Word_Data)
{

	/* ���ô���EEPROM�漴��ȡָ�����У�������ȡ�����ֽ� */

	/* ��1��������I2C���������ź� */
	i2c_Start();

	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte(max10300_WR_address | I2C_WR);	/* �˴���дָ�� */

	/* ��3��������ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}

	/* ��4���������ֽڵ�ַ */
	i2c_SendByte(Register_Address);
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	
	/* ��5������ʼд������ */
	i2c_SendByte(Word_Data);

	/* ��6��������ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}

	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	return 1;	/* ִ�гɹ� */

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	return 0;
}



u8 max10300_Bus_Read(u8 Register_Address)
{
	u8  data;


	/* ��1��������I2C���������ź� */
	i2c_Start();

	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte(max10300_WR_address | I2C_WR);	/* �˴���дָ�� */

	/* ��3��������ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}

	/* ��4���������ֽڵ�ַ�� */
	i2c_SendByte((uint8_t)Register_Address);
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	

	/* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
	i2c_Start();

	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte(max10300_WR_address | I2C_RD);	/* �˴��Ƕ�ָ�� */

	/* ��8��������ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}

	/* ��9������ȡ���� */
	{
		data = i2c_ReadByte();	/* ��1���ֽ� */

		i2c_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
	}
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	return data;	/* ִ�гɹ� ����dataֵ */

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	return 0;
}


#if 1
void max10300_FIFO_Read(u8 Register_Address,u16  Word_Data[][2],u8 count)
{
	u8 i=0;
	u8 no = count;
	u8 data1, data2;
	/* ��1��������I2C���������ź� */
	i2c_Start();

	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte(max10300_WR_address | I2C_WR);	/* �˴���дָ�� */

	/* ��3��������ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}

	/* ��4���������ֽڵ�ַ�� */
	i2c_SendByte((uint8_t)Register_Address);
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	

	/* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
	i2c_Start();

	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte(max10300_WR_address | I2C_RD);	/* �˴��Ƕ�ָ�� */

	/* ��8��������ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}

	/* ��9������ȡ���� */
	while (no)
	{
		data1 = i2c_ReadByte();	
		i2c_Ack();
		data2 = i2c_ReadByte();
		i2c_Ack();
		Word_Data[i][0] = (((u16)data1 << 8) | data2);  //

		
		data1 = i2c_ReadByte();	
		i2c_Ack();
		data2 = i2c_ReadByte();
		if(1==no)
			i2c_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
		else
			i2c_Ack();
		Word_Data[i][1] = (((u16)data1 << 8) | data2); 

		no--;	
		i++;
	}
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
}
#else

void max10300_FIFO_Read(u8 Register_Address,u16  Word_Data[][2],u8 count)
{
	u8 i=0;
	u8 no = count;
	u8 data1, data2;
	

	while(I2C_GetFlagStatus(USR_I2C_USED, I2C_FLAG_BUSY))
		; //���ÿ⺯�����I2C�����Ƿ���BUSY״̬
		

	I2C_AcknowledgeConfig(USR_I2C_USED, ENABLE);   /*����1�ֽ�1Ӧ��ģʽ*/


	I2C_GenerateSTART(USR_I2C_USED, ENABLE);
	 while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_MODE_SELECT))
	 	; //���EV5
	
	I2C_Send7bitAddress(USR_I2C_USED, max10300_WR_address, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(USR_I2C_USED,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		; //ADDR=1�����EV6 
		
	I2C_SendData(USR_I2C_USED, Register_Address);
	 while(! I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	 	;//��λ�Ĵ����ǿգ����ݼĴ����Ѿ��գ�����EV8���������ݵ�DR�ȿ�������¼�

	I2C_GenerateSTART(USR_I2C_USED, ENABLE);
	 while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_MODE_SELECT))
	 	; //���EV5
	
	I2C_Send7bitAddress(USR_I2C_USED, max10300_WR_address, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));


 	while (no)
    {	
	
		while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_BYTE_RECEIVED)); // EV7 
	    data1 = I2C_ReceiveData(USR_I2C_USED);

		while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_BYTE_RECEIVED)); // EV7
	    data2 = I2C_ReceiveData(USR_I2C_USED);

	    Word_Data[i][0] = (((u16)data1 << 8) | data2);  //



       while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_BYTE_RECEIVED)); // EV7
		data1 = I2C_ReceiveData(USR_I2C_USED);

	    if(no==1)
		{
     		I2C_AcknowledgeConfig(I2C1, DISABLE);	//���һλ��Ҫ�ر�Ӧ���
    		I2C_GenerateSTOP(I2C1, ENABLE);			//����ֹͣλ
		
		}

		while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_BYTE_RECEIVED)); // EV7
		data2 = I2C_ReceiveData(USR_I2C_USED);

		Word_Data[i][1] = (((u16)data1 << 8) | data2); 
		i++;

		
	    /* Decrement the read bytes counter */
	    no--;
    }
	
	I2C_AcknowledgeConfig(USR_I2C_USED, ENABLE);//��Ӧ��λʹ�ܻ�ȥ���ȴ��´�ͨ��
	I2C_GenerateSTOP(I2C1, ENABLE);			//����ֹͣλ

}

#endif
#define INTERRUPT_REG  					0X00
#define INTERRUPT_REG_A_FULL  			(0X01<<7)
#define INTERRUPT_REG_TEMP_RDY  		(0X01<<6)
#define INTERRUPT_REG_HR_RDY  			(0X01<<5)
#define INTERRUPT_REG_SPO2_RDY  		(0X01<<4)
#define INTERRUPT_REG_PWR_RDY  			(0X01<<0)

void max10300_init()
{
	max10300_Bus_Write(0x06, 0x0b);  //mode configuration : temp_en[3]      MODE[2:0]=010 HR only enabled    011 SP02 enabled
	//max10300_Bus_Write(0x06, 0x0a);  //MODE[2:0]=010 HR only enabled     when used is mode ,the red led is not used.
	max10300_Bus_Write(0x01, 0xF0); //open all of interrupt
	max10300_Bus_Write(INTERRUPT_REG, 0x00); //all interrupt clear
	max10300_Bus_Write(0x09, 0x33); //r_pa=3,ir_pa=3

#ifdef SAMPLE_50
	max10300_Bus_Write(0x07, 0x43); //SPO2_SR[4:2]=000   50 per second    LED_PW[1:0]=11  16BITS
#else
	max10300_Bus_Write(0x07, 0x47); //SPO2_SR[4:2]=001  100 per second    LED_PW[1:0]=11  16BITS
#endif
	
	max10300_Bus_Write(0x02, 0x00);   //set FIFO write Pointer reg = 0x00 for clear it
	max10300_Bus_Write(0x03, 0x00);	//set Over Flow Counter  reg = 0x00 for clear it
	max10300_Bus_Write(0x04, 0x0f);	//set FIFO Read Pointer  reg = 0x0f for   
											//waitting  write pointer eq read pointer   to   interrupts  INTERRUPT_REG_A_FULL
}

double my_floor(double x)
{
   double y=x;
    if( (*( ( (int *) &y)+1) & 0x80000000)  != 0) //����if(x<0)
        return (float)((int)x)-1;
    else
        return (float)((int)x);
}

double my_fmod(double x, double y)
{
   double temp, ret;
  
   if (y == 0.0)
      return 0.0;
   temp = my_floor(x/y);
   ret = x - temp * y;
   if ((x < 0.0) != (y < 0.0))
      ret = ret - y;
   return ret;
}


#define XPI            (3.1415926535897932384626433832795)
#define XENTRY        (100)
#define XINCL        (XPI/2/XENTRY)

  static const double XSinTbl[] = {
        0.00000000000000000  , 0.015707317311820675 , 0.031410759078128292 , 0.047106450709642665 , 0.062790519529313374 ,
        0.078459095727844944 , 0.094108313318514325 , 0.10973431109104528  , 0.12533323356430426  , 0.14090123193758267  ,
        0.15643446504023087  , 0.17192910027940955  , 0.18738131458572463  , 0.20278729535651249  , 0.21814324139654256  ,
        0.23344536385590542  , 0.24868988716485479  , 0.26387304996537292  , 0.27899110603922928  , 0.29404032523230400  ,
        0.30901699437494740  , 0.32391741819814940  , 0.33873792024529142  , 0.35347484377925714  , 0.36812455268467797  ,
        0.38268343236508978  , 0.39714789063478062  , 0.41151435860510882  , 0.42577929156507272  , 0.43993916985591514  ,
        0.45399049973954680  , 0.46792981426057340  , 0.48175367410171532  , 0.49545866843240760  , 0.50904141575037132  ,
        0.52249856471594880  , 0.53582679497899666  , 0.54902281799813180  , 0.56208337785213058  , 0.57500525204327857  ,
        0.58778525229247314  , 0.60042022532588402  , 0.61290705365297649  , 0.62524265633570519  , 0.63742398974868975  ,
        0.64944804833018377  , 0.66131186532365183  , 0.67301251350977331  , 0.68454710592868873  , 0.69591279659231442  ,
        0.70710678118654757  , 0.71812629776318881  , 0.72896862742141155  , 0.73963109497860968  , 0.75011106963045959  ,
        0.76040596560003104  , 0.77051324277578925  , 0.78043040733832969  , 0.79015501237569041  , 0.79968465848709058  ,
        0.80901699437494745  , 0.81814971742502351  , 0.82708057427456183  , 0.83580736136827027  , 0.84432792550201508  ,
        0.85264016435409218  , 0.86074202700394364  , 0.86863151443819120  , 0.87630668004386369  , 0.88376563008869347  ,
        0.89100652418836779  , 0.89802757576061565  , 0.90482705246601958  , 0.91140327663544529  , 0.91775462568398114  ,
        0.92387953251128674  , 0.92977648588825146  , 0.93544403082986738  , 0.94088076895422557  , 0.94608535882754530  ,
        0.95105651629515353  , 0.95579301479833012  , 0.96029368567694307  , 0.96455741845779808  , 0.96858316112863108  ,
        0.97236992039767667  , 0.97591676193874743  , 0.97922281062176575  , 0.98228725072868872  , 0.98510932615477398  ,
        0.98768834059513777  , 0.99002365771655754  , 0.99211470131447788  , 0.99396095545517971  , 0.99556196460308000  ,
        0.99691733373312796  , 0.99802672842827156  , 0.99888987496197001  , 0.99950656036573160  , 0.99987663248166059  ,
        1.00000000000000000  };

double XSin( double x )
{
    int s = 0 , n;
    double dx , sx , cx;
    if( x < 0 )
        s = 1 , x = -x;
    x = my_fmod( x , 2 * XPI );
    if( x > XPI )
        s = !s , x -= XPI;
    if( x > XPI / 2 )
        x = XPI - x;
    n = (int)( x / XINCL );
    dx = x - n * XINCL;
    if( dx > XINCL / 2 )
        ++n , dx -= XINCL;
    sx = XSinTbl[n];
    cx = XSinTbl[XENTRY-n];
    x = sx + dx*cx - (dx*dx)*sx/2
        - (dx*dx*dx)*cx/6 
        + (dx*dx*dx*dx)*sx/24
        ;
     
    return s ? -x : x;
}
 
double XCos( double x )
{
    return XSin( x + XPI/2 );
}

/*********************************FFT*************************************
                         ���ٸ���Ҷ�任C����
������飺�˺�����ͨ�õĿ��ٸ���Ҷ�任C���Ժ�������ֲ��ǿ�����²��ֲ���
          ��Ӳ�����˺����������������ʽ��ʾһ������������Ϊ��Ȼ˳��ĸ�
          ��������ʵ���ǿ�����鲿Ϊ0�������Ϊ����FFT�任����Ȼ˳���
          ����
ʹ��˵����ʹ�ô˺���ֻ����ĺ궨��FFT_N��ֵ����ʵ�ֵ����ĸı䣬FFT_N��
          Ӧ��Ϊ2��N�η��������������ʱӦ�ں��油0
�������ã�FFT(s);
ʱ    �䣺2010-2-20
��    ����Ver1.0
�ο����ף�     
**********************************************************************/

#define PI 3.1415926535897932384626433832795028841971               //����Բ����ֵ
#define FFT_N 1024                                                  //���帣��Ҷ�任�ĵ���

struct compx     //����һ�������ṹ
	{
		float real;
		float imag;
	};                  

struct compx s1[FFT_N+16];           //FFT������������S[1]��ʼ��ţ����ݴ�С�Լ�����
struct compx s2[FFT_N+16];           //FFT������������S[1]��ʼ��ţ����ݴ�С�Լ�����


/*******************************************************************
����ԭ�ͣ�struct compx EE(struct compx b1,struct compx b2)  
�������ܣ��������������г˷�����
��������������������嶨��ĸ���a,b
���������a��b�ĳ˻��������������ʽ���
*******************************************************************/
struct compx EE(struct compx a,struct compx b)      
{
	 struct compx c;
	 c.real=a.real*b.real-a.imag*b.imag;
	 c.imag=a.real*b.imag+a.imag*b.real;
	 return(c);
}

/*****************************************************************
����ԭ�ͣ�void FFT(struct compx *xin,int N)
�������ܣ�������ĸ�������п��ٸ���Ҷ�任��FFT��
���������*xin�����ṹ������׵�ַָ�룬struct��
*****************************************************************/
void FFT(struct compx *xin)
{
	int f,m,nv2,nm1,i,k,l,j=0;
	struct compx u,w,t;

	nv2=FFT_N/2;                  //��ַ���㣬������Ȼ˳���ɵ�λ�򣬲����׵��㷨
	nm1=FFT_N-1;  
	for(i=0;i<nm1;i++)        
	{
		if(i<j)                    //���i<j,�����б�ַ
		{
			t=xin[j];           
			xin[j]=xin[i];
			xin[i]=t;
		}
		k=nv2;                    //��j����һ����λ��
		
		while(k<=j)               //���k<=j,��ʾj�����λΪ1   
		{           
			j=j-k;                 //�����λ���0
			k=k/2;                 //k/2���Ƚϴθ�λ���������ƣ�����Ƚϣ�ֱ��ĳ��λΪ0
		}
		
		j=j+k;                   //��0��Ϊ1
	}
	 
	{  //FFT����ˣ�ʹ�õ����������FFT����
		int le,lei,ip;                           
		f=FFT_N;
		for(l=1;(f=f/2)!=1;l++)                  //����l��ֵ����������μ���
			;
		for(m=1;m<=l;m++)                           // ���Ƶ��νἶ��
		{                                           //m��ʾ��m�����Σ�lΪ���μ�����l=log��2��N
			le=2<<(m-1);                            //le���ν���룬����m�����εĵ��ν����le��
			lei=le/2;                               //ͬһ���ν��вμ����������ľ���
			u.real=1.0;                             //uΪ���ν�����ϵ������ʼֵΪ1
			u.imag=0.0;
			w.real=XCos(PI/lei);                     //wΪϵ���̣�����ǰϵ����ǰһ��ϵ������
			w.imag=-XSin(PI/lei);
			for(j=0;j<=lei-1;j++)                   //���Ƽ��㲻ͬ�ֵ��νᣬ������ϵ����ͬ�ĵ��ν�
			{
				for(i=j;i<=FFT_N-1;i=i+le)            //����ͬһ���ν����㣬������ϵ����ͬ���ν�
				{
					ip=i+lei;                           //i��ip�ֱ��ʾ�μӵ�������������ڵ�
					t=EE(xin[ip],u);                    //�������㣬�����ʽ
					xin[ip].real=xin[i].real-t.real;
					xin[ip].imag=xin[i].imag-t.imag;
					xin[i].real=xin[i].real+t.real;
					xin[i].imag=xin[i].imag+t.imag;
				}
				u=EE(u,w);                           //�ı�ϵ����������һ����������
			}
		}
	}

}

u16 g_fft_index=0;

u16 qsqrt(u32 a)
{
  u32 rem = 0, root = 0, divisor = 0;
  u16 i;
  for(i=0; i<16; i++)
  {
    root <<= 1;
    rem = ((rem << 2) + (a>>30));
    a <<= 2;
    divisor = (root << 1) + 1;
    if(divisor <= rem)
    {
      rem -= divisor;
      root++;
    }
  }
  return root;
}

#define START_INDEX    10   //�˳���Ƶ����
u16 find_max_num_index(struct compx *data,u16 count)
{
	u16 i=START_INDEX;
	u16 max_num_index = i;
	//struct compx temp=data[i];
	float temp = data[i].real;
	for(i=START_INDEX;i<count;i++)
	{
		if(temp < data[i].real)
		{
			temp = data[i].real;
			max_num_index = i;
		}
	}
	printf("max_num_index=%d\r\n",max_num_index);
	return max_num_index; 
	
}

#define CORRECTED_VALUE	50   //���Ա궨ѪҺ��������   ����׼������Ҫ��������

void sp02_treated_fun(u16 max_index)
{
	float sp02_num=0;
	 
//	delayl_init();	    	 //��ʱ������ʼ��	   
//	OLED_Init();			//��ʼ��OLED     
   
 	printf("\r\n zhiliu s1=%f,s2=%f \r\n",s1[0].real,s2[0].real);
	printf("\r\n s1=%f,s2=%f \r\n",s1[max_index].real,s2[max_index].real);
	if((s1[max_index].real*s2[0].real)>(s2[max_index].real*s1[0].real))  //if   ir>red      sp02>75%
	{
		sp02_num = (s2[max_index].real*s1[0].real)/(s1[max_index].real*s2[0].real);
		printf("\r\nsp02_num  : %f\r\n",sp02_num*100);
		printf("\r\nѪ������Ϊ: %f\r\n",(1-sp02_num)*100+CORRECTED_VALUE);
		
//		OLED_ShowString(0,0, "SpO2:",16);
//		if((1-sp02_num)*100+CORRECTED_VALUE>99)
//			OLED_ShowString(40,0, "99",16);
//		else
//			OLED_ShowNum(40,0,(1-sp02_num)*100+CORRECTED_VALUE,4,16);

//		OLED_ShowString(80,0,"%",16); 
//		OLED_ShowString(0,30,"Heart Rate:",12);   
//		OLED_Refresh_Gram();//������ʾ��OLED 	 
		
	}
	else   // sp02<75%
	{
		printf("\r\n ����ȱ��! \r\n");
		
//		OLED_ShowString(0,0, "SpO2:",16);
//		OLED_ShowString(40,0,"ANOXIA!",16);
//		OLED_ShowString(0,30,"Heart Rate:",12);   
//		OLED_Refresh_Gram();//������ʾ��OLED 
	}	
}

void test_max30100_fun(void)
{
	u16 temp_num=0;
	u16 fifo_word_buff[15][2];
	u16 Heart_Rate=0;
	u16 s1_max_index=0;
	u16 s2_max_index=0;
	
//	delayl_init();	    	 //��ʱ������ʼ��	   
//	OLED_Init();			//��ʼ��OLED     
	
		temp_num = max10300_Bus_Read(INTERRUPT_REG);
		//if( (INTERRUPT_REG_HR_RDY&temp_num) && (INTERRUPT_REG_SPO2_RDY&temp_num) )
		if( INTERRUPT_REG_A_FULL&temp_num )
		{
			max10300_FIFO_Read(0x05,fifo_word_buff,15); //read the hr and spo2 data form fifo in reg=0x05
			{
				u16 i=0;
				for(i=0;i<15;i++)
				{ 
					if(g_fft_index < FFT_N)
					{
						s1[g_fft_index].real = fifo_word_buff[i][0];
						s1[g_fft_index].imag= 0;
						s2[g_fft_index].real = fifo_word_buff[i][1];
						s2[g_fft_index].imag= 0;
						g_fft_index++;
					}
				}
				if(g_fft_index>=FFT_N)
				{
						{ //printf()  fft data
							u16 index=0;
							for(index=0;index<40;index++)
							{					
								//printf("s1[%3d]= %f\r\n",index,s1[index].real);
								//printf("s2[%3d]= %f\r\n",index,s2[index].real);
							}
						}
					 FFT(s1);
					 FFT(s2);
					 for(i=0;i<FFT_N;i++) 
					 {
							s1[i].real=sqrtf(s1[i].real*s1[i].real+s1[i].imag*s1[i].imag);
							s2[i].real=sqrtf(s2[i].real*s2[i].real+s2[i].imag*s2[i].imag);
					 }

					{ //printf()  fft data
						u16 index=START_INDEX;
						for( ;index<60;index++)
						{	
						#ifdef SAMPLE_50
								printf("f=%3.3f HZ,s1[%3d] = %f \r\n",50.0/FFT_N*index,index,s1[index].real);
								printf("f=%3.3f HZ,s2[%3d] = %f \r\n",50.0/FFT_N*index,index,s2[index].real);
						#else
//								printf("f=%3.3f HZ,s1[%3d] = %f \r\n",100.0/FFT_N*index,index,s1[index].real);
//								printf("f=%3.3f HZ,s2[%3d] = %f \r\n",100.0/FFT_N*index,index,s2[index].real);
						#endif
						}
						
					}
					s1_max_index = find_max_num_index(s1, 60);
					s2_max_index = find_max_num_index(s2, 60);
					if(s1_max_index == s2_max_index)	
					{
				#ifdef SAMPLE_50	 
						Heart_Rate =  60*50*((s1_max_index+s2_max_index )/2)/FFT_N;
				#else
						Heart_Rate =  60*100*((s1_max_index+s2_max_index )/2)/FFT_N;
				#endif
						printf("\r\n����Ϊ: %d\r\n",Heart_Rate);
						
						//sp02_treated_fun(s1_max_index);	
						
//						OLED_ShowString(0,0, "SpO2:",16);
//						OLED_ShowString(0,30,"Heart_Rate:",12); 
//						OLED_ShowNum(70,30,Heart_Rate-10,4,16);
//						OLED_Refresh_Gram();//������ʾ��OLED 
											
					}
//					else
//					{
//						printf("\r\n ����ʧ�ܣ�  ��ѹ�����²���!\r\n");
//						
////						OLED_ShowString(0,0, "SpO2:",16);
////						OLED_ShowString(0,30,"Heart_Rate:",12); 
////						OLED_ShowString(70,30,"ERROR",16); 
////						OLED_Refresh_Gram();//������ʾ��OLED 
//					}
					g_fft_index = 0;
				}
			}
		/*	ic �Զ�����FIFOָ�� ���ò���
			max10300_Bus_Write(0x02, 0x00);   //set FIFO write Pointer reg = 0x00 for clear it
			max10300_Bus_Write(0x03, 0x00);	//set Over Flow Counter  reg = 0x00 for clear it
			max10300_Bus_Write(0x04, 0x0f);	//set FIFO Read Pointer  reg = 0x0F 
			max10300_Bus_Write(INTERRUPT_REG, 0x00); // all interrupt clear
		*/
		}
	}


