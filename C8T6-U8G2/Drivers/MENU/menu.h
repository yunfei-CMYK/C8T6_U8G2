//
// Created by JLF on 2023/12/2.
//

#ifndef C8T6_U8G2_MENU_H
#define C8T6_U8G2_MENU_H

#include "main.h"
#include "keyled.h"
#include "math.h"
#include "u8g2.h"

#define Screen_Width  128
#define Screen_Height  64
#define Top_Y       0
#define Step           16
#define Bottom_Y  48

/*------------------key value---------------------*/
#define up  1
#define down 2
#define left 3
#define right 4
#define middle 5
#define lastMainmenuStart 7     /*------last page 7,8,9,10-------*/

typedef struct {
    uint8_t id;
} Key;

typedef struct MENU
{
    const char *label;
}M_SELECT;


typedef enum {
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
}MenuState;


void Progress_bar(u8g2_t *u8g2);
void printMenu(u8g2_t *u8g2, const M_SELECT *menu, int menulen, const int *MenuStart);
int animation(short *cur, const short *trg, uint8_t step, uint8_t slow_cnt);
uint8_t myScan(int mode);
void up_function();
void down_function();
void savePreviousState();
void RecoverPreviousState();
void check();
void controlKey(u8g2_t *u8g2);
void scrollMenu(u8g2_t *u8g2);
void InitParameters();
void StateControl(u8g2_t *u8g2);
#endif //C8T6_U8G2_MENU_H
