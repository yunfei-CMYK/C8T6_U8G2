//
// Created by JLF on 2023/12/2.
//

#include "menu.h"


KEYS keyState = KEY_NONE;
/*!
 *  initialize select frame value and bar value
 */
short frame_y = 0, frame_y_trg = 0;
short bar_y = 0, bar_y_trg = 0;

/*-------------------Draw menu content----------------*/
int8_t menubase_x = 5;
int8_t menubase_y = 15;


/*-------------------Box coordinate--------------------*/
short current_x = 0;
short current_y = 0;
short target_x = 0;
short target_y = 0;
uint8_t Box_CurW = 16;
uint8_t Box_CurH = 16;

uint8_t key = 0;
int mainMenuStart = 0;

/*--------------default display Main_Menu-----------------*/
MenuState currentMenuState = Main_Menu;

M_SELECT mainmenu[] = {
        {"System"},
        {"Devices"},
        {"Network"},
        {"Personalize"},
        {"Application"},
        {"Account"},
        {"Game"},
        {"Privacy"},
        {"Time & Date"},
        {"Update"},
        {"About"},
};

M_SELECT SystemSubmenu[] = {
        {"Display"},
        {"Sound"},
        {"Shake"},
        {"About"},
};

/*
 * @param u8g2
 * this function is draw select_bar
 */
void select_bar(u8g2_t *u8g2) {
    u8g2_DrawLine(u8g2, 127, 0, 127, 63);
    u8g2_DrawLine(u8g2, 125, 15, 127, 15);
    u8g2_DrawLine(u8g2, 125, 31, 127, 31);
    u8g2_DrawLine(u8g2, 125, 47, 127, 47);
    u8g2_DrawLine(u8g2, 125, 63, 127, 63);
    u8g2_DrawRBox(u8g2, 0, frame_y, 110, 17, 2);      //xuan ze kuang
    u8g2_DrawBox(u8g2, 125, bar_y, 2, 17);     //jin du tiao
    animation(&frame_y, &frame_y_trg, 1, 8);
    animation(&bar_y, &bar_y_trg, 1, 8);
    u8g2_SetDrawColor(u8g2, 2);
}

void showMainMenu(u8g2_t *u8g2, const int *mainMenuStart) {
    u8g2_ClearBuffer(u8g2);
    int mainMenu_len = sizeof(mainmenu) / sizeof(mainmenu[0]);
    for (int i = *mainMenuStart; i < mainMenu_len; i++) {
        u8g2_DrawStr(u8g2, 5, (i - *mainMenuStart + 1) * 15, mainmenu[i].label);
    }
    u8g2_SetFont(u8g2, u8g2_font_t0_16_mf);
    select_bar(u8g2);
    u8g2_SendBuffer(u8g2);
}


/*!
 *
 * @param u8g2
 * @param menu
 * @param menulen    menu real len
 * @param MenuStart
 */
void showMenu(u8g2_t *u8g2, const M_SELECT *menu, int menulen, const int *MenuStart)
{
    u8g2_ClearBuffer(u8g2);
    for (int i = *MenuStart; i < menulen; ++i) {
        u8g2_DrawStr(u8g2, menubase_x, (i - *MenuStart + 1) * menubase_y, mainmenu[i].label);
    }
    u8g2_SetFont(u8g2, u8g2_font_t0_16_mf);
    select_bar(u8g2);
    u8g2_SendBuffer(u8g2);
}

int animation(short *cur, const short *trg, uint8_t step, uint8_t slow_cnt) {
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
    showMenu(u8g2,mainmenu,11,&ms);
}


