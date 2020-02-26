/**
 * @file
 * @brief Order module keeps track of up and down orders,
 * and provide operations on them. 
 */

#ifndef ORDERS_H
#define ORDERS_H

typedef enum order
{
    UP=0, // bare for å gjøre det klart at verdien er viktig for programmets funksjon
    DOWN=1,
    BOTH=2
} order;

int *up_orders;
int *down_orders;

int floor_count;

int Orders_init(int number_of_floors);

int Orders_floor_is_in_orders(int floor, order order);

int Orders_get_h_l_order(order order);

void Orders_remove_order(int floor, order order);

void Orders_remove_all_orders();

int Orders_over_floor(int current_floor, order order);

int Orders_under_floor(int current_floor, order order);

void Orders_recieve_and_set_orders(int * orders);

#endif
