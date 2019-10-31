#ifndef _BSP_SPI_H_
#define _BSP_SPI_H_

#define Open_SPI1
//#define Open_SPI2

#ifdef Open_SPI1
/*SPI Clock*/
#define      SPI_APBxClock_Cmd      		RCC_APB2PeriphClockCmd
#define      SPI_CLK                		RCC_APB2Periph_SPI1
#define      SPI_GPIO_APBxClock_Cmd   	RCC_APB2PeriphClockCmd

//CS	PA4
//#define      SPI_CS_CLK             		RCC_APB2Periph_GPIOA    
//#define      SPI_CS_PORT           		 	GPIOA
//#define      SPI_CS_PIN         	  		GPIO_Pin_4

//SCK	PA5
#define      SPI_SCK_CLK            		RCC_APB2Periph_GPIOA   
#define      SPI_SCK_PORT          			GPIOA   
#define      SPI_SCK_PIN           			GPIO_Pin_5

//MISO	PA6
#define      SPI_MISO_CLK                RCC_APB2Periph_GPIOA    
#define      SPI_MISO_PORT               GPIOA 
#define      SPI_MISO_PIN                GPIO_Pin_6

//MOSI	PA7
#define      SPI_MOSI_CLK                RCC_APB2Periph_GPIOA    
#define      SPI_MOSI_PORT               GPIOA 
#define      SPI_MOSI_PIN                GPIO_Pin_7

#endif

#ifdef Open_SPI2
/*SPI Clock*/
#define      SPI_APBxClock_Cmd      		RCC_APB1PeriphClockCmd
#define      SPI_CLK                		RCC_APB1Periph_SPI2
#define      SPI_GPIO_APBxClock_Cmd   	RCC_APB2PeriphClockCmd

//CS	PB12
//#define      SPI_CS_CLK             		RCC_APB2Periph_GPIOB    
//#define      SPI_CS_PORT           		 	GPIOB
//#define      SPI_CS_PIN         	  		GPIO_Pin_12

//SCK	PB13
#define      SPI_SCK_CLK            		RCC_APB2Periph_GPIOB   
#define      SPI_SCK_PORT          			GPIOB   
#define      SPI_SCK_PIN           			GPIO_Pin_13

//MISO	PB14
#define      SPI_MISO_CLK                RCC_APB2Periph_GPIOB    
#define      SPI_MISO_PORT               GPIOB 
#define      SPI_MISO_PIN                GPIO_Pin_14

//MOSI	PB15
#define      SPI_MOSI_CLK                RCC_APB2Periph_GPIOB    
#define      SPI_MOSI_PORT               GPIOB 
#define      SPI_MOSI_PIN                GPIO_Pin_15

#endif
void bsp_InitSPI(void);

void bsp_spiWrite0(uint8_t _ucByte);
uint8_t bsp_spiRead0(void);

void bsp_spiWrite1(uint8_t _ucByte);
uint8_t bsp_spiRead1(void);

uint8_t bsp_SpiBusBusy(void);


void bsp_SpiBusEnter(void);
void bsp_SpiBusExit(void);
uint8_t bsp_SpiBusBusy(void);

#endif

