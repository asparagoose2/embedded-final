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
#include <stdio.h>
#include "LabC5_Accel_BitBang.X/oledDriver/oledC_shapes.h"
#include "LabC5_Accel_BitBang.X/oledDriver/oledC.h"
#include "date_time.h"
#include "main_header.h"

extern unsigned long current_time_in_seconds;
extern WorkingMode working_mode;
extern uint16_t FONT_COLOR;
extern bool is_alarm_enabled;
extern unsigned long alarm_time_in_seconds;


int menu_mode = MAIN_MENU;
int selected_item = MENU_SET_TIME;
int selected_sub_item = 0;
bool selected_changed = false;
int prev_selected_item = 0;
int prev_selected_sub_item = 0;

const int DATA_IN_MONTHS[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

const char* menu_items_literals[] = {"Display Mode", "12H/24H","Set Time","Set Date","Alarm"};
const char* disp_mode_items_literals[] = {"Analog", "Digital","Ring Digital","Rings"};
const char* format_items_literals[] = {"12H", "24H"};
const char* alarm_items_literals[] = {"Set Alarm", "Alarm On/Off"};
const char** menus_arr[] = {disp_mode_items_literals,format_items_literals,NULL,NULL,alarm_items_literals};

Datetime selected_date;

unsigned long selected_time;
int curr_selected_time_unit = 0;
bool selected_time_changed = false;


// select the menu item 
void set_selected_item(int item)
{
    if(item != selected_item)
    {
        prev_selected_item = selected_item;
        selected_item = item % MENU_ITEMS_COUNT;
        selected_changed = true;
    }   
}

// select a sub menu item
void set_selected_sub_item(int item, int item_count)
{
    if(item != selected_sub_item)
    {
        prev_selected_sub_item = selected_sub_item;
        selected_sub_item = item % item_count;
        selected_changed = true;
    }
}

// draw indication which time unit is being edited (white line under the number)
void draw_selected_time_unit(int selected, bool isDate)
{
    if(isDate)
    {
        int width = selected == 2 ? 22 : 11;
        oledC_DrawRectangle(5, 65, 95, 67, 0x0);
        oledC_DrawRectangle(7 + selected*30, 65, 7 + width + selected*29, 67, 0xffff);
    }
    else 
    {
        oledC_DrawRectangle(5, 65, 95, 67, 0x0);
        oledC_DrawRectangle(7 + selected*32, 65, 25 + selected*32, 67, 0xffff);
    }
    
}

// draw the date that is being selected
void draw_selected_date() 
{
    char str[15] = {0};
    sprintf(str,"%02d / %02d / %04d",selected_date.tm_mday,selected_date.tm_mon+1,selected_date.tm_year + 1900);
    oledC_DrawRectangle(0, 53, 95, 60, 0x0);
    oledC_DrawString(7, 53, 1, 1, str, FONT_COLOR);
}

// left button logic
void select_item()
{
    oledC_clearScreen();
    if(MAIN_MENU == menu_mode)
    { 
        switch(selected_item) 
        {
            case(MENU_SET_TIME):
                menu_mode = SET_TIME_MENU;
                selected_time = current_time_in_seconds;
                selected_time_changed = true;
                set_clock_format(HRS_24);
                draw_selected_time_unit(curr_selected_time_unit,false);                
                break;
            case(MENU_SET_DATE):
                menu_mode = SET_DATE_MENU;
                epoc_to_datetime(current_time_in_seconds, &selected_date);
                selected_time_changed = true;
                draw_selected_time_unit(curr_selected_time_unit,true);                
                break;
            case(MENU_DISP_MODE):
                menu_mode = CLOCK_FORMAT_MENU;
                break;
            default:    
                menu_mode = SELECTED_MENU;
                break;
        }
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
            
            case(MENU_ALARM):
                if(selected_sub_item == 0)
                {
                    menu_mode = SET_ALARM_MENU;
                    selected_time = alarm_time_in_seconds;
                    selected_time_changed = true;
                    set_clock_format(HRS_24);
                    draw_selected_time_unit(curr_selected_time_unit,false);
                }
                else if(selected_sub_item == 1)
                {
                    is_alarm_enabled = !is_alarm_enabled;
                }
                break;        
        }
    }
    else if(menu_mode == SET_TIME_MENU)
    {
        selected_time_changed = true;
        curr_selected_time_unit = (curr_selected_time_unit + 1) % 3;
        draw_selected_time_unit(curr_selected_time_unit, false);
    }
    else if(menu_mode == SET_DATE_MENU)
    {
        selected_time_changed = true;
        curr_selected_time_unit = (curr_selected_time_unit + 1) % 3;
        draw_selected_time_unit(curr_selected_time_unit,true);
    }
    else if(menu_mode == SET_ALARM_MENU)
    {
        selected_time_changed = true;
        curr_selected_time_unit = (curr_selected_time_unit + 1) % 3;
        draw_selected_time_unit(curr_selected_time_unit, false);
    }
    else if(menu_mode == CLOCK_FORMAT_MENU)
    {
        set_clock_display(selected_sub_item);
    }
}

void left_button_pressed()
{
    select_item();
    
}

