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

extern M_SELECT mainmenu[];
extern M_SELECT SystemSubmenu[];

enum MenuState {
    Main_Menu = 0,
    System_Menu,
    Devices_Menu,
    Network_Menu,
    Personalize_Menu,
    Application_Menu,
    Account_Menu,
    Game_Menu,
    Privacy_Menu,
    Time_Date_Menu,
    Update_Menu,
    About_Menu,
};

void select_bar(u8g2_t *u8g2);
void showMainMenu(u8g2_t *u8g2, const int *mainMenuStart);
void showMenu(u8g2_t *u8g2, const M_SELECT *menu, int menulen, const int *MenuStart);

#endif //C8T6_U8G2_MENU_H
