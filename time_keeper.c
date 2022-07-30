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
const Point ANALOG_DATE_POS = {23, 65};
const Point DIGITAL_DATE_POS = {47, 85};
const Point DIGITAL_RING_DATE_POS = {25, 58};


int curr_clock_format = 12;
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


void draw_digital_clock(unsigned long time_in_seconds, bool drawDate) 
{
    char curr_time_str[9] = {0};
//    char prev_time_str[9] = {0};
    Datetime datetime;
    epoc_to_datetime(time_in_seconds, &datetime);
    if(datetime.tm_hour == 0 && datetime.tm_min == 0 && datetime.tm_sec == 0 || drawDate)
        draw_date(datetime.tm_mday,datetime.tm_mon+1,datetime.tm_year,DIGITAL_DATE_POS);
    sprintf(curr_time_str, "%02d:%02d:%02d", datetime.tm_hour % curr_clock_format,datetime.tm_min,datetime.tm_sec);
    epoc_to_datetime(time_in_seconds - 1, &datetime);
//    sprintf(prev_time_str, "%02d:%02d:%02d", datetime.tm_hour % curr_clock_format,datetime.tm_min,datetime.tm_sec);
    oledC_DrawRectangle(0, 45, 95, 60, BACKGROUND_COLOR);
    oledC_DrawString(5, 45, 2, 2, curr_time_str, FONT_COLOR);

}

void draw_ring_digital_clock(unsigned long time_in_seconds)
{
    Datetime datetime;
    epoc_to_datetime(time_in_seconds, &datetime);
    drawPieSlice(CENTER_POINT.x, CENTER_POINT.y, SCREEN_SIZE/2 - 4, FONT_COLOR, 0, datetime.tm_sec*6);
    if(time_in_seconds % 60 == 0)
    {
        drawPieSlice(CENTER_POINT.x, CENTER_POINT.y, SCREEN_SIZE/2 - 4, BACKGROUND_COLOR, 0, 360);
        char curr_time_str[9] = {0};
        char prev_time_str[9] = {0};
        sprintf(curr_time_str, "%02d:%02d", datetime.tm_hour % curr_clock_format,datetime.tm_min);
        epoc_to_datetime(time_in_seconds - 1, &datetime);
        sprintf(prev_time_str, "%02d:%02d", datetime.tm_hour % curr_clock_format,datetime.tm_min);
        oledC_DrawRectangle(22, 38, 85, 57, BACKGROUND_COLOR);
        oledC_DrawString(22, 38, 2, 2, curr_time_str, FONT_COLOR);
        if(datetime.tm_hour == 0 && datetime.tm_min == 0 && datetime.tm_sec == 0)
            draw_date(datetime.tm_mday,datetime.tm_mon+1,datetime.tm_year,DIGITAL_RING_DATE_POS);
    }
}

