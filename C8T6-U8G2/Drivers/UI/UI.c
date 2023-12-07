//
// Created by JLF on 2023/12/1.
//

#include "UI.h"
#include "math.h"

KEYS keyState = KEY_NONE;

/*-------------------Box coordinate--------------------*/
short current_x = 0;
short current_y = 0;
short target_x = 0;
short target_y = 0;
uint8_t Box_CurW = 16;
uint8_t Box_CurH = 16;

uint8_t key = 0;


int animation(short *cur, short *trg, uint8_t step, uint8_t slow_cnt) {
    double temp;
    temp = fabs(*trg - *cur) > slow_cnt ? step : 1;
    if (*cur < *trg) {
        *cur += temp;
    } else if (*cur > *trg) {
        *cur -= temp;
    } else {
        return 0;
    }
    return 1;
}

int betteranimation(double *cur, double vel, double *trg, double k, double c) {
    double delta = *trg - *cur; // 计算当前值与目标值的差
    vel += k * delta - c * (vel); // 根据二阶欠阻尼的公式更新速度
    *cur += vel; // 根据速度更新当前值

    if (fabs(delta) < 0.1 && fabs(vel) < 0.1) { // 如果当前值已经非常接近目标值并且速度很小
        return 0; // 返回0表示动画结束
    }
    return 1; // 返回1表示动画未结束
}


void scanKeys(u8g2_t *u8g2) {
    keyState = ScanPressedKey(0);
    if (HAL_GPIO_ReadPin(KeyMid_GPIO_Port, KeyMid_Pin) == GPIO_PIN_RESET) {
        Box_CurH = 12;
        Box_CurW = 55;
    } else if (HAL_GPIO_ReadPin(KeyMid_GPIO_Port, KeyMid_Pin) == GPIO_PIN_SET) {
        Box_CurH = 16;
        Box_CurW = 60;
    }
    if (keyState == KEY_RIGHT) {
        if (target_x >= Screen_Width - Box_Width) {
            target_x = Screen_Width - Box_Width;
        } else {
            target_x += Step;
        }
    } else if (keyState == KEY_LEFT) {
        if (target_x <= Boundary + Box_Width) {
            target_x = 0;
        } else {
            target_x -= Step;
        }

    } else if (keyState == KEY_UP) {
        if (target_y <= Boundary + Box_Height) {
            target_y = 0;
        } else {
            target_y -= Step;
        }
    } else if (keyState == KEY_DOWN) {
        if (target_y >= Screen_Height - Box_Height) {
            target_y = Screen_Height - Box_Height;
        } else {
            target_y += Step;
        }
    }
    u8g2_ClearBuffer(u8g2);
    animation(&current_x, &target_x, 2, 20);
    animation(&current_y, &target_y, 2, 20);
    u8g2_DrawRFrame(u8g2, current_x, current_y, Box_CurW, Box_CurH, 5);

    u8g2_SendBuffer(u8g2);
}

void ZDscanKeys(u8g2_t *u8g2) {
    uint8_t mode = 0;
    key = key_scan(mode);
    if (key) {
        switch (key) {
            case KeyUp_Press:
                if (target_y <= Boundary + Box_Height) {
                    target_y = 0;
                } else {
                    target_y -= Step;
                }
                break;
            case KeyDown_Press:
                if (target_y >= Screen_Height - Box_Height) {
                    target_y = Screen_Height - Box_Height;
                } else {
                    target_y += Step;
                }
                break;
            case KeyLeft_Press:
                if (target_x <= 15) {
                    target_x = 0;
                } else {
                    target_x -= Step;
                }
                break;
            case KeyRight_Press:
                if (target_x >= Screen_Width - Box_CurW) {
                    target_x = Screen_Width - Box_CurW;
                } else {
                    target_x += Step;
                }
                break;
            case KeyMid_Press:
                LED_Toggle();
                if (mode == 0) {
                    mode = 1;
                } else {
                    mode = 0;
                }
//                if (Box_CurW == 64)
//                {
//                    Box_CurW=32;
//                }
//                else
//                {
//                    Box_CurW=64;
//                }
                break;
        }
    } else {
        HAL_Delay(10);
    }
    u8g2_ClearBuffer(u8g2);
//    u8g2_DrawRFrame(u8g2, current_x, current_y, Box_CurW, Box_CurH, 5);
    u8g2_DrawRBox(u8g2, current_x, current_y, Box_CurW, Box_CurH, 5);
    animation(&current_x, &target_x, 2, 4);
    animation(&current_y, &target_y, 2, 4);
    u8g2_SendBuffer(u8g2);
}

#define up  1
#define down 2
#define left 3
#define right 4
#define middle 5

typedef struct {
    uint8_t id;
} Key;


/*!
 *
 * @param mode key mode support 0:singal click or 1:continue click
 * @return keyvalue
 */
uint8_t myScan(int mode) {
    int m = mode;
    key = key_scan(m);
    Key keyvalue;
    if (key) {
        switch (key) {
            case KeyUp_Press:
                keyvalue.id = up;
                break;
            case KeyDown_Press:
                keyvalue.id = down;
                break;
            case KeyLeft_Press:
                keyvalue.id = left;
                break;
            case KeyRight_Press:
                keyvalue.id = right;
                break;
            case KeyMid_Press:
                keyvalue.id = middle;
                break;
        }
    } else {
        HAL_Delay(10);
    }
    return keyvalue.id;
}

void controlBar(u8g2_t *u8g2) {
    key = myScan(0);
    short max_y = 48;  // 最大的y坐标值
    short min_y = 0;   // 最小的y坐标值
    // 在处理按键之前再次检查当前的frame_y和bar_y是否越界
    if (frame_y < min_y && bar_y < min_y) {
        frame_y = min_y;
        bar_y = min_y;
    } else if (frame_y > max_y && bar_y > min_y) {
        frame_y = max_y;
        bar_y = max_y;
    }
    switch (key) {
        case up:
            LED_Toggle();
            if (frame_y <= 0 && bar_y <= 0) {
                frame_y_trg = 48;
                bar_y_trg = 48;
            } else {
                frame_y_trg -= 16;
                bar_y_trg -= 16;
            }

            break;
        case down:
            LED_Toggle();
            if (frame_y >= 48 && bar_y >= 48) {
                frame_y_trg = 0;
                bar_y_trg = 0;
            } else {
                frame_y_trg += 16;
                bar_y_trg += 16;
            }

            break;
        case left:
            LED_Toggle();
            break;
        case right:
            LED_Toggle();
            break;
        case middle:
            LED_Toggle();
            break;
        default:
            break;
    }
    showMainMenu(u8g2);
}