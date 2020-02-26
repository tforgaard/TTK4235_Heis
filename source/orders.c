#include "orders.h"

#include <stdlib.h>

int Orders_init(int number_of_floors, Orders * orders)
{
    orders->up_orders = malloc(sizeof(int) * number_of_floors);
    orders->down_orders = malloc(sizeof(int) * number_of_floors);

    if (!orders->up_orders || !orders->down_orders)
    {
        return -1;
    }

    for (int i = 0; i < number_of_floors; i++)
    {
        orders->up_orders[i] = 0;
        orders->down_orders[i] = 0;
    }

    orders->floor_count = number_of_floors;

    return 0;
}

int Orders_floor_is_in_up_orders(int floor, Orders * orders)
{
    if (orders->up_orders[floor] == 1)
    {
        return 1;
    }
    return 0;
}

int Orders_floor_is_in_down_orders(int floor, Orders * orders)
{
    if (orders->down_orders[floor] == 1)
    {
        return 1;
    }
    return 0;
}

int Orders_get_highest_order(Orders * orders)
{
    for (int i = orders->floor_count - 1; i >= 0; i--)
    {
        if (orders->down_orders[i] == 1)
        {
            return i;
        }
    }
    return -1;
}

int Orders_get_lowest_order(Orders * orders)
{
    for (int i = 0; i < orders->floor_count; i++)
    {
        if (orders->up_orders[i] == 1)
        {
            return i;
        }
    }
    return -1;
}

void Orders_remove_up_order(int floor, Orders * orders)
{
    orders->up_orders[floor] = 0;
}

void Orders_remove_down_order(int floor, Orders * orders)
{
    orders->down_orders[floor] = 0;
}

int Orders_up_order_over_floor(int current_floor, Orders * orders)
{
    for (int i = current_floor + 1; i < orders->floor_count; i++)
    {
        if (orders->up_orders[i] == 1)
        {
            return 1;
        }
    }
    return 0;
}

int Orders_up_order_under_floor(int current_floor, Orders * orders)
{
    for (int i = 0; i < current_floor; i++)
    {
        if (orders->up_orders[i] == 1)
        {
            return 1;
        }
    }
    return 0;
}

int Orders_down_order_over_floor(int current_floor, Orders * orders)
{
    for (int i = current_floor + 1; i < orders->floor_count; i++)
    {
        if (orders->down_orders[i] == 1)
        {
            return 1;
        }
    }
    return 0;
}

int Orders_down_order_under_floor(int current_floor, Orders * orders)
{
    for (int i = 0; i < current_floor; i++)
    {
        if (orders->down_orders[i] == 1)
        {
            return 1;
        }
    }
    return 0;
}

void Orders_remove_all_orders(Orders * orders)
{
    for (int i = 0; i < orders->floor_count; i++)
    {
        orders->up_orders[i] = 0;
        orders->down_orders[i] = 0;
    }
}
