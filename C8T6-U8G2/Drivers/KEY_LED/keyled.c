/*
 * keyled.c
 *
 *  Created on: Dec 16, 2019
 *      Author: 王维波
 *      功能： 按键和LED驱动
 */

#include    "keyled.h"

//需要在main.h中定义宏 CHECK_KEYS_BY_QUEUE， 可以在CubeMX里FreeRTOS的User Constant里定义这个宏
#ifdef CHECK_KEYS_BY_QUEUE        //如果使用队列检测按键输入
#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"cmsis_os.h"
#include	"queue.h"

extern	osMessageQueueId_t  Queue_KeysHandle;	//Queue_Keys是在CubeMX中设计的队列，用于存储按键值
#define	QueueForKyes		Queue_KeysHandle	//替代性符号定义

#endif

//轮询方式扫描4个按键,返回按键值
//timeout单位ms，若timeout=0表示一直扫描，直到有键按下
KEYS ScanPressedKey(uint32_t timeout) {
    KEYS key = KEY_NONE;
    uint32_t tickstart = HAL_GetTick();  //当前计数值
    const uint32_t btnDelay = 20;    //按键按下阶段的抖动，延时再采样时间
    GPIO_PinState keyState;

    while (1) {
#ifdef    KeyLeft_Pin         //如果定义了KeyLeft，就可以检测KeyLeft
        keyState = HAL_GPIO_ReadPin(KeyLeft_GPIO_Port, KeyLeft_Pin); //PE4=KeyLeft,低输入有效
        if (keyState == GPIO_PIN_RESET) {
            HAL_Delay(btnDelay);  //前抖动期
            keyState = HAL_GPIO_ReadPin(KeyLeft_GPIO_Port, KeyLeft_Pin); //再采样
            if (keyState == GPIO_PIN_RESET)
                return KEY_LEFT;
        }
#endif

#ifdef    KeyRight_Pin    //如果定义了KeyRight，就可以检测KeyRight
        keyState = HAL_GPIO_ReadPin(KeyRight_GPIO_Port, KeyRight_Pin); //PE2=KeyRight,低输入有效
        if (keyState == GPIO_PIN_RESET) {
            HAL_Delay(btnDelay); //前抖动期
            keyState = HAL_GPIO_ReadPin(KeyRight_GPIO_Port, KeyRight_Pin);//再采样
            if (keyState == GPIO_PIN_RESET)
                return KEY_RIGHT;
        }
#endif

#ifdef    KeyDown_Pin        //如果定义了KeyDown，就可以检测KeyDown
        keyState = HAL_GPIO_ReadPin(KeyDown_GPIO_Port, KeyDown_Pin); //PE3=KeyDown,低输入有效
        if (keyState == GPIO_PIN_RESET) {
            HAL_Delay(btnDelay); //前抖动期
            keyState = HAL_GPIO_ReadPin(KeyDown_GPIO_Port, KeyDown_Pin);//再采样
            if (keyState == GPIO_PIN_RESET)
                return KEY_DOWN;
        }
#endif

#ifdef    KeyUp_Pin        //如果定义了KeyUp，就可以检测KeyUp
        keyState = HAL_GPIO_ReadPin(KeyUp_GPIO_Port, KeyUp_Pin); //PA0=KeyUp,高输入有效
        if (keyState == GPIO_PIN_RESET) {
            HAL_Delay(btnDelay); //10ms 抖动期
            keyState = HAL_GPIO_ReadPin(KeyUp_GPIO_Port, KeyUp_Pin);//再采样
            if (keyState == GPIO_PIN_RESET)
                return KEY_UP;
        }
#endif
#ifdef    KeyMid_Pin        //如果定义了KeyUp，就可以检测KeyUp
        keyState = HAL_GPIO_ReadPin(KeyMid_GPIO_Port, KeyMid_Pin); //PA0=KeyUp,高输入有效
        if (keyState == GPIO_PIN_RESET) {
            HAL_Delay(btnDelay); //10ms 抖动期
            keyState = HAL_GPIO_ReadPin(KeyMid_GPIO_Port, KeyMid_Pin);//再采样
            if (keyState == GPIO_PIN_RESET)
                return KEY_MID;
        }
#endif

        if (timeout != KEY_WAIT_ALWAYS)  //没有按键按下时，会计算超时，timeout时退出
        {
            if ((HAL_GetTick() - tickstart) > timeout)
                break;
        }
    }

    return key;
}

