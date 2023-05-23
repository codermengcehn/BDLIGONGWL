 
/*-------------------------------------------------*/
/*         	                                       */
/*           	 LED������ƣ�����           	    */
/*                                                 */
/*-------------------------------------------------*/

// Ӳ�����ӣ�
// PD2  LED0

#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "led.h"        //������Ҫ��ͷ�ļ�

/*-------------------------------------------------*/
/*����������ʼ��LED����                       	    */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStructure;                     
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	GPIO_SetBits(GPIOD, GPIO_Pin_2); 						 //PE.5 ����� 
	
}

/*-------------------------------------------------*/
/*��������LED����                                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_On(void)
{			
	GPIO_ResetBits(GPIOD, GPIO_Pin_2); 						 //PD2 �����
} 


/*-------------------------------------------------*/
/*��������LED�ر�                                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_Off(void)
{		
	GPIO_SetBits(GPIOD, GPIO_Pin_2); 						 //PD2 �����
}

void gpio_init_sensor()
{

	//relay1 b3
	GPIO_InitTypeDef GPIO_InitStructure;                     
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	
	//realy2 b4                  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;       
	GPIO_Init(GPIOB, &GPIO_InitStructure); 			
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	
	//water
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//pep
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//ai
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}



