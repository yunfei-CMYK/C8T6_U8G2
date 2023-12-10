//
// Created by JLF on 2023/12/2.
//
#include "menu.h"

/*-------------------Draw menu content basic location----------------*/
int8_t menubase_x = 5;
int8_t menubase_y = 15;


/*-------------------Box coordinate--------------------*/
short frame_y = 0, frame_y_trg = 0;
short bar_y = 0, bar_y_trg = 0;
short frame_width = 60, frame_width_trg = 60;

uint8_t key = 0;

int suitable_len = 10;
/*-------------menu state parameter------------*/

int MenuStart = 0;
int MenuLastStart = 7;

uint8_t MenuItemNum = 10;

int ui_select = 0;


MenuState state = Main_Menu;


/*---------saves the state of the previous menu*/
int previousMenuStart = 0;
int previous_ui_select = 0;
int previous_frame_y_trg = 0;
int previous_bar_y_trg = 0;


/*--------------default display Main_Menu-----------------*/
MenuState currentMenuState = Main_Menu;

M_SELECT Mainmenu[] = {
        {"-System"},
        {"-Devices"},
        {"-Network"},
        {"-Personalize"},
        {"-Application"},
        {"-Account"},
        {"-Game"},
        {"-Privacy"},
        {"-Time & Date"},
        {"-Update"},
        {"-About"},
};

M_SELECT Systemmenu[] = {
        {"Display"},
        {"Sound"},
        {"Shake"},
        {"System-Info"},
        {"Storage"},
        {"Power"},
        {"Security"},
        {"STM32F103C8T6"},
};

M_SELECT Devicesmenu[] = {
        {"Keyboard"},
        {"Mouse"},
        {"Earphone"},
        {"ST-Link"},
        {"Screen"},
        {"USB-C"},
        {"Camera"},
        {"LEGION"},
};

M_SELECT Networkmenu[] = {
        {"Network-Status"},
        {"WLAN"},
        {"Mobile-Data"},
        {"DNS"},
        {"Proxy"},
        {"MAC-Address"},
        {"Wireless"},
        {"Reset"},
};

M_SELECT Personalizemenu[] = {
        {"Theme"},
        {"Wallpaper"},
        {"Font-Size"},
        {"Ringtone"},
        {"Color"},
        {"UI-Style"},
        {"TaskBar"},
        {"StartMenu"},
};

M_SELECT Applicationmenu[] = {
        {"Permissions"},
        {"Default-Apps"},
        {"Notifications"},
        {"Storage-Usage"},
        {"Advanced"},
        {"Offline-Map"},
        {"Video"},
        {"TikTok"},
};

M_SELECT Accountmenu[] = {
        {"Sign-in"},
        {"Account-Info"},
        {"Payment-Card"},
        {"History"},
        {"Family"},
        {"OtherID"},
        {"FaceID"},
        {"Fingerprint"},
};