void draw_rings_clock(unsigned long time_in_seconds)
{
    Datetime datetime;
    epoc_to_datetime(time_in_seconds, &datetime);
    if(datetime.tm_sec == 0)
    {
        drawPieSlice(CENTER_POINT.x, CENTER_POINT.y, SCREEN_SIZE/2 - 4, BACKGROUND_COLOR, 0, 360);   
    }
    else
    {
        drawPieSlice(CENTER_POINT.x, CENTER_POINT.y, SCREEN_SIZE/2 - 4, FONT_COLOR, 0, datetime.tm_sec*6);   
    }
    if(datetime.tm_min == 0)
    {
        drawPieSlice(CENTER_POINT.x, CENTER_POINT.y, SCREEN_SIZE/2 - 16, BACKGROUND_COLOR, 0, 360); // clean minutes ring
        drawPieSlice(CENTER_POINT.x, CENTER_POINT.y, SCREEN_SIZE/2 - 15, BACKGROUND_COLOR, 0, 360);
    }
    else if(datetime.tm_sec == 0)
    {
        drawPieSlice(CENTER_POINT.x, CENTER_POINT.y, SCREEN_SIZE/2 - 16, FONT_COLOR, 0, datetime.tm_min*6); 
        drawPieSlice(CENTER_POINT.x, CENTER_POINT.y, SCREEN_SIZE/2 - 15, FONT_COLOR, 0, datetime.tm_min*6);
    }
    if(datetime.tm_hour == 0)
    {
        drawPieSlice(CENTER_POINT.x, CENTER_POINT.y, SCREEN_SIZE/2 - 27, BACKGROUND_COLOR, 0, 360); // clear hours ring
        drawPieSlice(CENTER_POINT.x, CENTER_POINT.y, SCREEN_SIZE/2 - 28, BACKGROUND_COLOR, 0, 360);
        drawPieSlice(CENTER_POINT.x, CENTER_POINT.y, SCREEN_SIZE/2 - 29, BACKGROUND_COLOR, 0, 360);
    }
    else if(datetime.tm_min == 0)
    {
        drawPieSlice(CENTER_POINT.x, CENTER_POINT.y, SCREEN_SIZE/2 - 27, FONT_COLOR, 0, datetime.tm_hour*30); 
        drawPieSlice(CENTER_POINT.x, CENTER_POINT.y, SCREEN_SIZE/2 - 28, FONT_COLOR, 0, datetime.tm_hour*30);
        drawPieSlice(CENTER_POINT.x, CENTER_POINT.y, SCREEN_SIZE/2 - 29, FONT_COLOR, 0, datetime.tm_hour*30);
    }
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
    oledC_DrawString(0, 85, 1, 1, "AM", BACKGROUND_COLOR);
    oledC_DrawString(0, 85, 1, 1, "PM", BACKGROUND_COLOR);
    oledC_DrawString(0, 85, 1, 1, am_pm == AM ? "AM" : "PM", FONT_COLOR);
    isAM = am_pm == AM;
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
    bool should_update_min = pev_datetime.tm_min != curr_datetime.tm_min || draw_full || true; 
    bool should_update_hour = pev_datetime.tm_hour != curr_datetime.tm_hour || draw_full || true;
    
    draw_date(curr_datetime.tm_mday,curr_datetime.tm_mon+1,curr_datetime.tm_year,ANALOG_DATE_POS);
    
    double seconds_angle = SECONDS_BASE_ANGLE * pev_datetime.tm_sec;
    seconds_dial = CALC_POINT(seconds_angle,SECONDS_DIAL_LENGTH,CENTER_POINT);
    DrawLine_DDA(CENTER_POINT, seconds_dial, SECONDS_DIAL_WIDTH, BACKGROUND_COLOR);
    seconds_angle = SECONDS_BASE_ANGLE * curr_datetime.tm_sec;
    seconds_dial = CALC_POINT(seconds_angle,SECONDS_DIAL_LENGTH,CENTER_POINT);
    DrawLine_DDA(CENTER_POINT, seconds_dial, SECONDS_DIAL_WIDTH, FONT_COLOR);

    if(should_update_min)
    {
        double miutes_angle = MINUTES_BASE_ANGLE * pev_datetime.tm_min +  ((HOURS_BASE_ANGLE/(60*5))*pev_datetime.tm_sec);
        minutes_dial = CALC_POINT(miutes_angle,MINUTES_DIAL_LENGTH,CENTER_POINT);
        DrawLine_DDA(CENTER_POINT, minutes_dial, MINUTES_DIAL_WIDTH, BACKGROUND_COLOR);
        miutes_angle = MINUTES_BASE_ANGLE * curr_datetime.tm_min + ((HOURS_BASE_ANGLE/(60*5))*curr_datetime.tm_sec);
        minutes_dial = CALC_POINT(miutes_angle,MINUTES_DIAL_LENGTH,CENTER_POINT);
        DrawLine_DDA(CENTER_POINT, minutes_dial, MINUTES_DIAL_WIDTH, FONT_COLOR);

    }
    
    if(should_update_hour)
    {
        double hours_angle = HOURS_BASE_ANGLE * pev_datetime.tm_hour + ((HOURS_BASE_ANGLE/60)*pev_datetime.tm_min);
        hours_dial = CALC_POINT(hours_angle,HOURS_DIAL_LEGNTH,CENTER_POINT);
        DrawLine_DDA(CENTER_POINT, hours_dial, HOURS_DIAL_WIDTH, BACKGROUND_COLOR);
        hours_angle = HOURS_BASE_ANGLE * curr_datetime.tm_hour + ((HOURS_BASE_ANGLE/60)*curr_datetime.tm_min);
        hours_dial = CALC_POINT(hours_angle,HOURS_DIAL_LEGNTH,CENTER_POINT);
        DrawLine_DDA(CENTER_POINT, hours_dial, HOURS_DIAL_WIDTH, FONT_COLOR);
    } 
}

void draw_full_analog_clock(unsigned long time_in_seconds)
{
    draw_watch_face();
    draw_analog_clock(time_in_seconds,true);
    
}

