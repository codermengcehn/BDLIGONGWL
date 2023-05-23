 /*-----------------------------------------------------*/
/*                                                     */
/*            ����main��������ں���Դ�ļ�              */
/*                                                     */
/*-----------------------------------------------------*/

#include "stm32f10x.h"   //������Ҫ��ͷ�ļ�
#include "control.h"     //������Ҫ��ͷ�ļ�
#include "delay.h"       //������Ҫ��ͷ�ļ�
#include "usart1.h"      //������Ҫ��ͷ�ļ�
#include "usart2.h"      //������Ҫ��ͷ�ļ�
#include "timer2.h"      //������Ҫ��ͷ�ļ�
#include "timer3.h"      //������Ҫ��ͷ�ļ�
#include "timer4.h"      //������Ҫ��ͷ�ļ�
#include "wifi.h"	     //������Ҫ��ͷ�ļ�
#include "led.h"	     //������Ҫ��ͷ�ļ� LED                                                  
#include "mqtt.h"        //������Ҫ��ͷ�ļ�
#include "dht11.h"       //������Ҫ��ͷ�ļ� ������ʪ��

int infrared_state(){ return  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8);}
int  ai_state(){ 	 return 	   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9);}
int  water_state(){  return     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7);}
void relay1_on(){        GPIO_SetBits(GPIOB,GPIO_Pin_13);  DelayMs(10);}
void relay2_on(){        GPIO_SetBits(GPIOB,GPIO_Pin_14);DelayMs(10);}

void relay1_off(){GPIO_ResetBits(GPIOB,GPIO_Pin_13);}
void relay2_off(){GPIO_ResetBits(GPIOB,GPIO_Pin_14);}
 
char *cmdLED_On = "relayon";      //LED��
char *cmdLED_Off = "relayoff";    //LED�ر�

char *cmdDHT11_On = "DHTON";    //��ʪ�����ݴ����
char *cmdDHT11_Off = "DHTOFF";  //��ʪ�����ݴ���ر�

char *ledFlag = "LEDOFF";       //LED״̬
int   dhtFlag = 0;		        //��ʪ�����ݴ���״̬

