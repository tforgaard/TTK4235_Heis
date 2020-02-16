#include "Orders.h"

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
    for (int i = HARDWARE_NUMBER_OF_FLOORS - 1; i >= 0; i--)
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
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
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
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
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
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
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
    //hardware_command_order_light(floor,HARDWARE_ORDER_DOWN,0);
    hardware_command_order_light(floor,HARDWARE_ORDER_UP,0);
    hardware_command_order_light(floor,HARDWARE_ORDER_INSIDE,0);
}

void Orders_remove_down_order(int floor)
{
    down_orders[floor]=0;
        hardware_command_order_light(floor,HARDWARE_ORDER_DOWN,0);
        //hardware_command_order_light(floor,HARDWARE_ORDER_UP,0);
        hardware_command_order_light(floor,HARDWARE_ORDER_INSIDE,0);
}

void Orders_remove_all_orders() {
    for (int i=0; i<HARDWARE_NUMBER_OF_FLOORS; i++){
        up_orders[i]=0;
        down_orders[i]=0;
        hardware_command_order_light(i,HARDWARE_ORDER_DOWN,0);
        hardware_command_order_light(i,HARDWARE_ORDER_UP,0);
        hardware_command_order_light(i,HARDWARE_ORDER_INSIDE,0);
    }
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
