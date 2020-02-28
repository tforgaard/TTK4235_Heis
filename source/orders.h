/**
 * @file
 * @brief Order module keeps track of up and down orders,
 * and provide operations on them. 
 */

#ifndef ORDERS_H
#define ORDERS_H

typedef struct Orders
{
    int *p_up_orders;
    int *p_down_orders;
    int floor_count;
} Orders;

typedef enum OrderType
{
    UP,
    DOWN,
    BOTH
} OrderType;

int Orders_init(Orders *p_orders, int number_of_floors);
void Orders_free(Orders *p_orders);

int Orders_floor_is_in(Orders *p_orders, OrderType type, int floor);

int Orders_extreme_value_of(Orders *p_orders, OrderType type);

void Orders_remove(Orders *p_orders, OrderType type, int floor);
void Orders_remove_all(Orders *p_orders);

int Orders_order_over(Orders *p_orders, OrderType type, int floor);
int Orders_order_under(Orders *p_orders, OrderType type, int floor);

#endif
