//
// Created by JLF on 2023/12/2.
//

#ifndef C8T6_U8G2_MENU_H
#define C8T6_U8G2_MENU_H

#include "u8g2.h"
#include "UI.h"

extern short frame_y,frame_y_trg;
extern short bar_y,bar_y_trg;


typedef struct MENU
{
    const char *label;
}M_SELECT;

void select_bar(u8g2_t *u8g2);
void showMainMenu(u8g2_t *u8g2, const int *mentstart);

#endif //C8T6_U8G2_MENU_H
