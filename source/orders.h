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

typedef enum OrderType
{
    UP,
    DOWN,
    BOTH
} OrderType;

int Orders_init(int number_of_floors, Orders * orders);

int Orders_floor_is_in(int floor, OrderType type, Orders * orders);

int Orders_extreme_value_of(OrderType type, Orders * orders);

void Orders_remove(int floor, OrderType type, Orders * orders);
void Orders_remove_all(Orders * orders);

int Orders_order_over(int floor, OrderType type, Orders * orders);
int Orders_order_under(int floor, OrderType type, Orders * orders);

#endif
