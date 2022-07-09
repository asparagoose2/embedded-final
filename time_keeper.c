/*
 * File:   time_keeper.c
 * Author: ofir
 *
 * Created on July 8, 2022, 8:03 PM
 */

#include <stdio.h>
#include <math.h>
#include "time_keeper.h"
#include "date_time.h"
#include "LabC5_Accel_BitBang.X/oledDriver/oledC.h"
//#include "LabC5_Accel_BitBang.X/oledDriver/oledC_colors.h"
#include "LabC5_Accel_BitBang.X/oledDriver/oledC_shapes.h"
#include <stdbool.h>

#define SCREEN_SIZE 95

#define SECONDS_BASE_ANGLE ((2 * M_PI) / 60)
#define MINUTES_BASE_ANGLE ((2 * M_PI) / 60)
#define HOURS_BASE_ANGLE ((2 * M_PI) / 12)
#define SECONDS_DIAL_WIDTH 1
#define MINUTES_DIAL_WIDTH 2
#define HOURS_DIAL_WIDTH 4
#define SECONDS_DIAL_LENGTH ((SCREEN_SIZE/2)-14)
#define MINUTES_DIAL_LENGTH ((SCREEN_SIZE/2)-14)
#define HOURS_DIAL_LEGNTH ((SCREEN_SIZE/2)-24)

uint16_t FONT_COLOR = 0xffff;
uint16_t BACKGROUND_COLOR = 0x0;

const Point CENTER_POINT = {SCREEN_SIZE / 2,SCREEN_SIZE / 2};
bool isAM = true;


void set_font_color(uint16_t color)
{
    FONT_COLOR = color;
}
void set_background_color(uint16_t color)
{
    BACKGROUND_COLOR = color;
}


/*
 * Set time
 */


void draw_digital_clock(unsigned long time_in_seconds) 
{
    char curr_time_str[9] = {0};
    char prev_time_str[9] = {0};
    Datetime datetime;
    epoc_to_datetime(time_in_seconds, &datetime);
    sprintf(curr_time_str, "%02d:%02d:%02d", datetime.tm_hour,datetime.tm_min,datetime.tm_sec);
    epoc_to_datetime(time_in_seconds - 1, &datetime);
    sprintf(prev_time_str, "%02d:%02d:%02d", datetime.tm_hour,datetime.tm_min,datetime.tm_sec);
    oledC_DrawString(5, 45, 2, 2, prev_time_str, BACKGROUND_COLOR);
    oledC_DrawString(5, 45, 2, 2, curr_time_str, FONT_COLOR);

}

void draw_corner_clock(unsigned long time_in_seconds) 
{
    char curr_time_str[9] = {0};
    char prev_time_str[9] = {0};
    Datetime datetime;
    epoc_to_datetime(time_in_seconds, &datetime);
    sprintf(curr_time_str, "%02d:%02d:%02d", datetime.tm_hour,datetime.tm_min,datetime.tm_sec);
    epoc_to_datetime(time_in_seconds - 1, &datetime);
    sprintf(prev_time_str, "%02d:%02d:%02d", datetime.tm_hour,datetime.tm_min,datetime.tm_sec);
    oledC_DrawString(47, 0, 1, 1, prev_time_str, BACKGROUND_COLOR);
    oledC_DrawString(47, 0, 1, 1, curr_time_str, FONT_COLOR);
}

void set_am_pm(AM_PM am_pm)
{
    if(am_pm == AM) 
    {
        oledC_DrawString(0, 85, 1, 1, "PM", BACKGROUND_COLOR);
        isAM = true;
        oledC_DrawString(0, 85, 1, 1, "AM", FONT_COLOR);
    } 
    else 
    {
        oledC_DrawString(0, 85, 1, 1, "AM", BACKGROUND_COLOR);
        isAM = false;
        oledC_DrawString(0, 85, 1, 1, "PM", FONT_COLOR);
    }
}

void toggle_am_pm() 
{
    oledC_DrawString(0, 85, 1, 1, isAM? "AM" : "PM", BACKGROUND_COLOR);
    isAM = !isAM;
    oledC_DrawString(0, 85, 1, 1, isAM? "AM" : "PM", FONT_COLOR);
}

