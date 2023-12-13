//
// Created by JLF on 2023/12/2.
//
#include "menu.h"
#include "icon.h"

/*-------------------Draw menu content basic location----------------*/
int8_t menubase_x = 5;
int8_t menubase_y = 15;

int8_t iconbase_x = 40;
int8_t iconbase_y = 4;


/*-------------------Box coordinate--------------------*/
short frame_y = 0, frame_y_trg = 0;
short bar_y = 0, bar_y_trg = 0;
short frame_width = 60, frame_width_trg = 60;

short IconBar_x = 8, IconBar_x_trg = 8;
short IconBar_y = 37,IconBar_y_trg = 37;

uint8_t key = 0;
uint8_t iconkey = 0;

int suitable_len = 10;
/*-------------menu state parameter------------*/

int MenuStart = 0;
int MenuLastStart = 4;

int IconStart = 0;
int IconLastStart = 8;

uint8_t MenuItemNum = 10;
uint8_t IconItemNum = 10;

int ui_select = 0;

MenuState state = Icon_Menu;


/*---------saves the state of the previous menu*/
int previousMenuStart = 0;
int previous_frame_y_trg = 0;
int previous_bar_y_trg = 0;
int previous_IconBar_x = 0;
int previous_ui_select = 0;
int previous_IconBar_x_trg = 0;
int previousIconStart = 0;

/*--------------default display Icon_Menu-----------------*/
Menu_Icon MenuIcon[] =
        {
                {all_icon[0],  "System"},
                {all_icon[1],  "Device"},
                {all_icon[2],  "Network"},
                {all_icon[3],  "Personalize"},
                {all_icon[4],  "Application"},
                {all_icon[5],  "Account"},
                {all_icon[6],  "Game"},
                {all_icon[7],  "Privacy"},
                {all_icon[8],  "Time&Date"},
                {all_icon[9],  "Update"},
                {all_icon[10], "About"},
        };
/*
 * if you want to increase the number of menus to be greater
 * than the main menu data,you can replace the value of 11
 * */
M_SELECT allMenu[][11] = {
        {
                {"Display"},
                {"Sound"},
                {"Shake"},
                {"System-Info"},
                {"Storage"},
                {"Power"},
                {"Security"},
                {"STM32F103C8T6"},
        },
        {
                {"Keyboard"},
                {"Mouse"},
                {"Earphone"},
                {"ST-Link"},
                {"Screen"},
                {"USB-C"},
                {"Camera"},
                {"LEGION"},
        },
        {
                {"Network-Status"},
                {"WLAN"},
                {"Mobile-Data"},
                {"DNS"},
                {"Proxy"},
                {"MAC-Address"},
                {"Wireless"},
                {"Reset"},
        },
        {
                {"Theme"},
                {"Wallpaper"},
                {"Font-Size"},
                {"Ringtone"},
                {"Color"},
                {"UI-Style"},
                {"TaskBar"},
                {"StartMenu"},
        },
        {
                {"Permissions"},
                {"Default-Apps"},
                {"Notifications"},
                {"Storage-Usage"},
                {"Advanced"},
                {"Offline-Map"},
                {"Video"},
                {"TikTok"},
        },
        {
                {"Sign-in"},
                {"Account-Info"},
                {"Payment-Card"},
                {"History"},
                {"Family"},
                {"OtherID"},
                {"FaceID"},
                {"Fingerprint"},
        },
        {
                {"Graphics"},
                {"Sound"},
                {"Level"},
                {"Achievements"},
                {"GTA-6"},
                {"Minecraft"},
                {"Halo"},
                {"LOL"},
        },
        {
                {"Privacy-Policy"},
                {"Data-Collection"},
                {"Data-Sharing"},
                {"Security"},
                {"Location"},
                {"Microphone"},
                {"AD"},
                {"Search"},
        },
        {
                {"Date"},
                {"Time"},
                {"Year"},
                {"Date Format"},
                {"12/24 Hour"},
                {"Date Format"},
                {"Auto-Setting"},
                {"Time-Zone"},
        },
        {
                {"Check-Update"},
                {"Auto-Update"},
                {"Pure-Mode"},
                {"Backup"},
                {"Stop-Update"},
                {"Advanced"},
                {"Beta"},
                {"Stable"},
        },

        {
                {"Device-Info"},
                {"Legal-Info"},
                {"Version-Info"},
                {"Version:1.3"},
                {"Powered-by-JLF"},
                {"UID:212431220"},
                {"Support:CY"},
                {"I Love You-CY"},
        },
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
    u8g2_DrawRBox(u8g2, 0, frame_y, frame_width, 17, 2);      //xuan ze kuang
    u8g2_DrawBox(u8g2, 125, bar_y, 2, 17);     //Progress bar
    frame_width_trg = (short) getFrameWidth(u8g2);
    animation(&frame_y, &frame_y_trg, 6, 8);
    animation(&bar_y, &bar_y_trg, 6, 8);
    animation(&frame_width, &frame_width_trg, 6, 8);
    u8g2_SetDrawColor(u8g2, 2);
}