uint8_t key_scan(uint8_t mode) {
    static uint8_t key_up = 1;   //按键松开标志
    uint8_t keyval = 0;
    if (mode) key_up = 1;         //支持连按
    if (key_up && (KEYUP == 0 || KEYDOWN == 0 || KEYLEFT == 0 || KEYRIGHT == 0 || KEYMID == 0)) {
        HAL_Delay(50);
        key_up = 0;
        if (KEYUP == 0) keyval = KeyUp_Press;
        if (KEYDOWN == 0) keyval = KeyDown_Press;
        if (KEYLEFT == 0) keyval = KeyLeft_Press;
        if (KEYRIGHT == 0) keyval = KeyRight_Press;
        if (KEYMID == 0) keyval = KeyMid_Press;
    } else if (KEYUP == 1 && KEYDOWN == 1 && KEYLEFT == 1 && KEYRIGHT == 1 && KEYMID == 1)  /*没有任何按键按下，标记按键松开*/
    {
        key_up = 1;
    }
    return keyval;   /*返回键值*/
}

#ifdef    CHECK_KEYS_BY_QUEUE
void AppTask_ScanKeys(void *argument)
{
    GPIO_PinState keyState=GPIO_PIN_SET;
    KEYS  key=KEY_NONE;
    for(;;)
    {
        key=KEY_NONE;
// 1.检测KeyRight
#ifdef	KeyRight_Pin 	//如果定义了KeyRight，就可以检测KeyRight
        keyState=HAL_GPIO_ReadPin(KeyRight_GPIO_Port, KeyRight_Pin); //KeyRight
        if (keyState==GPIO_PIN_RESET)  //KeyRight 是低输入有效
            key=KEY_RIGHT;
#endif

// 2.检测KeyDown
#ifdef	KeyDown_Pin		//如果定义了KeyDown，就可以检测KeyDown
        keyState=HAL_GPIO_ReadPin(KeyDown_GPIO_Port, KeyDown_Pin); //KeyDown
        if (keyState==GPIO_PIN_RESET)  //KeyDown 是低输入有效
            key=KEY_DOWN;
#endif

// 3.检测KeyLeft
#ifdef	KeyLeft_Pin		 //如果定义了KeyLeft，就可以检测KeyLeft
        keyState=HAL_GPIO_ReadPin(KeyLeft_GPIO_Port, KeyLeft_Pin); //KeyLeft
        if (keyState==GPIO_PIN_RESET)  //KeyLeft 是低输入有效
            key=KEY_LEFT;
#endif

//4. 检测KeyUp
#ifdef	KeyUp_Pin		//如果定义了KeyUp，就可以检测KeyUp
        keyState=HAL_GPIO_ReadPin(KeyUp_GPIO_Port, KeyUp_Pin); //KeyUp
        if (keyState==GPIO_PIN_SET)  //KeyUp 是高输入有效
            key=KEY_UP;
#endif

// 5. 是否有键按下
        if (key != KEY_NONE)   //有键按下
        {
            xQueueSendToBack(QueueForKyes, &key, pdMS_TO_TICKS(100));
            vTaskDelay(pdMS_TO_TICKS(300)); //至少300才能去除抖动影响，同时让任务调度执行
        }
        else
            vTaskDelay(pdMS_TO_TICKS(5)); 	//死循环内延时，进入阻塞状态
    }
}

#endif
