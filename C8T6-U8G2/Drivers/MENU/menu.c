//
// Created by JLF on 2023/12/2.
//

#include "menu.h"

/*!
 *  initialize select frame value and bar value
 */
short frame_y = 0, frame_y_trg = 0;
short bar_y = 0, bar_y_trg = 0;


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
        u8g2_DrawStr(u8g2, 5, (i - *MenuStart + 1) * 15, mainmenu[i].label);
    }
    u8g2_SetFont(u8g2, u8g2_font_t0_16_mf);
    select_bar(u8g2);
    u8g2_SendBuffer(u8g2);
}