int main(void) 
{	
	
	Delay_Init();          //��ʱ���ܳ�ʼ��              
	Usart1_Init(9600);     //����1���ܳ�ʼ����������9600
	Usart2_Init(115200);   //����2���ܳ�ʼ����������115200	
	TIM4_Init(500,7200);   //TIM4��ʼ������ʱʱ�� 500*7200*1000/72000000 = 50ms
	LED_Init();			   //LED��ʼ��
	DHT11_Init();          //��ʼ��DHT11	
	gpio_init_sensor();
	printf("init all sensor\n\r");
	WiFi_ResetIO_Init();   //��ʼ��WiFi�ĸ�λIO
	IoT_Parameter_Init();  //��ʼ����IoTƽ̨MQTT�������Ĳ���	

	while(1)
	{	
//
//		if(ai_state()==1)
//		{
//			printf("find a cat \n\r");
//			relay2_on();
//			DelayMs(500);
//			relay2_off();
//		}
//		
		if(water_state() == 0 ){
			//printf("no water\n\r");
			relay1_on();
			
			
			
		}else
		relay1_off();
		
		if((ai_state()==1))
		{
			printf("is a animail && AI find a cat in camera stream  >>>>>>>>>>>>>>>>>>>>>>>\n\r");
			relay2_on();
			DelayMs(500);
			
		}else
		relay2_off();
		
		// connectFlag=1ͬ���������������� 
		if(connectFlag == 1) 
		{     
			/*-------------------------------------------------------------*/
			/*                     �����ͻ���������						 */
			/*-------------------------------------------------------------*/
			if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr) //if�����Ļ���˵�����ͻ�������������
			{                
				//3������ɽ���if
				//��1�֣�0x10 ���ӱ���
				//��2�֣�0x82 ���ı��ģ���connectPackFlag��λ����ʾ���ӱ��ĳɹ�
				//��3�֣�subcribePackFlag��λ��˵�����ӺͶ��ľ��ɹ����������Ŀɷ�
				if((MQTT_TxDataOutPtr[2] == 0x10)||((MQTT_TxDataOutPtr[2] == 0x82)&&(connectPackFlag == 1))||(subcribePackFlag == 1)) 
				{    
					if (MQTT_TxDataOutPtr[2] == 0x30) 
					{	
						u1_printf("��������:0x%x����Ƭ������������������\r\n", MQTT_TxDataOutPtr[2]);	
						u1_printf("\r\n");
					}
					else 
					{
						u1_printf("��������:0x%x\r\n", MQTT_TxDataOutPtr[2]);//������ʾ��Ϣ
					}
					
					MQTT_TxData(MQTT_TxDataOutPtr);							//��������
					MQTT_TxDataOutPtr += TBUFF_UNIT;						//ָ������
					if(MQTT_TxDataOutPtr == MQTT_TxDataEndPtr)				//���ָ�뵽������β����
					{ 
						MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];				//ָ���λ����������ͷ
					}	
				} 				
			}
			/*-------------------------------------------------------------*/
			/*                     ������ջ���������                      */
			/*-------------------------------------------------------------*/
			if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr) //if�����Ļ���˵�����ջ�������������	
			{												
				u1_printf("���յ�����:");
				/*-----------------------------------------------------*/
				/*                    ����CONNACK����                  */
				/*-----------------------------------------------------*/				
				//if�жϣ������һ���ֽ���0x20����ʾ�յ�����CONNACK����
				//��������Ҫ�жϵ�4���ֽڣ�����CONNECT�����Ƿ�ɹ�
				if(MQTT_RxDataOutPtr[2] == 0x20)
				{             			
					switch(MQTT_RxDataOutPtr[5])
					{					   
						case 0x00 : u1_printf("CONNECT���ĳɹ�\r\n");							//���������Ϣ	
										connectPackFlag = 1;									//CONNECT���ĳɹ������ı��Ŀɷ�
									break;														//������֧case 0x00                                              
						case 0x01 : u1_printf("�����Ѿܾ�����֧�ֵ�Э��汾��׼������\r\n");    //���������Ϣ
									connectFlag = 0;											//connectFlag���㣬��������
									break;														//������֧case 0x01   
						case 0x02 : u1_printf("�����Ѿܾ������ϸ�Ŀͻ��˱�ʶ����׼������\r\n");//���������Ϣ
									connectFlag = 0;                                            //connectFlag���㣬��������
									break; 														//������֧case 0x02 
						case 0x03 : u1_printf("�����Ѿܾ�������˲����ã�׼������\r\n");	    //���������Ϣ
									connectFlag = 0;											//connectFlag���㣬��������
									break;														//������֧case 0x03
						case 0x04 : u1_printf("�����Ѿܾ�����Ч���û��������룬׼������\r\n");	//���������Ϣ
									connectFlag = 0;											//connectFlag���㣬��������						
									break;														//������֧case 0x04
						case 0x05 : u1_printf("�����Ѿܾ���δ��Ȩ��׼������\r\n");				//���������Ϣ
									connectFlag = 0;											//connectFlag���㣬��������						
									break;														//������֧case 0x05 		
						default   : u1_printf("�����Ѿܾ���δ֪״̬��׼������\r\n");		    //���������Ϣ 
									connectFlag = 0;											//connectFlag���㣬��������					
									break;														//������֧case default 								
					}				
			    }			
				//if�жϣ���һ���ֽ���0x90����ʾ�յ�����SUBACK����
				//��������Ҫ�ж϶��Ļظ��������ǲ��ǳɹ�
				else if(MQTT_RxDataOutPtr[2] == 0x90)
				{ 
					switch(MQTT_RxDataOutPtr[6])
					{					
						case 0x00 :
						case 0x01 : 
									u1_printf("���ĳɹ�\r\n");			//���������Ϣ
									subcribePackFlag = 1;				//subcribePackFlag��1����ʾ���ı��ĳɹ����������Ŀɷ���
									pingFlag = 0;						//pingFlag����
									TIM3_ENABLE_30S();					//����30s��PING��ʱ��	
									Send_Data();
									TIM2_ENABLE_10S();
									break;								//������֧                                             
						default: 
									u1_printf("����ʧ�ܣ�׼������\r\n");//���������Ϣ 
									connectFlag = 0;					//connectFlag���㣬��������
									break;								//������֧ 								
						}					
			    }
				//if�жϣ���һ���ֽ���0xD0����ʾ�յ�����PINGRESP����
				else if(MQTT_RxDataOutPtr[2] == 0xD0)
				{ 
					u1_printf("PING���Ļظ�\r\n");                      //���������Ϣ 
					if(pingFlag == 1)
					{                   						        //���pingFlag=1����ʾ��һ�η���
						 pingFlag = 0;    				       			//Ҫ���pingFlag��־
					}
					else if(pingFlag > 1)	
					{ 				 									//���pingFlag>1����ʾ�Ƕ�η����ˣ�������2s����Ŀ��ٷ���
						pingFlag = 0;     				      			//Ҫ���pingFlag��־
						TIM3_ENABLE_30S(); 				      			//PING��ʱ���ػ�30s��ʱ��
					}				
				}	
				//if�жϣ������һ���ֽ���0x30����ʾ�յ����Ƿ�������������������
				//����Ҫ��ȡ��������
				else if((MQTT_RxDataOutPtr[2] == 0x30))
			    { 
					u1_printf("�������ȼ�0����\r\n"); 		   	//���������Ϣ 
					MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);   //����ȼ�0��������
				}									
				MQTT_RxDataOutPtr += RBUFF_UNIT;                //ָ������
				if(MQTT_RxDataOutPtr == MQTT_RxDataEndPtr)      //���ָ�뵽������β����
				{
					MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];      //ָ���λ����������ͷ              
				}		          
			}//������ջ��������ݵ�else if��֧��β
						
			/*---------------------------------------------------------------------------------------------------------------------*/
			/*                    							 ���������������                   		          			         */
			/*---------------------------------------------------------------------------------------------------------------------*/
			if(MQTT_CMDOutPtr != MQTT_CMDInPtr)							  //if�����Ļ���˵�����������������	
			{                             		       
				u1_printf("����:%s\r\n",&MQTT_CMDOutPtr[2]);              //���������Ϣ
				
				if(!memcmp(&MQTT_CMDOutPtr[2],cmdLED_On,strlen(cmdLED_On)))//�ж�ָ������CMD1 
				{                                            
					ledFlag = "relayon";  				
					relay1_on();
					relay2_on();
					DelayMs(1000);
					relay1_off();	
					relay2_off();
//					LED_On();									  		   //LED����
					
				}
				else if(!memcmp(&MQTT_CMDOutPtr[2],cmdLED_Off,strlen(cmdLED_Off))) //�ж�ָ������CMD11
				{ 
					ledFlag = "relayoff";                                              
					relay1_off();	
					relay2_off();
					//LED�ر�
				}
				else if(!memcmp(&MQTT_CMDOutPtr[2],cmdDHT11_On,strlen(cmdDHT11_On)))//�ж�ָ������CMD3
				{      
					dhtFlag = 1;                 //dataFlag��1����ʾ���ڲɼ�״̬��
					TIM2_ENABLE_10S();           //��ʱ��2��,10s����ɼ���ʪ��   
				}
				else if(!memcmp(&MQTT_CMDOutPtr[2],cmdDHT11_Off,strlen(cmdDHT11_Off)))//�ж�ָ������CMD3
				{     
					dhtFlag = 0;                 //dataFlag��0����ʾ����ֹͣ״̬��
					TIM_Cmd(TIM2,DISABLE);		 //�ж�2·����״̬�Ͳɼ�״̬����������������
				}								
				//������������ᷢ��״̬
				else u1_printf("δָ֪��\r\n");				//���������Ϣ	
			
				MQTT_CMDOutPtr += CBUFF_UNIT;				//ָ������
				if(MQTT_CMDOutPtr == MQTT_CMDEndPtr)	    //���ָ�뵽������β����
				MQTT_CMDOutPtr = MQTT_CMDBuf[0];			//ָ���λ����������ͷ	
				
				Send_Data();//���Ϳ�������
				//��������������ݵ�else if��֧��β					
			}	//connectFlag=1��if��֧�Ľ�β
		}
	/*--------------------------------------------------------------------*/
	/*      connectFlag=0ͬ�������Ͽ�������,����Ҫ�������ӷ�����            */
	/*--------------------------------------------------------------------*/
		else
		{ 
			u1_printf("��Ҫ���ӷ�����\r\n");               //���������Ϣ
			TIM_Cmd(TIM4, DISABLE);                        //�ر�TIM4 
			TIM_Cmd(TIM3, DISABLE);                        //�ر�TIM3  
			WiFi_RxCounter = 0;                            //WiFi������������������                        
			memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);      //���WiFi���ջ����� 
			if(WiFi_Connect_IoTServer() == 0)			   //���WiFi�����Ʒ�������������0����ʾ��ȷ������if
			{   			     
				u1_printf("����TCP���ӳɹ�\r\n");          //���������Ϣ
				connectFlag = 1;                           //connectFlag��1����ʾ���ӳɹ�	
				WiFi_RxCounter = 0;                        //WiFi������������������                        
				memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);  //���WiFi���ջ����� 
				MQTT_Buff_Init();                          //��ʼ�����ͻ�����                    
			}				
		}
	}
}
