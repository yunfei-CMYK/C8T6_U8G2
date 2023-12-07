//
// Created by JLF on 2023/12/1.
//

#ifndef C8T6_U8G2_UI_H
#define C8T6_U8G2_UI_H

#include "main.h"
#include "keyled.h"
#include "u8g2.h"
#include "menu.h"
#include "math.h"

#define Screen_Width  128
#define Screen_Height  64
#define Boundary       0
#define Step           16
#define Box_Width     16
#define Box_Height    16

#define up  1
#define down 2
#define left 3
#define right 4
#define middle 5

typedef struct {
    uint8_t id;
} Key;

int animation(short *cur, short *trg, uint8_t step, uint8_t slow_cnt);
uint8_t myScan(int mode);
void up_function();
void down_function();
int controlBar();
void scrollMenu(u8g2_t *u8g2);

#endif //C8T6_U8G2_UI_H
