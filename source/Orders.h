/**
 * @file
 * @brief Order module keeps track of up and down orders,
 * and provide operations on them. 
 */

#ifndef ORDERS_H
#define ORDERS_H

typedef enum
{
    UP,
    DOWN,
    BOTH
} Order;

int *up_orders;
int *down_orders;

int floor_count;

int Orders_init(int number_of_floors);

int Orders_floor_is_in_orders(int floor, Order order);

int Orders_get_highest_order(); //among down orders
int Orders_get_lowest_order();  //among up orders

int Orders_get_h_l_order(Order order);

void Orders_remove_order(int floor, Order order);

void Orders_remove_all_orders();

int Orders_over_floor(int current_floor, Order order);

int Orders_under_floor(int current_floor, Order order);

void Orders_recieve_and_set_orders(int * orders);

#endif
