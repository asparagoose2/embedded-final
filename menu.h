 
#ifndef MENU_H
#define	MENU_H

#define MENU_ITEMS_COUNT 5

enum MENU_PAGE_MODE {
    MAIN_MENU,
    SELECTED_MENU,
    ALARM_SUB_MENU,
    SET_TIME_MENU,
    SET_DATE_MENU
};

enum MENU_ITEMS {
    MENU_DISP_MODE,
    MENU_CLOCK_FORMAT,
    MENU_SET_TIME,
    MENU_SET_DATE,
    MENU_ALARM
};

enum ALARM_MENU_ITEMS {
    MENU_SET_ALARM,
    MENU_TOGGLE_ALARM
};

void menu();

void set_selected_item(int item);
void inc_selected_item();
void select_item();
void left_button_pressed();
void right_button_pressed();
void pot_changed_value(int pot_val);
void print_menu();

#endif	/* MENU_H */

