#include "Input.h"
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

int Orders_get_lowest_order() //fix logic in elevator.h if ==-1;
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
    Input_removeButtonOrderUp(floor);
    Input_removeButtonOrderInside(floor);
}

void Orders_remove_down_order(int floor)
{
    down_orders[floor]=0;
    Input_removeButtonOrderDown(floor);
    Input_removeButtonOrderInside(floor);
}

void Orders_remove_all_orders() {
    for (int i=0; i<HARDWARE_NUMBER_OF_FLOORS; i++){
        up_orders[i]=0;
        down_orders[i]=0;
    }
}

void Orders_get_orders_from_IO()
{
    int * buttonOrderUp = Input_get_buttonOrderUp();
    int * buttonOrderDown = Input_get_buttonOrderDown();
    int * buttonOrderInside = Input_get_buttonOrderInside();

    int current_floor = Input_getLastFloor();

    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
    {   
        if (buttonOrderDown[i]==1){
            down_orders[i] = 1;
        }

        if (buttonOrderUp[i]==1){
            up_orders[i] = 1;
        }

        if (buttonOrderInside[i]==1){
            if (i > current_floor)
            {
                up_orders[i] = 1;
            }
            else
            {
                down_orders[i] = 1;
            }  
        }
    }
}