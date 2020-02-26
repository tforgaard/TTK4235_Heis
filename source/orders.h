/**
 * @file
 * @brief Order module keeps track of up and down orders,
 * and provide operations on them. 
 */

#ifndef ORDERS_H
#define ORDERS_H

typedef struct Orders
{
    int *up_orders;
    int *down_orders;
    int floor_count;
} Orders;

int Orders_init(int number_of_floors, Orders * orders);

int Orders_floor_is_in_up_orders(int floor, Orders * orders);
int Orders_floor_is_in_down_orders(int floor, Orders * orders);

int Orders_get_highest_order(Orders * orders); //among down orders
int Orders_get_lowest_order(Orders * orders);  //among up orders

void Orders_remove_up_order(int floor, Orders * orders);
void Orders_remove_down_order(int floor, Orders * orders);

void Orders_remove_all_orders(Orders * orders);

int Orders_up_order_over_floor(int current_floor, Orders * orders);
int Orders_up_order_under_floor(int current_floor, Orders * orders);

int Orders_down_order_over_floor(int current_floor, Orders * orders);
int Orders_down_order_under_floor(int current_floor, Orders * orders);


#endif