void draw_date(int day, int month, int year, Point pos)
{
    char str[10];
    sprintf(str, "%02d/%02d/%02d", day,month,year%100);
    oledC_DrawRectangle(pos.x, pos.y, pos.x+48, pos.y+8, BACKGROUND_COLOR);
    oledC_DrawString(pos.x, pos.y, 1, 1, str, FONT_COLOR);
}

// used when first displaying the clock screen
void init_clock_display(unsigned long time_in_seconds, CLOCK_DISPLAY_MODE mode)
{
    Datetime datetime;
    epoc_to_datetime(time_in_seconds, &datetime);
    if(mode == ANALOG_CLOCK_DISPLAY)
    {
        draw_date(datetime.tm_mday,datetime.tm_mon+1,datetime.tm_year,ANALOG_DATE_POS);
        set_am_pm(datetime.tm_hour > 11 ? PM : AM);
        draw_watch_face();
        draw_full_analog_clock(time_in_seconds);
    }
    else if(mode == DIGITAL_CLOCK_DISPLAY)
    {        
        draw_date(datetime.tm_mday,datetime.tm_mon+1,datetime.tm_year,DIGITAL_DATE_POS);
        
        if(curr_clock_format == 12)
        {   
            set_am_pm(datetime.tm_hour > 11 ? PM : AM);
        }
    }
    else if(DIGITAL_RING_CLOCK_DISPLAY == mode)
    {
        char curr_time_str[6] = {0};        
        draw_date(datetime.tm_mday,datetime.tm_mon+1,datetime.tm_year,DIGITAL_RING_DATE_POS);
        
        sprintf(curr_time_str, "%02d:%02d", datetime.tm_hour % curr_clock_format,datetime.tm_min);
        oledC_DrawString(22, 38, 2, 2, curr_time_str, FONT_COLOR);
        
        if(curr_clock_format == 12)
        {   
            set_am_pm(datetime.tm_hour > 11 ? PM : AM);
        }
    }
    else if(RINGS_CLOCK_DISPLAY == mode)
    {
        set_am_pm(datetime.tm_hour > 11 ? PM : AM);
        drawPieSlice(CENTER_POINT.x, CENTER_POINT.y, SCREEN_SIZE/2 - 4, FONT_COLOR, 0, datetime.tm_sec*6);   
        drawPieSlice(CENTER_POINT.x, CENTER_POINT.y, SCREEN_SIZE/2 - 16, FONT_COLOR, 0, datetime.tm_min*6); 
        drawPieSlice(CENTER_POINT.x, CENTER_POINT.y, SCREEN_SIZE/2 - 15, FONT_COLOR, 0, datetime.tm_min*6);
        drawPieSlice(CENTER_POINT.x, CENTER_POINT.y, SCREEN_SIZE/2 - 27, FONT_COLOR, 0, datetime.tm_hour*30); 
        drawPieSlice(CENTER_POINT.x, CENTER_POINT.y, SCREEN_SIZE/2 - 28, FONT_COLOR, 0, datetime.tm_hour*30);
        drawPieSlice(CENTER_POINT.x, CENTER_POINT.y, SCREEN_SIZE/2 - 29, FONT_COLOR, 0, datetime.tm_hour*30);
    }
}
    

void draw_clock(unsigned long time_in_seconds, CLOCK_DISPLAY_MODE mode)
{
    Datetime datetime;
    epoc_to_datetime(time_in_seconds, &datetime);
    if(datetime.tm_hour % 12 == 0 && datetime.tm_min == 0 && datetime.tm_sec == 0 && !(mode == DIGITAL_CLOCK_DISPLAY && curr_clock_format == 24))
    {
        set_am_pm(datetime.tm_hour > 11 ? PM : AM);
    }
    
    switch(mode) {
        case ANALOG_CLOCK_DISPLAY:
            draw_analog_clock(time_in_seconds,false);
            break;
        case DIGITAL_CLOCK_DISPLAY:
            draw_digital_clock(time_in_seconds, true);
            break;
        case DIGITAL_RING_CLOCK_DISPLAY:
            draw_ring_digital_clock(time_in_seconds);
            break;
        case RINGS_CLOCK_DISPLAY:
            draw_rings_clock(time_in_seconds);
            break;
    }
}

void set_clock_format(CLOCK_FORMAT format)
{
    curr_clock_format = format == HRS_12 ? 12 : 24;
}