M_SELECT Gamemenu[] = {
        {"Graphics"},
        {"Sound"},
        {"Level"},
        {"Achievements"},
        {"GTA-6"},
        {"Minecraft"},
        {"Halo"},
        {"LOL"},
};
M_SELECT Privacymenu[] = {
        {"Privacy-Policy"},
        {"Data-Collection"},
        {"Data-Sharing"},
        {"Security"},
        {"Location"},
        {"Microphone"},
        {"AD"},
        {"Search"},
};
M_SELECT TimeDatemenu[] = {
        {"Date"},
        {"Time"},
        {"Year"},
        {"Date Format"},
        {"12/24 Hour"},
        {"Date Format"},
        {"Auto-Setting"},
        {"Time-Zone"},
};
M_SELECT Updatemenu[] = {
        {"Check-Update"},
        {"Auto-Update"},
        {"Pure-Mode"},
        {"Backup"},
        {"Stop-Update"},
        {"Advanced"},
        {"Beta"},
        {"Stable"},
};
M_SELECT Aboutmenu[] = {
        {"Device-Info"},
        {"Legal-Info"},
        {"Version-Info"},
        {"Version:1.0"},
        {"Powered-by-JLF"},
        {"UID:212431220"},
        {"Support:CY"},
        {"I Love You-CY"},
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
//    u8g2_DrawRBox(u8g2, 0, frame_y, 120, 17, 2);      //xuan ze kuang
    u8g2_DrawRBox(u8g2, 0, frame_y, frame_width, 17, 2);      //xuan ze kuang
    u8g2_DrawBox(u8g2, 125, bar_y, 2, 17);     //Progress bar
    frame_width_trg = (short)getFrameWidth(u8g2);
    animation(&frame_y, &frame_y_trg, 4, 8);
    animation(&bar_y, &bar_y_trg, 4, 8);
    animation(&frame_width,&frame_width_trg,6,8);
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
//    u8g2_SetFont(u8g2,u8g2_font_wqy16_t_chinese2);
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
    keyvalue.id = 0;
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

/*
 * up key control code
 */
void up_function() {
    if (frame_y <= Top_Y && bar_y <= Top_Y) {
        frame_y_trg = Top_Y;
        bar_y_trg = Top_Y;
        if (MenuStart > 0) {
            MenuStart--;
        }
    } else {
        frame_y_trg -= Step;
        bar_y_trg -= Step;
    }
    if(ui_select>0)ui_select--;
}

/*
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
    if (ui_select < MenuItemNum) {
        ui_select++;
    }
}

/*-------------Dynamically gets the width of the selection box--------*/
int getFrameWidth(u8g2_t *u8g2)
{
    int num = ui_select;
    int Width = 60;
    switch (state) {
        case Main_Menu:
            Width = u8g2_GetStrWidth(u8g2,Mainmenu[num].label)+suitable_len;
            break;
        case System_Menu:
            Width = u8g2_GetStrWidth(u8g2,Systemmenu[num].label)+suitable_len;
            break;
        case Devices_Menu:
            Width = u8g2_GetStrWidth(u8g2,Devicesmenu[num].label)+suitable_len;
            break;
        case Network_Menu:
            Width = u8g2_GetStrWidth(u8g2,Networkmenu[num].label)+suitable_len;
            break;
        case Personalize_Menu:
            Width = u8g2_GetStrWidth(u8g2,Personalizemenu[num].label)+suitable_len;
            break;
        case Application_Menu:
            Width = u8g2_GetStrWidth(u8g2,Applicationmenu[num].label)+suitable_len;
            break;
        case Account_Menu:
            Width = u8g2_GetStrWidth(u8g2,Accountmenu[num].label)+suitable_len;
            break;
        case Game_Menu:
            Width = u8g2_GetStrWidth(u8g2,Gamemenu[num].label)+suitable_len;
            break;
        case Privacy_Menu:
            Width = u8g2_GetStrWidth(u8g2,Privacymenu[num].label)+suitable_len;
            break;
        case Time_Date_Menu:
            Width = u8g2_GetStrWidth(u8g2,TimeDatemenu[num].label)+suitable_len;
            break;
        case Update_Menu:
            Width = u8g2_GetStrWidth(u8g2,Updatemenu[num].label)+suitable_len;
            break;
        case About_Menu:
            Width = u8g2_GetStrWidth(u8g2,Aboutmenu[num].label)+suitable_len;
            break;
        default:
            break;
    }
    frame_width_trg = (short)Width;
    return frame_width_trg;
}

/*-------------save the state of the previous menu-------------------*/
void savePreviousState() {
    previousMenuStart = MenuStart;
    previous_ui_select = ui_select;
    previous_frame_y_trg = frame_y_trg;
    previous_bar_y_trg = bar_y_trg;
}

/*-------------Recover the state of the previous menu-------------------*/
void RecoverPreviousState() {
    MenuStart = previousMenuStart;
    ui_select = previous_ui_select;
    frame_y_trg = (short)previous_frame_y_trg;
    bar_y_trg = (short)previous_bar_y_trg;
}

/*-------------check if the select frame and bar is crossed -------------------*/
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

/*
 * Perform a state toggle and move the selection box
 */
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
            InitParameters();
            break;
        case right:
            switch (ui_select) {
                case 0:
                    if (state == Main_Menu) {
                        savePreviousState();
                        state = System_Menu;
                    }
                    LED_Toggle();
                    break;
                case 1:
                    if (state == Main_Menu) {
                        savePreviousState();
                        state = Devices_Menu;
                    }
                    LED_Toggle();
                    break;
                case 2:
                    if (state == Main_Menu) {
                        savePreviousState();
                        state = Network_Menu;
                    }
                    LED_Toggle();
                    break;
                case 3:
                    if (state == Main_Menu) {
                        savePreviousState();
                        state = Personalize_Menu;
                    }
                    LED_Toggle();
                    break;
                case 4:
                    if (state == Main_Menu) {
                        savePreviousState();
                        state = Application_Menu;
                    }
                    LED_Toggle();
                    break;
                case 5:
                    if (state == Main_Menu) {
                        savePreviousState();
                        state = Account_Menu;
                    }
                    LED_Toggle();
                    break;
                case 6:
                    if (state == Main_Menu) {
                        savePreviousState();
                        state = Game_Menu;
                    }
                    LED_Toggle();
                    break;
                case 7:
                    if (state == Main_Menu) {
                        savePreviousState();
                        state = Privacy_Menu;
                    }
                    LED_Toggle();
                    break;
                case 8:
                    if (state == Main_Menu) {
                        savePreviousState();
                        state = Time_Date_Menu;
                    }
                    LED_Toggle();
                    break;
                case 9:
                    if (state == Main_Menu) {
                        savePreviousState();
                        state = Update_Menu;
                    }
                    LED_Toggle();
                    break;
                case 10:
                    if (state == Main_Menu) {
                        savePreviousState();
                        state = About_Menu;
                    }
                    LED_Toggle();
                    break;
                default:
                    break;
            }
            break;
        case middle:
            /* test function*/
            if (state == System_Menu && ui_select == 0)
            {
                LED_Toggle();
            }
            break;
        default:
            break;
    }
    scrollMenu(u8g2);
}

