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

int Orders_init(Orders * orders, int number_of_floors);
void Orders_free(Orders * orders);

int Orders_floor_is_in(Orders * orders, OrderType type, int floor);

int Orders_extreme_value_of(Orders * orders, OrderType type);

void Orders_remove(Orders * orders, OrderType type, int floor);
void Orders_remove_all(Orders * orders);

int Orders_order_over(Orders * orders, OrderType type, int floor);
int Orders_order_under(Orders * orders, OrderType type, int floor);

#endif