/*!
 * @param u8g2
 * use Icon_bar to select icon
 */
void Icon_bar(u8g2_t *u8g2) {
    u8g2_DrawBox(u8g2, IconBar_x, IconBar_y, IconBar_Width, IconBar_Height);
    animation(&IconBar_x, &IconBar_x_trg, 8, 10);
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


void printIcon(u8g2_t *u8g2, int iconlen, const int *IconStart) {
    u8g2_ClearBuffer(u8g2);
    for (int i = *IconStart; i < iconlen; ++i) {
        u8g2_DrawXBMP(u8g2, (i - *IconStart) * iconbase_x + 8, iconbase_y, Icon_Width, Icon_Height, all_icon[i]);
    }
    Icon_bar(u8g2);
    printIconTitle(u8g2);
    u8g2_SendBuffer(u8g2);
}

void printIconTitle(u8g2_t *u8g2) {
    u8g2_SetFont(u8g2, u8g2_font_t0_16_mf);
    int i = ui_select;
    int title_width = getIconTitleWidth(u8g2);
    u8g2_DrawStr(u8g2, (u8g2_GetDisplayWidth(u8g2) - title_width) / 2, IconTitle_Y, MenuIcon[i].label);
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
    if (ui_select > 0)ui_select--;
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

void left_function() {
    if (IconBar_x <= IconStart_X) {
        IconBar_x_trg = IconStart_X;
        if (IconStart > 0) {
            IconStart--;
        }
    } else {
        IconBar_x_trg -= IconStep;
    }
    if (ui_select > 0)ui_select--;
}

void right_function() {
    if (IconBar_x >= IconEnd_X) {
        IconBar_x_trg = IconEnd_X;
        if (IconStart < IconLastStart) {
            IconStart++;
        }
    } else {
        IconBar_x_trg += IconStep;
    }
    if (ui_select < IconItemNum) {
        ui_select++;
    }
}

void mid_function()
{
    switch (ui_select) {
        case 0:
            if (state == Icon_Menu) {
                savePreviousState();
                state = System_Menu;
            }
            LED_Toggle();
            break;
        case 1:
            if (state == Icon_Menu) {
                savePreviousState();
                state = Devices_Menu;
            }
            LED_Toggle();
            break;
        case 2:
            if (state == Icon_Menu) {
                savePreviousState();
                state = Network_Menu;
            }
            LED_Toggle();
            break;
        case 3:
            if (state == Icon_Menu) {
                savePreviousState();
                state = Personalize_Menu;
            }
            LED_Toggle();
            break;
        case 4:
            if (state == Icon_Menu) {
                savePreviousState();
                state = Application_Menu;
            }
            LED_Toggle();
            break;
        case 5:
            if (state == Icon_Menu) {
                savePreviousState();
                state = Account_Menu;
            }
            LED_Toggle();
            break;
        case 6:
            if (state == Icon_Menu) {
                savePreviousState();
                state = Game_Menu;
            }
            LED_Toggle();
            break;
        case 7:
            if (state == Icon_Menu) {
                savePreviousState();
                state = Privacy_Menu;
            }
            LED_Toggle();
            break;
        case 8:
            if (state == Icon_Menu) {
                savePreviousState();
                state = Time_Date_Menu;
            }
            LED_Toggle();
            break;
        case 9:
            if (state == Icon_Menu) {
                savePreviousState();
                state = Update_Menu;
            }
            LED_Toggle();
            break;
        case 10:
            if (state == Icon_Menu) {
                savePreviousState();
                state = About_Menu;
            }
            LED_Toggle();
            break;
        default:
            break;
    }
}
/*
 *  to get Icon Title Width
 * */
int getIconTitleWidth(u8g2_t *u8g2) {
    int icon_num = ui_select;
    int iconTitle_width = 0;
    if (state == Icon_Menu) {
        iconTitle_width = u8g2_GetStrWidth(u8g2, MenuIcon[icon_num].label);
    }
    return iconTitle_width;
}

/*-------------Dynamically gets the width of the selection box--------*/
int getFrameWidth(u8g2_t *u8g2) {
    int num = ui_select;
    int Width = 0;
    switch (state) {
        case System_Menu:
            Width = u8g2_GetStrWidth(u8g2, allMenu[0][num].label) + suitable_len;
            break;
        case Devices_Menu:
            Width = u8g2_GetStrWidth(u8g2, allMenu[1][num].label) + suitable_len;
            break;
        case Network_Menu:
            Width = u8g2_GetStrWidth(u8g2, allMenu[2][num].label) + suitable_len;
            break;
        case Personalize_Menu:
            Width = u8g2_GetStrWidth(u8g2, allMenu[3][num].label) + suitable_len;
            break;
        case Application_Menu:
            Width = u8g2_GetStrWidth(u8g2, allMenu[4][num].label) + suitable_len;
            break;
        case Account_Menu:
            Width = u8g2_GetStrWidth(u8g2, allMenu[5][num].label) + suitable_len;
            break;
        case Game_Menu:
            Width = u8g2_GetStrWidth(u8g2, allMenu[6][num].label) + suitable_len;
            break;
        case Privacy_Menu:
            Width = u8g2_GetStrWidth(u8g2, allMenu[7][num].label) + suitable_len;
            break;
        case Time_Date_Menu:
            Width = u8g2_GetStrWidth(u8g2, allMenu[8][num].label) + suitable_len;
            break;
        case Update_Menu:
            Width = u8g2_GetStrWidth(u8g2, allMenu[9][num].label) + suitable_len;
            break;
        case About_Menu:
            Width = u8g2_GetStrWidth(u8g2, allMenu[10][num].label) + suitable_len;
            break;
        default:
            break;
    }
    frame_width_trg = (short) Width;
    return frame_width_trg;
}

/*-------------save the state of the previous menu-------------------*/
void savePreviousState() {
//    previousMenuStart = MenuStart;
//    previous_frame_y_trg = frame_y_trg;
//    previous_bar_y_trg = bar_y_trg;
    previous_ui_select = ui_select;
    previous_IconBar_x_trg = IconBar_x_trg;
    previousIconStart = IconStart;
}

/*-------------Recover the state of the previous menu-------------------*/
void RecoverPreviousState() {
//    MenuStart = previousMenuStart;
//    frame_y_trg = (short) previous_frame_y_trg;
//    bar_y_trg = (short) previous_bar_y_trg;
    ui_select = previous_ui_select;
    IconBar_x_trg = (short) previous_IconBar_x_trg;
    IconStart = previousIconStart;
}

/*-------------check if the select frame and bar is crossed -------------------*/
void check() {
    short max_y = Bottom_Y;  // 最大的y坐标值
    short min_y = Top_Y;   // 最小的y坐标值
    // 在处理按键之前再次检查当前的frame_y和bar_y是否越界
    if (frame_y < min_y && bar_y < min_y) {
        frame_y = min_y;
        bar_y = min_y;
    } else if (frame_y > max_y && bar_y > min_y) {
        frame_y = max_y;
        bar_y = max_y;
    }
}

void checkicon() {
    short IconBarmax_x = IconEnd_X;
    short IconBarmin_x = IconStart_X;
    if (IconBar_x < IconBarmin_x) {
        IconBar_x = IconBarmin_x;
    } else if (IconBar_x > IconBarmax_x) {
        IconBar_x = IconBarmax_x;
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
            LED_Toggle();
            if(state != Icon_Menu)
            {
                state = Icon_Menu;
//                InitParameters();
            }
            break;
        default:
            break;
    }
    scrollMenu(u8g2);
}

void controlIconKey(u8g2_t *u8g2) {
    iconkey = myScan(0);
    checkicon();
    switch (iconkey) {
        case left:
            LED_Toggle();
            if (state == Icon_Menu) {
                left_function();
            }else{

            }
            break;
        case right:
            LED_Toggle();
            if (state == Icon_Menu) {
                right_function();
            }else{

            }
            break;
        case middle:
            /* change state function*/
            LED_Toggle();
            mid_function();
            break;
        default:
            break;
    }
    scrollIcon(u8g2);
}

/*---------------------------Scroll the icon-----------------------*/
void scrollIcon(u8g2_t *u8g2) {
    if (state == Icon_Menu) {
        int num = sizeof(all_icon) / sizeof(all_icon[0]);
        printIcon(u8g2, num, &IconStart);
        printIconTitle(u8g2);
    }
}

/*-------------------------Scroll the submenu---------------------*/
void scrollMenu(u8g2_t *u8g2) {
    if (state == System_Menu) {
        int num = sizeof(allMenu[0]) / sizeof(allMenu[0][0]);
        printMenu(u8g2, allMenu[0], num, &MenuStart);
    } else if (state == Devices_Menu) {
        int num = sizeof(allMenu[1]) / sizeof(allMenu[1][0]);
        printMenu(u8g2, allMenu[1], num, &MenuStart);
    } else if (state == Network_Menu) {
        int num = sizeof(allMenu[2]) / sizeof(allMenu[2][0]);
        printMenu(u8g2, allMenu[2], num, &MenuStart);
    } else if (state == Personalize_Menu) {
        int num = sizeof(allMenu[3]) / sizeof(allMenu[3][0]);
        printMenu(u8g2, allMenu[3], num, &MenuStart);
    } else if (state == Application_Menu) {
        int num = sizeof(allMenu[4]) / sizeof(allMenu[4][0]);
        printMenu(u8g2, allMenu[4], num, &MenuStart);
    } else if (state == Account_Menu) {
        int num = sizeof(allMenu[5]) / sizeof(allMenu[5][0]);
        printMenu(u8g2, allMenu[5], num, &MenuStart);
    } else if (state == Game_Menu) {
        int num = sizeof(allMenu[6]) / sizeof(allMenu[6][0]);
        printMenu(u8g2, allMenu[6], num, &MenuStart);
    } else if (state == Privacy_Menu) {
        int num = sizeof(allMenu[7]) / sizeof(allMenu[7][0]);
        printMenu(u8g2, allMenu[7], num, &MenuStart);
    } else if (state == Time_Date_Menu) {
        int num = sizeof(allMenu[8]) / sizeof(allMenu[8][0]);
        printMenu(u8g2, allMenu[8], num, &MenuStart);
    } else if (state == Update_Menu) {
        int num = sizeof(allMenu[9]) / sizeof(allMenu[9][0]);
        printMenu(u8g2, allMenu[9], num, &MenuStart);
    } else if (state == About_Menu) {
        int num = sizeof(allMenu[10]) / sizeof(allMenu[10][0]);
        printMenu(u8g2, allMenu[10], num, &MenuStart);
    }
}

/*--------page init---------------*/
void InitParameters() {
    ui_select = 0;
    MenuStart = 0;
    IconStart = 0;
    frame_y_trg = 0;
    bar_y_trg = 0;
    IconBar_x_trg = 8;
}

void StateControl(u8g2_t *u8g2) {
    switch (state) {
        case Icon_Menu:
            InitParameters();
            RecoverPreviousState();
            IconLastStart = 8;
            IconItemNum = 10;
            do {
                controlIconKey(u8g2);
                if (state != Icon_Menu) {
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

void StartUIDisplay(u8g2_t *u8g2) {
    for (int i = 10; i <= 100; i = i + 1) {
        u8g2_ClearBuffer(u8g2);

        u8g2_SetFont(u8g2, u8g2_font_ncenB12_tf);
        u8g2_DrawStr(u8g2, 12, 32, "Dragon & UI");//字符显示

        u8g2_DrawRBox(u8g2, 16, 40, i, 10, 4);//圆角填充框矩形框
        u8g2_DrawRFrame(u8g2, 16, 40, 100, 10, 5);//圆角矩形

        u8g2_SendBuffer(u8g2);
        HAL_Delay(10);         // control StartUI velocity
    }
    u8g2_ClearBuffer(u8g2);
    u8g2_SetFont(u8g2, u8g2_font_t0_16_mf);
    u8g2_DrawStr(u8g2, 12, 16, "Hello");//字符显示
    u8g2_DrawStr(u8g2, 12, 32, "Press the Mid");//字符显示
    u8g2_DrawStr(u8g2, 12, 48, "Enter the UI ");//字符显示
    u8g2_SendBuffer(u8g2);
}

void JLF_UI(u8g2_t *u8g2) {
    int startKey = 0;
    startKey = myScan(0);
    while (startKey == middle) {
        StateControl(u8g2);
    }
}