void scrollMenu(u8g2_t *u8g2) {
    if (state == Main_Menu) {
        int num = sizeof(Mainmenu) / sizeof(Mainmenu[0]);
        printMenu(u8g2, Mainmenu, num, &MenuStart);
    } else if (state == System_Menu) {
        int num = sizeof(Systemmenu) / sizeof(Systemmenu[0]);
        printMenu(u8g2, Systemmenu, num, &MenuStart);
    } else if (state == Devices_Menu) {
        int num = sizeof(Devicesmenu) / sizeof(Devicesmenu[0]);
        printMenu(u8g2, Devicesmenu, num, &MenuStart);

    } else if (state == Network_Menu) {
        int num = sizeof(Networkmenu) / sizeof(Networkmenu[0]);
        printMenu(u8g2, Networkmenu, num, &MenuStart);
    } else if (state == Personalize_Menu) {
        int num = sizeof(Personalizemenu) / sizeof(Personalizemenu[0]);
        printMenu(u8g2, Personalizemenu, num, &MenuStart);
    } else if (state == Application_Menu) {
        int num = sizeof(Applicationmenu) / sizeof(Applicationmenu[0]);
        printMenu(u8g2, Applicationmenu, num, &MenuStart);
    } else if (state == Account_Menu) {
        int num = sizeof(Accountmenu) / sizeof(Accountmenu[0]);
        printMenu(u8g2, Accountmenu, num, &MenuStart);
    } else if (state == Game_Menu) {
        int num = sizeof(Gamemenu) / sizeof(Gamemenu[0]);
        printMenu(u8g2, Gamemenu, num, &MenuStart);
    } else if (state == Privacy_Menu) {
        int num = sizeof(Privacymenu) / sizeof(Privacymenu[0]);
        printMenu(u8g2, Privacymenu, num, &MenuStart);
    } else if (state == Time_Date_Menu) {
        int num = sizeof(TimeDatemenu) / sizeof(TimeDatemenu[0]);
        printMenu(u8g2, TimeDatemenu, num, &MenuStart);
    } else if (state == Update_Menu) {
        int num = sizeof(Updatemenu) / sizeof(Updatemenu[0]);
        printMenu(u8g2, Updatemenu, num, &MenuStart);
    } else if (state == About_Menu) {
        int num = sizeof(Aboutmenu) / sizeof(Aboutmenu[0]);
        printMenu(u8g2, Aboutmenu, num, &MenuStart);
    }
}

/*--------page init---------------*/
void InitParameters() {
    ui_select = 0;
    MenuStart = 0;
    frame_y_trg = 0;
    bar_y_trg = 0;
}

