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

int mainMenuStart = 0;
int MenuState = 0;

uint8_t key = 0;

int animation(short *cur, short *trg, uint8_t step, uint8_t slow_cnt) {
    double temp;
    temp = fabs((double) *trg - (double) *cur) > slow_cnt ? step : 1;
    if (*cur < *trg) {
        *cur += temp;
    } else if (*cur > *trg) {
        *cur -= temp;
    } else {
        return 0;
    }
    return 1;
}

/*
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
            default:
                break;
        }
    } else {
        HAL_Delay(10);
    }
    return keyvalue.id;
}

/*!
 * up key control code
 */
void up_function() {
    if (frame_y <= 0 && bar_y <= 0) {
        frame_y_trg = 0;
        bar_y_trg = 0;
        if (mainMenuStart > 0) {
            mainMenuStart--;
        }
    } else {
        frame_y_trg -= 16;
        bar_y_trg -= 16;
    }
}

/*!
 * down key control code
 */
void down_function() {
    if (frame_y >= 48 && bar_y >= 48) {
        frame_y_trg = 48;
        bar_y_trg = 48;
        if (mainMenuStart < lastMainmenuStart) {   /*---------one page only display 4 menu item-------------*/
            mainMenuStart++;
        }
    } else {
        frame_y_trg += 16;
        bar_y_trg += 16;
    }
}

int controlBar(u8g2_t *u8g2) {
    MenuState = Main_Menu;
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
            up_function();
            break;
        case down:
            LED_Toggle();
            down_function();
            break;
        case left:
//            showMenu(u8g2,SystemSubmenu,4,0);
            break;
        case right:

            break;
        case middle:
            break;
        default:
            break;
    }
    return mainMenuStart;
}

void scrollMenu(u8g2_t *u8g2) {
    int ms = controlBar(u8g2);         /*------ms:menu start location-------------*/
//    showMainMenu(u8g2, &ms);
    showMenu(u8g2,mainmenu,11,&ms);
}

void allproces()
{

}


