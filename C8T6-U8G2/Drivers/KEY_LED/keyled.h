/*
 * keyled.h
 *
 *  Created on: Dec 16, 2019
 *      Author: 王维波
 *      功能：轮询方式扫描按键输入，LED显示控制
 *      使用方法：在CubeMX中定义LED和按键引脚的的Label
 */

#ifndef KEYLED_H_
#define KEYLED_H_

#include	"main.h"   //在main.h中定义了Keys和LEDs的Labels宏

/*  这些宏在main.h中定义，在CubeMX中定义引脚的Label自动生成的宏定义
#define KeyRight_Pin 		GPIO_PIN_4
#define KeyRight_GPIO_Port 	GPIOE

#define KeyDown_Pin 		GPIO_PIN_3
#define KeyDown_GPIO_Port 	GPIOE

#define KeyLeft_Pin 		GPIO_PIN_2
#define KeyLeft_GPIO_Port 	GPIOE

#define LED1_Pin 			GPIO_PIN_9
#define LED1_GPIO_Port 		GPIOF

#define LED2_Pin 			GPIO_PIN_10
#define LED2_GPIO_Port 		GPIOF

#define KeyUp_Pin 			GPIO_PIN_0
#define KeyUp_GPIO_Port 	GPIOA


#define Buzzer_Pin 			GPIO_PIN_8
#define Buzzer_GPIO_Port 	GPIOF

 */

//表示4个按键的枚举类型
typedef enum {
	KEY_NONE=0,		//没有按键被按下
	KEY_LEFT,		//KeyLeft
	KEY_RIGHT,		//KeyRight
	KEY_UP,			//KeyUp
	KEY_DOWN,		//KeyDown
    KEY_MID
}KEYS;

#define		KEY_WAIT_ALWAYS		0	//作为函数ScanKeys()的一种参数，表示一直等待按键输入
//轮询方式扫描按键，timeout=KEY_WAIT_ALWAYS表示一直扫描，否是等待时间timeout, 延时单位ms
KEYS  ScanPressedKey(uint32_t timeout);


#ifdef	LED_Pin		//LED1的控制
	#define	 LED_Toggle()	HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin)    //输出翻转

	#define	 LED1_ON()		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET)  //输出0，亮

	#define	 LED_OFF()		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET)  //输出1，灭
#endif


#ifdef	LED2_Pin	//LED2的控制
	#define	 LED2_Toggle()	HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin)    //输出翻转

	#define	 LED2_ON()		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET)  //输出0，亮

	#define	 LED2_OFF()		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET)  //输出1，灭
#endif


#ifdef	Buzzer_Pin		//蜂鸣器的控制
	#define	 Buzzer_Toggle()	HAL_GPIO_TogglePin(Buzzer_GPIO_Port, Buzzer_Pin)  //输出翻转

	#define	 Buzzer_ON()		HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET)    //输出1，蜂鸣器响

	#define	 Buzzer_OFF()		HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET)  //输出0，蜂鸣器不响
#endif

/*---------------------------正点原子按键程序------------------------------*/

#define KEYRIGHT        HAL_GPIO_ReadPin(KeyRight_GPIO_Port, KeyRight_Pin)
#define KEYLEFT         HAL_GPIO_ReadPin(KeyLeft_GPIO_Port, KeyLeft_Pin)
#define KEYUP           HAL_GPIO_ReadPin(KeyUp_GPIO_Port, KeyUp_Pin)
#define KEYDOWN         HAL_GPIO_ReadPin(KeyDown_GPIO_Port, KeyDown_Pin)
#define KEYMID          HAL_GPIO_ReadPin(KeyMid_GPIO_Port, KeyMid_Pin)

#define KeyLeft_Press    1
#define KeyRight_Press   2
#define KeyUp_Press      3
#define KeyDown_Press    4
#define KeyMid_Press     5

uint8_t key_scan(uint8_t mode);


#endif /* KEYLED_H_ */
