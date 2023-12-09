//
// Created by JLF on 2023/12/2.
//
#include "menu.h"

/*!
 *  initialize select frame value and bar value
 */
short frame_y = 0, frame_y_trg = 0;
short bar_y = 0, bar_y_trg = 0;

/*-------------------Draw menu content----------------*/
int8_t menubase_x = 5;
int8_t menubase_y = 15;


/*-------------------Box coordinate--------------------*/
short target_x = 0;
short target_y = 0;

uint8_t key = 0;


int MenuStart = 0;
int MenuLastStart = 7;

uint8_t MenuItemNum = 10;

int16_t ui_select = 0;

MenuState state = Main_Menu;

/*--------------------------menuitem callback function-----------------*/


/*--------------default display Main_Menu-----------------*/
MenuState currentMenuState = Main_Menu;

M_SELECT Mainmenu[] = {
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

M_SELECT Systemmenu[] = {
        {"Display"},
        {"Sound"},
        {"Shake"},
        {"About"},
        {"Keyboard"},
        {"Mouse"},
        {"Earphone"},
        {"ST-Link"},
};

M_SELECT Devicesmenu[]= {
        {"Keyboard"},
        {"Mouse"},
        {"Earphone"},
        {"ST-Link"},
};

M_SELECT Networkmenu[]= {
        {"Network-Status"},
        {"WLAN"},
        {"Mobile-Data"},
        {"Reset"},
};

M_SELECT Personalizemenu[]= {
        {"Theme"},
        {"Wallpaper"},
        {"Font-Size"},
        {"Ringtone"},
};

M_SELECT Applicationmenu[]= {
        {"App-Permissions"},
        {"Default-Apps"},
        {"Notifications"},
        {"Storage-Usage"},
};

M_SELECT Accountmenu[]= {
        {"Sign-in"},
        {"Account-Info"},
        {"Payment-Card"},
        {"History"},
};

M_SELECT Gamemenu[]= {
        {"Graphics-Setting"},
        {"Sound-Setting"},
        {"Difficulty-Level"},
        {"Achievements"},
};
M_SELECT Privacymenu[]= {
        {"Privacy-Policy"},
        {"Data-Collection"},
        {"Data-Sharing"},
        {"Security"},
};
M_SELECT TimeDatemenu[]= {
        {"Date"},
        {"Time"},
        {"12/24 Hour"},
        {"Date Format"},
};
M_SELECT Updatemenu[]= {
        {"Check-Update"},
        {"Auto-Update"},
        {"Pure-Mode"},
        {"Backup"},
};
M_SELECT Aboutmenu[]= {
        {"Device-Info"},
        {"Legal-Info"},
        {"Version-Info"},
        {"Powered-by-JLF"},
};
/*!
 *
 * @param cur
 * @param trg
 * @param step
 * @param slow_cnt
 * @return
 */
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
 * @param u8g2
 * this function is draw select_bar
 */
void Progress_bar(u8g2_t *u8g2) {
    u8g2_DrawLine(u8g2, 127, 0, 127, 63);
    u8g2_DrawLine(u8g2, 125, 15, 127, 15);
    u8g2_DrawLine(u8g2, 125, 31, 127, 31);
    u8g2_DrawLine(u8g2, 125, 47, 127, 47);
    u8g2_DrawLine(u8g2, 125, 63, 127, 63);
    u8g2_DrawRBox(u8g2, 0, frame_y, 110, 17, 2);      //xuan ze kuang
    u8g2_DrawBox(u8g2, 125, bar_y, 2, 17);     //Progress bar
    animation(&frame_y, &frame_y_trg, 1, 8);
    animation(&bar_y, &bar_y_trg, 1, 8);
    u8g2_SetDrawColor(u8g2, 2);
}

/*!
 *
 * @param u8g2
 * @param menu
 * @param menulen    menu real len
 * @param MenuStart
 */
void printMenu(u8g2_t *u8g2, const M_SELECT *menu, int menulen, const int *MenuStart) {
    u8g2_ClearBuffer(u8g2);
    for (int i = *MenuStart; i < menulen; ++i) {
        u8g2_DrawStr(u8g2, menubase_x, (i - *MenuStart + 1) * menubase_y, menu[i].label);
    }
    u8g2_SetFont(u8g2, u8g2_font_t0_16_mf);
    Progress_bar(u8g2);
    u8g2_SendBuffer(u8g2);
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
    if (frame_y <= Top_Y && bar_y <= Top_Y) {
        frame_y_trg = Top_Y;
        bar_y_trg = Top_Y;
        if (ui_select > 0) ui_select = 0;
        if (MenuStart > 0) {
            MenuStart --;
        }
    } else {
        frame_y_trg -= Step;
        bar_y_trg -= Step;
        ui_select--;
    }
}

/*!
 * down key control code
 */
void down_function() {
    if (frame_y >= 48 && bar_y >= 48) {
        frame_y_trg = 48;
        bar_y_trg = 48;
        if (MenuStart < MenuLastStart) {   /*---------one page only display 4 menu item-------------*/
            MenuStart++;
        }

    } else {
        frame_y_trg += 16;
        bar_y_trg += 16;
    }
    if (ui_select < MenuItemNum)
    {
        ui_select++;
    }
}


void check() {
    short max_y = Bottom_Y;  // 最大的y坐标值
    short min_y = 0;   // 最小的y坐标值
    // 在处理按键之前再次检查当前的frame_y和bar_y是否越界
    if (frame_y < min_y && bar_y < min_y) {
        frame_y = min_y;
        bar_y = min_y;
    } else if (frame_y > max_y && bar_y > min_y) {
        frame_y = max_y;
        bar_y = max_y;
    }
}


/*!
 *
 * @param u8g2
 * @param menu
 * @param menulen
 */
//void scrollMenu(u8g2_t *u8g2, const M_SELECT menu, int menulen) {
//    printMenu(u8g2, &menu, menulen, &MenuStart);
//}

void controlKey(u8g2_t *u8g2) {
    key = myScan(0);
    check();
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
            state = Main_Menu;
            break;
        case right:
            switch (ui_select) {
                case 0:
                    state = System_Menu;
                    LED_Toggle();
                    break;
                case 1:
                    state = Devices_Menu;
                    LED_Toggle();
                    break;
                case 2:
                    state = Network_Menu;
                    LED_Toggle();
                    break;
                case 3:
                    state = Personalize_Menu;
                    LED_Toggle();
                    break;
                case 4:
                    state = Application_Menu;
                    LED_Toggle();
                    break;
                case 5:
                    state = Account_Menu;
                    LED_Toggle();
                    break;
                case 6:
                    state = Game_Menu;
                    LED_Toggle();
                    break;
                case 7:
                    state = Privacy_Menu;
                    LED_Toggle();
                    break;
                case 8:
                    state = Time_Date_Menu;
                    LED_Toggle();
                    break;
                case 9:
                    state = Update_Menu;
                    LED_Toggle();
                    break;
                case 10:
                    state = About_Menu;
                    LED_Toggle();
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    scrollMenu(u8g2);
}
void scrollMenu(u8g2_t *u8g2) {
    if(state == Main_Menu)
    {
        int num = sizeof(Mainmenu) / sizeof(Mainmenu[0]);
        printMenu(u8g2, Mainmenu, num, &MenuStart);
    }
    else if (state == System_Menu)
    {
        int num = sizeof(Systemmenu) / sizeof(Systemmenu[0]);
        printMenu(u8g2, Systemmenu, num, &MenuStart);
    }
    else if(state == Devices_Menu)
    {
        int num = sizeof(Devicesmenu) / sizeof(Devicesmenu[0]);
        printMenu(u8g2, Devicesmenu, num, &MenuStart);

    }
    else if (state == Network_Menu){
        int num = sizeof(Networkmenu) / sizeof(Networkmenu[0]);
        printMenu(u8g2, Networkmenu, num, &MenuStart);
    }
    else if (state == Personalize_Menu){
        int num = sizeof(Personalizemenu) / sizeof(Personalizemenu[0]);
        printMenu(u8g2, Personalizemenu, num, &MenuStart);
    }
    else if (state == Application_Menu){
        int num = sizeof(Applicationmenu) / sizeof(Applicationmenu[0]);
        printMenu(u8g2, Applicationmenu, num, &MenuStart);
    }
    else if (state == Account_Menu){
        int num = sizeof(Accountmenu) / sizeof(Accountmenu[0]);
        printMenu(u8g2, Accountmenu, num, &MenuStart);
    }
    else if (state == Game_Menu){
        int num = sizeof(Gamemenu) / sizeof(Gamemenu[0]);
        printMenu(u8g2, Gamemenu, num, &MenuStart);
    }
    else if (state == Privacy_Menu){
        int num = sizeof(Privacymenu) / sizeof(Privacymenu[0]);
        printMenu(u8g2, Privacymenu, num, &MenuStart);
    }
    else if (state == Time_Date_Menu){
        int num = sizeof(TimeDatemenu) / sizeof(TimeDatemenu[0]);
        printMenu(u8g2, TimeDatemenu, num, &MenuStart);
    }
    else if (state == Update_Menu){
        int num = sizeof(Updatemenu) / sizeof(Updatemenu[0]);
        printMenu(u8g2, Updatemenu, num, &MenuStart);
    }
    else if (state == About_Menu){
        int num = sizeof(Aboutmenu) / sizeof(Aboutmenu[0]);
        printMenu(u8g2, Aboutmenu, num, &MenuStart);
    }
}

void InitParameters()
{
    ui_select = 0;
    MenuStart = 0;
    frame_y_trg = 0;
    bar_y_trg = 0;
}

void testcontrol(u8g2_t *u8g2)
{
    controlKey(u8g2);
}
void StateControl(u8g2_t *u8g2)
{
    switch (state) {
        case Main_Menu:
            InitParameters();
            MenuLastStart = 7;
            MenuItemNum = 10;
            do {
                controlKey(u8g2);
                if(state != Main_Menu){
                    u8g2_ClearBuffer(u8g2);
                    break;
                }
            } while (1);
            break;
        case System_Menu:
            InitParameters();
            MenuLastStart = 4;
            MenuItemNum = 7;
            do {
                controlKey(u8g2);
                if(state != System_Menu){
                    u8g2_ClearBuffer(u8g2);
                    break;
                }
            } while (1);
            break;
        case Devices_Menu:
            InitParameters();
            MenuLastStart = 0;
            MenuItemNum = 3;
            do {
                controlKey(u8g2);
                if(state != Devices_Menu){
                    u8g2_ClearBuffer(u8g2);
                    break;
                }
            } while (1);
            break;
        case Network_Menu:
            InitParameters();
            MenuLastStart = 0;
            MenuItemNum = 3;
            do {
                controlKey(u8g2);
                if(state != Network_Menu){
                    u8g2_ClearBuffer(u8g2);
                    break;
                }
            } while (1);
            break;
        case Personalize_Menu:
            InitParameters();
            MenuLastStart = 0;
            MenuItemNum = 3;
            do {
                controlKey(u8g2);
                if(state != Personalize_Menu){
                    u8g2_ClearBuffer(u8g2);
                    break;
                }
            } while (1);
            break;
        case Application_Menu:
            InitParameters();
            MenuLastStart = 0;
            MenuItemNum = 3;
            do {
                controlKey(u8g2);
                if(state != Application_Menu){
                    u8g2_ClearBuffer(u8g2);
                    break;
                }
            } while (1);
            break;
        case Account_Menu:
            InitParameters();
            MenuLastStart = 0;
            MenuItemNum = 3;
            do {
                controlKey(u8g2);
                if(state != Account_Menu){
                    u8g2_ClearBuffer(u8g2);
                    break;
                }
            } while (1);
            break;
        case Game_Menu:
            InitParameters();
            MenuLastStart = 0;
            MenuItemNum = 3;
            do {
                controlKey(u8g2);
                if(state != Game_Menu){
                    u8g2_ClearBuffer(u8g2);
                    break;
                }
            } while (1);
            break;
        case Privacy_Menu:
            InitParameters();
            MenuLastStart = 0;
            MenuItemNum = 3;
            do {
                controlKey(u8g2);
                if(state != Privacy_Menu){
                    u8g2_ClearBuffer(u8g2);
                    break;
                }
            } while (1);
            break;
        case Time_Date_Menu:
            InitParameters();
            MenuLastStart = 0;
            MenuItemNum = 3;
            do {
                controlKey(u8g2);
                if(state != Time_Date_Menu){
                    u8g2_ClearBuffer(u8g2);
                    break;
                }
            } while (1);
            break;
        case Update_Menu:
            InitParameters();
            MenuLastStart = 0;
            MenuItemNum = 3;
            do {
                controlKey(u8g2);
                if(state != Update_Menu){
                    u8g2_ClearBuffer(u8g2);
                    break;
                }
            } while (1);
            break;
        case About_Menu:
            InitParameters();
            MenuLastStart = 0;
            MenuItemNum = 3;
            do {
                controlKey(u8g2);
                if(state != About_Menu){
                    u8g2_ClearBuffer(u8g2);
                    break;
                }
            } while (1);
            break;
        default:
            break;
    }
}









