#include "dht11.h"
#include "delay.h"
#include "stdio.h"
#include "led.h"
#include "beep.h"
DHT11_H11 dht11;
//PG11  ��������//�������/
//mode 1 ���ģʽ 0 ����ģʽ
void dht11_config(uint8_t mode)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure={0};//2.����ṹ�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	if(mode==1)
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//������� 
		GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	}
	else if(mode==0)
	{
	GPIO_InitStructure.GPIO_Mode =   GPIO_Mode_IN_FLOATING ;//�������� 
	}
	GPIO_Init(GPIOG,& GPIO_InitStructure);	
}
//��ʼ�ź�
void start(void)
{
	dht11_config(1);//���ģʽ
	DHT11_L;//����͵�ƽ
	delay_ms(20);//��ʾ���ĺ���
}
//��Ӧ�ź�  ����0��ȷ ��������
uint8_t check(void)
{	
	uint8_t cnt=0;
	dht11_config(0);//
	while(DHT11_STATE==1)			//�ȴ��͵�ƽ�ĵ���
	{
		delay_1us();
		cnt++;
		if(cnt>100)
		{
		return 1;
		}
	}
	cnt=0;//����ִ�е�����˵���������ڵ͵�ƽ
	while(DHT11_STATE==0)			//�ȴ��ߵ�ƽ�ĵ���
	{
		delay_1us();
		cnt++;
		if(cnt>100)
		{
		return 2;
		}
	}
		return 0;
}
//��ȡһλ
uint8_t readonrbit(void)
{
	uint8_t cnt=0;
	while(DHT11_STATE==1)			//�ȴ��͵�ƽ�ĵ���
	{
		delay_1us();
		cnt++;
		if(cnt>100)
		{
		return 3;
		}
	}
	cnt=0;
	while(DHT11_STATE==0)			//�ȴ��ߵ�ƽ�ĵ���
	{
		delay_1us();
		cnt++;
		if(cnt>100)
		{
		return 4;
		}
	}
	delay_us(30);//��ʱ30us�жϵ�ƽ״̬
	if(DHT11_STATE==0)
	{
		return 0;
	}
	else 
	{
		return 1;
	}
}
//����һ���ֽ�
uint8_t readonebyte()
{
	uint8_t data=0;
	for(uint8_t i=0;i<8;i++)
	{
		data<<=1;//����һλ 
		data|=(readonrbit());
	}
	return data;
}
//��ȡ��ʪ��
//extern uint16_t dhtime[2];
uint8_t value[5];
void get_tem_hum(void)
{
	start();
	if(check()==0)
	{
		for(uint8_t i=0;i<5;i++)
		{
		 value[i]=readonebyte();
		}
		if(value[0]+value[1]+value[2]+value[3]==value[4])
		{
			dht11.hum=value[0];
			dht11.tem=value[2]+value[3]/10;
		  //printf("ʪ�ȣ�%.2f\r\n",dht11.hum);
			//printf("�¶ȣ�%.2f\r\n",dht11.tem);
			if(dht11.tem>30)
			{
				//beep_on(beepport, beeppin);
			}
			else
			{
				//beep_off(beepport, beeppin);
			}
			if(dht11.hum>60)
			{
				//ledb_on();
			}
			else
			{
				//ledb_off();
			}	
		}
	}
	else
	{
	// printf("dht11��Ӧʧ��\r\n");
	}
	//dhtime[0]=0;
}