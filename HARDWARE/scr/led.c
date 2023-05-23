 
/*-------------------------------------------------*/
/*         	                                       */
/*           	 LED（补光灯）控制           	    */
/*                                                 */
/*-------------------------------------------------*/

// 硬件连接：
// PD2  LED0

#include "stm32f10x.h"  //包含需要的头文件
#include "led.h"        //包含需要的头文件

/*-------------------------------------------------*/
/*函数名：初始化LED函数                       	    */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStructure;                     
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	GPIO_SetBits(GPIOD, GPIO_Pin_2); 						 //PE.5 输出高 
	
}

/*-------------------------------------------------*/
/*函数名：LED开启                                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_On(void)
{			
	GPIO_ResetBits(GPIOD, GPIO_Pin_2); 						 //PD2 输出低
} 


/*-------------------------------------------------*/
/*函数名：LED关闭                                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_Off(void)
{		
	GPIO_SetBits(GPIOD, GPIO_Pin_2); 						 //PD2 输出高
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



