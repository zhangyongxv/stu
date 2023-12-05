#include "rtc.h"
#include "stdio.h"

void rtc_config(void)
{

   //NVIC_Configuration();//中断优先级

  if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)//备份设置寄存器//从指定的后备寄存器中读出数据
  {
    /* Backup data register value is not correct or not yet programmed (when
       the first time the program is executed) */

    printf("\r\n\n RTC not yet configured....");

    /* RTC Configuration */
    RTC_Configuration();

    printf("\r\n RTC configured....");

    /* Adjust time by values entered by the user on the hyperterminal */
    Time_Adjust();//设置时间

    BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
  }
  else
  {
    /* Check if the Power On Reset flag is set */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)//复位
    {
      printf("\r\n\n Power On Reset occurred....");
    }
    /* Check if the Pin Reset flag is set */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)//引脚复位
    {
      printf("\r\n\n External Reset occurred....");
    }

    printf("\r\n No need to configure RTC....");
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();//等待同步

    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();//等待最近一次对 RTC 寄存器的写操作完成
  }
}


void NVIC_Configuration(void)//中断优先级
{
  NVIC_InitTypeDef NVIC_InitStructure={0};
  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


void RTC_Configuration(void)
{
  /* Enable PWR and BKP clocks *///使能或者失能 APB1 外设时钟电源和BKP时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);//使能或者失能 RTC 和后备寄存器访问//功耗控制
	
  /* Reset Backup Domain */
  BKP_DeInit();//将外设 BKP 的全部寄存器重设为缺省值

  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_ON);//设置外部低俗晶振（LSE）
  /* Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)//等待LSE晶振就绪
  {}

  /* Select LSE as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//选择rtc时钟来源为LSE

  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);//使能RTC时钟

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();//等待 RTC 寄存器(RTC_CNT, RTC_ALR and RTC_PRL)与RTC 的 APB 时钟同步

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();//等待最近一次对 RTC 寄存器的写操作完成

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);//秒中断使能

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();//等待最近一次对 RTC 寄存器的写操作完成

  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) *///设置预分频值

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();//等待最近一次对 RTC 寄存器的写操作完成
}




/**
  * @brief  Adjusts time.
  * @param  None
  * @retval None
  */
void Time_Adjust(void)
{
	
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();//等待最近一次对 RTC 寄存器的写操作完成
  /* Change the current time */
  RTC_SetCounter(0);//设置 RTC 计数器的值
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();//等待最近一次对 RTC 寄存器的写操作完成
	
}
