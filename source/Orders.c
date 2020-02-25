#include "Orders.h"

#include <stdlib.h>

int Orders_init(int number_of_floors)
{
    up_orders = malloc(sizeof(int) * number_of_floors);
    down_orders = malloc(sizeof(int) * number_of_floors);

    if (!up_orders || !down_orders)
    {
        return -1;
    }

    for (int i = 0; i < number_of_floors; i++)
    {
        up_orders[i] = 0;
        down_orders[i] = 0;
    }

    floor_count = number_of_floors;

    return 0;
}

int Orders_floor_is_in_orders(int floor, order order)
{
    if (order == UP && up_orders[floor] == 1)
    {
        return 1;
    }
    else if (order == DOWN && down_orders[floor] == 1)
    {
        return 1;
    }
    else if (order == BOTH  && (up_orders[floor] == 1 || down_orders[floor] == 1))
    {
        return 1;
    }
    return 0;
}

int Orders_get_h_l_order(order order)
{
    if (order == UP)
    {
        for (int i = 0; i < floor_count; i++)
        {
            if (up_orders[i] == 1)
            {
                return i;
            }
        }
        return -1;
    }
    else if (order == DOWN)
    {
        for (int i = floor_count - 1; i >= 0; i--)
        {
            if (down_orders[i] == 1)
            {
                return i;
            }
        }
        return -1;
    }
    return -1;
}

void Orders_remove_order(int floor, order order)
{
    if (order == UP)
    {
        up_orders[floor] = 0;
    }
    else if (order == DOWN)
    {
        down_orders[floor] = 0;
    }
    else if (order == BOTH)
    {
        up_orders[floor] = 0;
        down_orders[floor] = 0;
    }
}

int Orders_over_floor(int current_floor, order order)
{
    for (int i = current_floor + 1; i < floor_count; i++)
    {
        if (order == UP && up_orders[i] == 1)
        {
            return 1;
        }
        else if (order == DOWN && down_orders[i] == 1)
        {
            return 1;
        }
        else if (order == BOTH && (up_orders[i] == 1 || down_orders[i] == 1))
        {
            return 1;
        }
    }
    return 0;
}

int Orders_under_floor(int current_floor, order order)
{
    for (int i = 0; i < current_floor; i++)
    {
        if (order == UP && up_orders[i] == 1)
        {
            return 1;
        }
        else if (order == DOWN && down_orders[i] == 1)
        {
            return 1;
        }
        else if (order == BOTH && (up_orders[i] == 1 || down_orders[i] == 1))
        {
            return 1;
        }
    }
    return 0;
}


void Orders_remove_all_orders()
{
    for (int i = 0; i < floor_count; i++)
    {
        Orders_remove_order(i, BOTH);
    }
}

void Orders_recieve_and_set_orders(int * orders)
{
    for (int i = 0; i < floor_count; i++)
    {
        if(orders[i] == 1)
        {
            up_orders[i] = 1;
        }
        if (orders[i + floor_count] == 1)
        {
            down_orders[i] = 1;
        }
    }
    free(orders);
}
