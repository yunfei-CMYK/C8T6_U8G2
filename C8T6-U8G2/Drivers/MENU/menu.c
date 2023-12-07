//
// Created by JLF on 2023/12/2.
//

#include "menu.h"

#define MENU_SIZE 9
#define DISPLAY_SIZE 4

short frame_y = 0, frame_y_trg = 0;
short bar_y = 0, bar_y_trg = 0;

M_SELECT mainmenu[] = {
        {"Sleep"},
        {"Editor"},
        {"Volt"},
        {"Setting"},
        {"Good"},
        {"Bad"},
        {"JLF"},
        {"CY"},
        {"NULL"},
};

int currentSelection = 0;
int displayStart = 0;

void mydrawMenu(u8g2_t *u8g2) {
    u8g2_ClearBuffer(u8g2);
        int i;
        for (i = 0; i < DISPLAY_SIZE; i++) {
            int index = displayStart + i;
            if (index < MENU_SIZE) {
                if (index == currentSelection) {
                    u8g2_DrawStr(u8g2, 0, 10*(i+1), ">"); // 绘制选择框
                }
                u8g2_DrawStr(u8g2, 10, 10*(i+1), mainmenu[index].label); // 绘制菜单项名称
            }
        }
    u8g2_SendBuffer(u8g2);
}

void scrollMenu(int direction) {
    if (direction > 0) { // 如果方向为正，表示向下滚动
        if (currentSelection < MENU_SIZE - 1) {
            currentSelection++; // 更新当前选择项
            if (currentSelection >= displayStart + DISPLAY_SIZE) {
                displayStart++; // 如果当前选择项超出显示范围，整体菜单向上滚动
            }
        }
    } else { // 如果方向为负，表示向上滚动
        if (currentSelection > 0) {
            currentSelection--; // 更新当前选择项
            if (currentSelection < displayStart) {
                displayStart--; // 如果当前选择项超出显示范围，整体菜单向下滚动
            }
        }
    }
}
/*!
 *
 * @param u8g2
 * this function is draw select_bar
 */
void select_bar(u8g2_t *u8g2) {
    u8g2_DrawLine(u8g2, 127, 0, 127, 63);
    u8g2_DrawLine(u8g2, 125, 15, 127, 15);
    u8g2_DrawLine(u8g2, 125, 31, 127, 31);
    u8g2_DrawLine(u8g2, 125, 47, 127, 47);
    u8g2_DrawLine(u8g2, 125, 63, 127, 63);
    u8g2_DrawRBox(u8g2, 5, frame_y, 100, 17, 2);      //xuan ze kuang
    u8g2_DrawBox(u8g2, 125, bar_y, 2, 17);     //jin du tiao
    animation(&frame_y, &frame_y_trg, 1, 8);
    animation(&bar_y, &bar_y_trg, 1, 8);
    u8g2_SetDrawColor(u8g2, 2);
}

void showMainMenu(u8g2_t *u8g2) {
    u8g2_ClearBuffer(u8g2);
    int mainMenu_len = sizeof(mainmenu) / sizeof(mainmenu[0]);
    for (int i = 0; i < mainMenu_len; i++) {
        u8g2_DrawStr(u8g2, 20, (i + 1) * 15, mainmenu[i].label);
    }
    u8g2_SetFont(u8g2, u8g2_font_t0_16_mf);
    select_bar(u8g2);
    u8g2_SendBuffer(u8g2);
}