// right button logic
void right_button_pressed()
{
    if(menu_mode == SELECTED_MENU)
    {
        oledC_clearScreen();
        menu_mode = MAIN_MENU;
    }
    else if(menu_mode == SET_TIME_MENU )
    {
        oledC_clearScreen();
        Datetime original, selected_dt;
        epoc_to_datetime(current_time_in_seconds, &original);
        epoc_to_datetime(selected_time, &selected_dt);
        original.tm_hour = selected_dt.tm_hour;
        original.tm_min = selected_dt.tm_min;
        original.tm_sec = selected_dt.tm_sec;
        set_time(datetime_to_epoc(&original));
        menu_mode = MAIN_MENU;    
    } 
    else if(menu_mode == SET_ALARM_MENU )
    {
        oledC_clearScreen();
        set_alarm(selected_time);
        menu_mode = MAIN_MENU;    
    } 
    else if(menu_mode == MAIN_MENU) 
    {
        oledC_clearScreen();
        set_working_mode(CLOCK_MODE);
    }
    else if(menu_mode == SET_DATE_MENU )
    {
        oledC_clearScreen();
        Datetime original;
        epoc_to_datetime(current_time_in_seconds, &original);
        original.tm_mday = selected_date.tm_mday;
        original.tm_mon = selected_date.tm_mon;
        original.tm_year = selected_date.tm_year;
        set_time(datetime_to_epoc(&original));
//        set_time(selected_time);
        menu_mode = MAIN_MENU;   
    }
    else if(menu_mode == CLOCK_FORMAT_MENU)
    {
        oledC_clearScreen();
        menu_mode = MAIN_MENU;   
    }
    
}


void pot_changed_value(int pot_val)
{
    if(menu_mode == SELECTED_MENU)
    {
        set_selected_sub_item(pot_val/512,2);

    }
    else if(menu_mode == CLOCK_FORMAT_MENU)
    {
        set_selected_sub_item(pot_val/256,4);
    }
    else if(menu_mode == MAIN_MENU)
    {
        set_selected_item(pot_val/205);
    }
    else if(SET_TIME_MENU == menu_mode || SET_ALARM_MENU == menu_mode)
    {
        Datetime selected_datetime;
        epoc_to_datetime(selected_time,&selected_datetime);
        if(curr_selected_time_unit == SELECTED_HOUR) 
        {
            selected_datetime.tm_hour = pot_val/43;
        } 
        else if(curr_selected_time_unit == SELECTED_MINUTE)  
        {
            selected_datetime.tm_min = pot_val/17 > 59 ? 59 : pot_val/17;
            
        }
        else if(curr_selected_time_unit == SELECTED_SECOND)  
        {
            selected_datetime.tm_sec = pot_val/17 > 59 ? 59 : pot_val/17;
        }
        unsigned long temp_time = datetime_to_epoc(&selected_datetime);
        if(selected_time != temp_time)
        {
            selected_time = temp_time;
            selected_time_changed = true;
        }
    }
    else if(SET_DATE_MENU == menu_mode)
    {
        Datetime new_date;
        new_date.tm_mday = selected_date.tm_mday;
        new_date.tm_mon = selected_date.tm_mon;
        new_date.tm_year = selected_date.tm_year;
        if(curr_selected_time_unit == SELECTED_HOUR) 
        {
            new_date.tm_mday = pot_val/(1024/DATA_IN_MONTHS[new_date.tm_mon] + 1)+1;
        } 
        else if(curr_selected_time_unit == SELECTED_MINUTE)  
        {
            new_date.tm_mon = pot_val/86;
            
        }
        else if(curr_selected_time_unit == SELECTED_SECOND)  
        {
            new_date.tm_year = pot_val/9 + 70;
        }
        if(selected_date.tm_mday == new_date.tm_mday ||selected_date.tm_mon == new_date.tm_mon || selected_date.tm_year == new_date.tm_year)
        {
            if(new_date.tm_mday > 28 && new_date.tm_mon == 1)
                new_date.tm_mday = 28;
            if(new_date.tm_mday >  DATA_IN_MONTHS[new_date.tm_mon])
                new_date.tm_mday = 30;
            selected_date.tm_mday = new_date.tm_mday;
            selected_date.tm_mon = new_date.tm_mon;
            selected_date.tm_year = new_date.tm_year;
            
            selected_time_changed = true;
        }
    }
}

// menu logic
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
    else if(menu_mode == MAIN_MENU)
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
    else if(SET_TIME_MENU == menu_mode || SET_ALARM_MENU == menu_mode)
    {
        oledC_DrawString(0, 18, 1, 1, selected_item == MENU_SET_TIME ? "Set Time:" : "Set Alarm:", 0xffff);
        if(selected_time_changed)
        {
            draw_digital_clock(selected_time, false);
            selected_time_changed = false;
        }
    }
    else if(SET_DATE_MENU == menu_mode)
    {
        oledC_DrawString(0, 18, 1, 1,  "Set Date:", 0xffff);
        if(selected_time_changed)
        {
            draw_selected_date();
            selected_time_changed = false;
        }
    }
    else if(CLOCK_FORMAT_MENU == menu_mode)
    {
        for(int i = 0; i < 4; i++)
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
    
}