void StateControl(u8g2_t *u8g2) {
    switch (state) {
        case Main_Menu:
            InitParameters();
            RecoverPreviousState();
            MenuLastStart = 7;
            MenuItemNum = 10;
            do {
                controlKey(u8g2);
                if (state != Main_Menu) {
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
                if (state != System_Menu) {
                    u8g2_ClearBuffer(u8g2);
                    break;
                }
            } while (1);
            break;
        case Devices_Menu:
            InitParameters();
            MenuLastStart = 4;
            MenuItemNum = 7;
            do {
                controlKey(u8g2);
                if (state != Devices_Menu) {
                    u8g2_ClearBuffer(u8g2);
                    break;
                }
            } while (1);
            break;
        case Network_Menu:
            InitParameters();
            MenuLastStart = 4;
            MenuItemNum = 7;
            do {
                controlKey(u8g2);
                if (state != Network_Menu) {
                    u8g2_ClearBuffer(u8g2);
                    break;
                }
            } while (1);
            break;
        case Personalize_Menu:
            InitParameters();
            MenuLastStart = 4;
            MenuItemNum = 7;
            do {
                controlKey(u8g2);
                if (state != Personalize_Menu) {
                    u8g2_ClearBuffer(u8g2);
                    break;
                }
            } while (1);
            break;
        case Application_Menu:
            InitParameters();
            MenuLastStart = 4;
            MenuItemNum = 7;
            do {
                controlKey(u8g2);
                if (state != Application_Menu) {
                    u8g2_ClearBuffer(u8g2);
                    break;
                }
            } while (1);
            break;
        case Account_Menu:
            InitParameters();
            MenuLastStart = 4;
            MenuItemNum = 7;
            do {
                controlKey(u8g2);
                if (state != Account_Menu) {
                    u8g2_ClearBuffer(u8g2);
                    break;
                }
            } while (1);
            break;
        case Game_Menu:
            InitParameters();
            MenuLastStart = 4;
            MenuItemNum = 7;
            do {
                controlKey(u8g2);
                if (state != Game_Menu) {
                    u8g2_ClearBuffer(u8g2);
                    break;
                }
            } while (1);
            break;
        case Privacy_Menu:
            InitParameters();
            MenuLastStart = 4;
            MenuItemNum = 7;
            do {
                controlKey(u8g2);
                if (state != Privacy_Menu) {
                    u8g2_ClearBuffer(u8g2);
                    break;
                }
            } while (1);
            break;
        case Time_Date_Menu:
            InitParameters();
            MenuLastStart = 4;
            MenuItemNum = 7;
            do {
                controlKey(u8g2);
                if (state != Time_Date_Menu) {
                    u8g2_ClearBuffer(u8g2);
                    break;
                }
            } while (1);
            break;
        case Update_Menu:
            InitParameters();
            MenuLastStart = 4;
            MenuItemNum = 7;
            do {
                controlKey(u8g2);
                if (state != Update_Menu) {
                    u8g2_ClearBuffer(u8g2);
                    break;
                }
            } while (1);
            break;
        case About_Menu:
            InitParameters();
            MenuLastStart = 4;
            MenuItemNum = 7;
            do {
                controlKey(u8g2);
                if (state != About_Menu) {
                    u8g2_ClearBuffer(u8g2);
                    break;
                }
            } while (1);
            break;
        default:
            break;
    }
}

void StartUIDisplay(u8g2_t *u8g2)
{
    for(int i=10;i<=100;i=i+1)
    {
        u8g2_ClearBuffer(u8g2);

        char buff[20];

        u8g2_SetFont(u8g2,u8g2_font_ncenB12_tf);
        u8g2_DrawStr(u8g2,32,32,"JLF-UI");//字符显示

        u8g2_DrawRBox(u8g2,16,40,i,10,4);//圆角填充框矩形框
        u8g2_DrawRFrame(u8g2,16,40,100,10,5);//圆角矩形

        u8g2_SendBuffer(u8g2);
        HAL_Delay(25);
    }
    u8g2_ClearBuffer(u8g2);
    u8g2_SetFont(u8g2,u8g2_font_t0_16_mf);
    u8g2_DrawStr(u8g2,12,32,"Press the key");//字符显示
    u8g2_SendBuffer(u8g2);
}

void JLF_UI(u8g2_t *u8g2)
{
    int startKey = 0;
    startKey = myScan(0);
    while(startKey == middle)
    {
        StateControl(u8g2);
    }
}






