/*
 * File:   menu.c
 * Author: ofir
 *
 * Created on July 9, 2022, 12:46 PM
 */

#include "xc.h"
#include "menu.h"
#include "time_keeper.h"
#include <stdbool.h>
#include <stdlib.h>
#include "LabC5_Accel_BitBang.X/oledDriver/oledC.h"

int menu_mode = MAIN_MENU;
int selected_item = MENU_SET_TIME;
int selected_sub_item = 0;
bool selected_changed = false;
int prev_selected_item = 0;
int prev_selected_sub_item = 0;

const char* menu_items_literals[] = {"Display Mode", "12H/24H","Set Time","Set Date","Alarm"};
const char* disp_mode_items_literals[] = {"Analog", "Digital"};
const char* format_items_literals[] = {"12H", "24H"};
const char* alarm_items_literals[] = {"Set Alarm", "Alarm On/Off"};
const char** menus_arr[] = {disp_mode_items_literals,format_items_literals,NULL,NULL,alarm_items_literals};


void set_selected_item(int item)
{
    if(item != selected_item)
    {
        prev_selected_item = selected_item;
        selected_item = item % MENU_ITEMS_COUNT;
        selected_changed = true;
    }
    
}

void set_selected_sub_item(int item)
{
    if(item != selected_sub_item)
    {
        prev_selected_sub_item = selected_sub_item;
        selected_sub_item = item % 2;
        selected_changed = true;
    }
}
void inc_selected_item()
{
    prev_selected_item = selected_item;
    selected_item = (selected_item + 1) % MENU_ITEMS_COUNT;
    selected_changed = true;
}

void select_item()
{
    oledC_clearScreen();
    if(MAIN_MENU == menu_mode)
    { 
        menu_mode = SELECTED_MENU;
    }
    else if(ALARM_SUB_MENU == menu_mode)
    {
        
    }
    else if(menu_mode == SELECTED_MENU)
    {
        switch(selected_item) 
        {
            case(MENU_DISP_MODE):
                set_clock_display(selected_sub_item);
                break;
            case(MENU_CLOCK_FORMAT):
                set_clock_format(selected_sub_item);
                break;
            case(MENU_SET_TIME):
                menu_mode = SET_TIME_MENU;
                break;
            case(MENU_SET_DATE):
                menu_mode = SET_DATE_MENU;
                break;
            case(MENU_ALARM):
                if(prev_selected_sub_item == 0)
                {
                    menu_mode = SET_TIME_MENU;
                }
                else if(prev_selected_sub_item == 1)
                {
                    toggle_alarm_on_off();
                }
                break;
            
        }
    }
}

void left_button_pressed()
{
    select_item();
    
}
void right_button_pressed()
{
    if(menu_mode == SELECTED_MENU)
    {
        menu_mode = MAIN_MENU;
    }
}
void pot_changed_value(int pot_val)
{
    if(menu_mode == SELECTED_MENU)
    {
        set_selected_sub_item(pot_val/512);
    }
    else if(menu_mode == MAIN_MENU)
    {
        set_selected_item(pot_val/205);
    }
}

void menu()
{
    if(menu_mode == SELECTED_MENU)
    {
        for(int i = 0; i < 2; i++)
        {
            if(selected_changed)
            {
                selected_changed = false;
                oledC_DrawCircle( 4 , 22 + 15*prev_selected_sub_item, 3, 0x0);
            }
            if(selected_sub_item == i)
            {
                oledC_DrawCircle( 4 , 22 + 15*i, 3, 0xffff);
            }
            oledC_DrawString(12, 18 + 15*i, 1, 1, menus_arr[selected_item][i], 0xffff);
        }
    }
    else if( menu_mode == MAIN_MENU)
    {
        for(int i = 0; i < MENU_ITEMS_COUNT; i++)
        {
            if(selected_changed)
            {
                selected_changed = false;
                oledC_DrawCircle( 4 , 22 + 15*prev_selected_item, 3, 0x0);
            }
            if(selected_item == i)
            {
                oledC_DrawCircle( 4 , 22 + 15*i, 3, 0xffff);
            }
            oledC_DrawString(12, 18 + 15*i, 1, 1, menu_items_literals[i], 0xffff);
        }
    }
    else if(SET_TIME_MENU == menu_mode)
    {
        
    }
}