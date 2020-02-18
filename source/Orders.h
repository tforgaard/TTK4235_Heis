#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"

#ifndef ORDERS_H
#define ORDERS_H

int *up_orders;
int *down_orders;

int floor_count;

int Orders_init(int number_of_floors);

int Orders_floor_is_in_up_orders(int floor);
int Orders_floor_is_in_down_orders(int floor);

int Orders_get_highest_order(); //among down orders
int Orders_get_lowest_order(); //among up orders

int Orders_up_orders_is_empty(); //bool
int Orders_down_orders_is_empty();

void Orders_remove_up_order(int floor);
void Orders_remove_down_order(int floor);

void Orders_remove_all_orders();

void Orders_set_up_order(int floor);
void Orders_set_down_order(int floor);

#endif