void draw_watch_face() 
{
    double angle = HOURS_BASE_ANGLE;
    Point p;
    char str[3] = {0};
    for(int i = 1 ; i < 13; i++) 
    {
        p = CALC_POINT((angle * i),((SCREEN_SIZE/2)-4),CENTER_POINT);
        p.x = i > 9 ? p.x - 4:p.x;
        
        p.y = p.y - 4;
        sprintf(str, "%d", i);
        oledC_DrawString(p.x, p.y, 1, 1, str, FONT_COLOR);
    }

}

void draw_analog_clock(unsigned long time_in_seconds, bool draw_full)
{
    Datetime pev_datetime, curr_datetime;
    Point seconds_dial;
    Point minutes_dial;
    Point hours_dial;
    
    epoc_to_datetime(time_in_seconds - 1, &pev_datetime);
    epoc_to_datetime(time_in_seconds, &curr_datetime);
    bool should_update_min = pev_datetime.tm_min != curr_datetime.tm_min || draw_full;
    bool should_update_hour = pev_datetime.tm_hour != curr_datetime.tm_hour || draw_full;
    
    double seconds_angle = SECONDS_BASE_ANGLE * pev_datetime.tm_sec;
    seconds_dial = CALC_POINT(seconds_angle,SECONDS_DIAL_LENGTH,CENTER_POINT);
    DrawLine_DDA(CENTER_POINT, seconds_dial, SECONDS_DIAL_WIDTH, BACKGROUND_COLOR);
    seconds_angle = SECONDS_BASE_ANGLE * curr_datetime.tm_sec;
    seconds_dial = CALC_POINT(seconds_angle,SECONDS_DIAL_LENGTH,CENTER_POINT);
    DrawLine_DDA(CENTER_POINT, seconds_dial, SECONDS_DIAL_WIDTH, FONT_COLOR);

    if(should_update_min)
    {
        double miutes_angle = MINUTES_BASE_ANGLE * pev_datetime.tm_min;
        minutes_dial = CALC_POINT(miutes_angle,MINUTES_DIAL_LENGTH,CENTER_POINT);
        DrawLine_DDA(CENTER_POINT, minutes_dial, MINUTES_DIAL_WIDTH, BACKGROUND_COLOR);
        miutes_angle = MINUTES_BASE_ANGLE * curr_datetime.tm_min;
        minutes_dial = CALC_POINT(miutes_angle,MINUTES_DIAL_LENGTH,CENTER_POINT);
        DrawLine_DDA(CENTER_POINT, minutes_dial, MINUTES_DIAL_WIDTH, FONT_COLOR);

    }
    
    if(should_update_hour)
    {
        double hours_angle = HOURS_BASE_ANGLE * pev_datetime.tm_hour;
        hours_dial = CALC_POINT(hours_angle,HOURS_DIAL_LEGNTH,CENTER_POINT);
        DrawLine_DDA(CENTER_POINT, hours_dial, HOURS_DIAL_WIDTH, BACKGROUND_COLOR);
        hours_angle = HOURS_BASE_ANGLE * curr_datetime.tm_hour;
        hours_dial = CALC_POINT(hours_angle,HOURS_DIAL_LEGNTH,CENTER_POINT);
        DrawLine_DDA(CENTER_POINT, hours_dial, HOURS_DIAL_WIDTH, FONT_COLOR);
    } 
}

void draw_full_analog_clock(unsigned long time_in_seconds)
{
    draw_watch_face();
    draw_analog_clock(time_in_seconds,true);
    
}

void toggle_alarm_on_off()
{
    
}

void draw_clock(unsigned long time_in_seconds, CLOCK_DISPLAY_MODE mode)
{
    Datetime datetime;
    epoc_to_datetime(time_in_seconds, &datetime);
    if(datetime.tm_hour % 12 == 0 && datetime.tm_min == 0 && datetime.tm_sec == 0)
    {
        set_am_pm(datetime.tm_hour > 11 ? PM : AM);
    }
    
    switch(mode) {
        case ANALOG_CLOCK_DISPLAY:
            draw_analog_clock(time_in_seconds,false);
            break;
        case DIGITAL_CLOCK_DISPLAY:
            draw_digital_clock(time_in_seconds);
            break;
    }
}



void set_clock_format(CLOCK_FORMAT format)
{
    
}
