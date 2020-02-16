#include "hardware.h"

#ifndef ORDERS_H
#define ORDERS_H

typedef enum button{
    HARDWARE_ORDER_UP,
    HARDWARE_ORDER_INSIDE,
    HARDWARE_ORDER_DOWN
} button;

int up_orders[HARDWARE_NUMBER_OF_FLOORS];
int down_orders[HARDWARE_NUMBER_OF_FLOORS];

int Orders_floor_is_in_up_orders(int floor);
int Orders_floor_is_in_down_orders(int floor);

int Orders_get_highest_order(); //among down orders
int Orders_get_lowest_order(); //among up orders

int Orders_up_orders_is_empty(); //bool
int Orders_down_orders_is_empty();

void Orders_remove_up_order(int floor);
void Orders_remove_down_order(int floor);

void Orders_remove_all_orders();

void Orders_get_orders_from_IO();

void Orders_add_order_from_button(button button_press, int floor, int current_floor);

#endif