#include "orders.h"

#include <stdlib.h>

int Orders_init(Orders *p_orders, int number_of_floors)
{
    p_orders->p_up_orders = malloc(sizeof(int) * number_of_floors);
    p_orders->p_down_orders = malloc(sizeof(int) * number_of_floors);

    if (!p_orders->p_up_orders || !p_orders->p_down_orders)
    {
        return -1;
    }

    for (int i = 0; i < number_of_floors; i++)
    {
        p_orders->p_up_orders[i] = 0;
        p_orders->p_down_orders[i] = 0;
    }

    p_orders->floor_count = number_of_floors;

    return 0;
}

void Orders_free(Orders *p_orders)
{
    free(p_orders->p_up_orders);
    free(p_orders->p_down_orders);
}

int Orders_floor_is_in(Orders *p_orders, OrderType type, int floor)
{
    if (type == UP && p_orders->p_up_orders[floor])
    {
        return 1;
    }
    else if (type == DOWN && p_orders->p_down_orders[floor])
    {
        return 1;
    }
    else if (type == BOTH && (p_orders->p_down_orders[floor] || p_orders->p_up_orders[floor]))
    {
        return 1;
    }
    return 0;  
}

int Orders_extreme_value_of(Orders *p_orders, OrderType type)
{
    if (type == UP)
    {
        for (int i = 0; i < p_orders->floor_count; i++)
        {
            if (p_orders->p_up_orders[i] == 1)
            {
                return i;
            }
        }
        return -1;
    }
    else if (type == DOWN)
    {
        for (int i = p_orders->floor_count - 1; i >= 0; i--)
        {
            if (p_orders->p_down_orders[i] == 1)
            {
                return i;
            }
        }
        return -1;
    }
    return -1;
}

void Orders_remove(Orders *p_orders, OrderType type, int floor)
{
    if (type == UP)
    {
        p_orders->p_up_orders[floor] = 0;
    }
    else if (type == DOWN)
    {
        p_orders->p_down_orders[floor] = 0;
    }
    else if (type == BOTH)
    {
        p_orders->p_up_orders[floor] = 0;
        p_orders->p_down_orders[floor] = 0;
    } 
}

int Orders_order_over(Orders *p_orders, OrderType type, int floor)
{
    for (int i = floor + 1; i < p_orders->floor_count; i++)
    {
        if (type == UP && p_orders->p_up_orders[i] == 1)
        {
            return 1;
        }
        else if (type == DOWN && p_orders->p_down_orders[i] == 1)
        {
            return 1;
        }
        else if (type == BOTH && (p_orders->p_up_orders[i] == 1 || p_orders->p_down_orders[i] == 1))
        {
            return 1;
        }
    }
    return 0;
}

int Orders_order_under(Orders *p_orders, OrderType type, int floor)
{
    for (int i = 0; i < floor; i++)
    {
        if (type == UP && p_orders->p_up_orders[i] == 1)
        {
            return 1;
        }
        else if (type == DOWN && p_orders->p_down_orders[i] == 1)
        {
            return 1;
        }
        else if (type == BOTH && (p_orders->p_up_orders[i] == 1 || p_orders->p_down_orders[i] == 1))
        {
            return 1;
        }
    }
    return 0;   
}


void Orders_remove_all(Orders *p_orders)
{
    for (int i = 0; i < p_orders->floor_count; i++)
    {
        p_orders->p_up_orders[i] = 0;
        p_orders->p_down_orders[i] = 0;
    }
}
