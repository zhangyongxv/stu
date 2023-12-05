#include "SPI.h"
//以下是SPI模块的初始化代码，配置成主机模式 						  
//SPI口初始化
//这里针是对SPI1的初始化
void SPI2_Init(void)
{//FLASH_CS pb12
	//SPI_MISO pb14
	//SPI_SCK pb13
	//SPI_MOSI pb15
	
	/* SPI的IO口和SPI外设打开时钟 */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	/* SPI的IO口设置 */
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;  //CS片选
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15 ;  //PB13-SPI1_SCK  PB15-SPI1_MOSI
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;  //fuyong推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入 MISO
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	SPI_InitTypeDef  SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	//SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;	
	//SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设
	
	//SPI_NSSInternalSoftwareConfig(SPI2, SPI_NSSInternalSoft_Set);//为选定的 SPI 软件配置内部 NSS 管脚
	
	//SPI_DataSizeConfig (SPI2,SPI_DataSize_8b);//设置选定的 SPI 数据大小
	//SPI_I2S_ITConfig(SPI2,SPI_I2S_IT_TXE|SPI_I2S_IT_RXNE,ENABLE);
	
//	//中断优先级
//	NVIC_InitTypeDef NVIC_InitStruct={0};
//	NVIC_InitStruct.NVIC_IRQChannel=SPI1_IRQn;
//	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;
//	NVIC_Init(& NVIC_InitStruct);
// 
 
	//SPI1_ReadWriteByte(0xff);//启动传输	
}


uint8_t spiflag=0;
void SPI2_IRQHandler(void)//中断函数
{
	uint8_t data=0;
	if(SPI_I2S_GetITStatus(SPI2,SPI_I2S_IT_RXNE)==1)//接受缓存非空中断标志位
	{
		data=SPI_I2S_ReceiveData(SPI2);
	}
	
	if(SPI_I2S_GetITStatus(SPI2,SPI_I2S_IT_TXE)==1)//发送缓存空中断标志位
	{
		spiflag=1;
		SPI_I2S_ClearFlag(SPI2,SPI_I2S_IT_TXE);//清楚空闲标志位
		SPI_I2S_SendData(SPI2,0x1234);
		
	}
}