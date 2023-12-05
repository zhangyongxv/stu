#include "mqtt.h"
#include "MQTTPacket.h"
#include "esp8266.h"
#include "string.h"
#include "delay.h"
#include "su03t.h"
extern esp esp8266;
//���Ӱ����Ʒ�������

//�����жϿͻ��˷��������Ƿ�ɹ�������ɹ��᷵�� 20 02 �������� 0���쳣 1��
uint8_t mqtt_checkback(uint16_t ms)
{
	while(ms--){
	if(esp8266.esrvflag1==1)
	{
	esp8266.esrvflag1=0;
		if((esp8266.resrv1[0]==0x20)&&(esp8266.resrv1[1]==0x02))
		{
		return 0;
		}
	}
	delay_ms(1);
	}
	return 1;
}

uint8_t mqtt_connect()
{
	//char buf[1024];
	MQTTPacket_connectData options=MQTTPacket_connectData_initializer;//�������ӱ���ͨ�ò���Ĭ������
	options.clientID.cstring = "ghskvL72IuQ.ESP8266|securemode=2,signmethod=hmacsha256,timestamp=1689735481049|";
	options.username.cstring = "ESP8266&ghskvL72IuQ";
	options.password.cstring = "cb2245d1bb6176eb5acb95a6cadf6cc25b373a08dff098115709457248aaa11b";
	memset(esp8266.sen1,0,sizeof(esp8266.sen1));//��շ��ͻ�����
	esp8266.sencnt1=0;
	esp8266.sencnt1=MQTTSerialize_connect( esp8266.sen1, 1024, &options);//b�ѱ��ĵ������������ϵ����ͻ�����//����ǹ���CONNACK���ĵĺ���
	
	memset(esp8266.resrv1,0,sizeof(esp8266.resrv1));//��ս��ջ�����
	esp8266.esrvcnt1=0;
	
	usart3_sendstr1(esp8266.sen1,esp8266.sencnt1);
	return mqtt_checkback(1000);
}
//MQTT�������ݺ���  
uint8_t mqtt_publish(char *payload)
{
	MQTTString topicName = MQTTString_initializer;
	topicName.cstring="/sys/ghskvL72IuQ/ESP8266/thing/event/property/post";
	//topicName.
	memset(esp8266.sen1,0,sizeof(esp8266.sen1));//��շ��ͻ�����
	esp8266.sencnt1=0;
	uint16_t len=strlen(payload);
	esp8266.sencnt1=MQTTSerialize_publish(esp8266.sen1, 1024, 0, 0, 0, 0,topicName, (unsigned char *)payload,len);//����PUBLISH���ĵĺ�������Ҫ������Ϣ�������˺������챨��
	
	memset(esp8266.resrv1,0,sizeof(esp8266.resrv1));//��ս��ջ�����
	esp8266.esrvcnt1=0;
	
	usart3_sendstr1(esp8266.sen1,esp8266.sencnt1);
	return mqtt_checkback(1000);
}


//MQTT����
uint8_t mqtt_subscribe()
{
		MQTTString topicFilters[1]={0};
	topicFilters[0].cstring="/sys/ghskvL72IuQ/ESP8266/thing/service/property/set";
	int requestedQoSs[1]={2};//
	
	memset(esp8266.sen1,0,sizeof(esp8266.sen1));//��շ��ͻ�����
	esp8266.sencnt1=0;
	
	esp8266.sencnt1=MQTTSerialize_subscribe(esp8266.sen1, 1024, 0, 0, 1, topicFilters, requestedQoSs);
	
	memset(esp8266.resrv1,0,sizeof(esp8266.resrv1));//��ս��ջ�����
	esp8266.esrvcnt1=0;
	
	usart3_sendstr1(esp8266.sen1,esp8266.sencnt1);

}
//��������    �ӷ������·��������������//����
uint8_t mqtt_analysis()
{
	char buff[1024];
	if(esp8266.esrvflag1==1)
	{
		esp8266.esrvflag1=0;
		printf("�յ������ݣ�%s\r\n",esp8266.resrv1);
		char * num =strstr((char *)esp8266.resrv1,"\"Humidity\":");
		strncpy(buff,num+11,2);
		printf("ʪ�ȣ�%s\r\n",buff);
		num =strstr((char *)esp8266.resrv1,"\"temperature\":");
		strncpy(buff,num+14,2);
		printf("�¶ȣ�%s\r\n",buff);
	}
}
