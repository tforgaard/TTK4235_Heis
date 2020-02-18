
#include "Orders.h"

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

int Orders_floor_is_in_up_orders(int floor)
{
    if (up_orders[floor] == 1)
    {
        return 1;
    }
    return 0;
}

int Orders_floor_is_in_down_orders(int floor)
{
    if (down_orders[floor] == 1)
    {
        return 1;
    }
    return 0;
}

int Orders_get_highest_order()
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

int Orders_get_lowest_order()
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

int Orders_up_orders_is_empty()
{
    for (int i = 0; i < floor_count; i++)
    {
        if (up_orders[i] == 1)
        {
            return 0;
        }
    }
    return 1;
}

int Orders_down_orders_is_empty()
{
    for (int i = 0; i < floor_count; i++)
    {
        if (down_orders[i] == 1)
        {
            return 0;
        }
    }
    return 1;
}

void Orders_remove_up_order(int floor)
{
    up_orders[floor]=0;
}

void Orders_remove_down_order(int floor)
{
    down_orders[floor]=0;
}

void Orders_add_order_from_button(HardwareOrder button_press, int floor, int current_floor){
    hardware_command_order_light(floor,button_press,1);
    if (button_press == HARDWARE_ORDER_DOWN){
        down_orders[floor] = 1;
    }

    else if (button_press == HARDWARE_ORDER_UP){
        up_orders[floor] = 1;
    }

    else if (button_press == HARDWARE_ORDER_INSIDE){
        if (floor > current_floor)
        {
            up_orders[floor] = 1;
        }
        else
        {
            down_orders[floor] = 1;
        }
    }
}
