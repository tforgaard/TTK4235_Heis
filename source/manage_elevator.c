#include "hardware.h"
#include "manage_elevator.h"

#include <stdlib.h>

void Elevator_init()
{
    current_floor = -1;
    hardware_command_movement(HARDWARE_MOVEMENT_UP);
    while (current_floor == -1)
    {
        for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
        {
            if (hardware_read_floor_sensor(i) == 1)
            {
                current_floor = i;
                break;
            }
        }
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
}

int * Elevator_check_buttons()
{
    //[up1, up2, ... upNFLOORS, down1, down2, .. downNFLOORS]
    //memory freed in recieving function
    int *orders = (int*)malloc(sizeof(int) * 2*HARDWARE_NUMBER_OF_FLOORS);

    for (int i = 0; i < 2*HARDWARE_NUMBER_OF_FLOORS; i++)
    {
        orders[i] = 0;
    }
    

    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
    {

        if (Elevator_get_open_doors_flag() && i == current_floor)
        {
            continue;
        }

        if (hardware_read_order(i, HARDWARE_ORDER_UP))
        {
            hardware_command_order_light(i, HARDWARE_ORDER_UP, 1);
            orders[i] = 1;
        }

        if (hardware_read_order(i, HARDWARE_ORDER_DOWN))
        {
            hardware_command_order_light(i, HARDWARE_ORDER_DOWN, 1);
            orders[HARDWARE_NUMBER_OF_FLOORS + i] = 1;
        }

        if (hardware_read_order(i, HARDWARE_ORDER_INSIDE))
        {
            hardware_command_order_light(i, HARDWARE_ORDER_INSIDE, 1);
            if (i > current_floor)
            {
                orders[i] = 1;
            }
            else
            {
                orders[HARDWARE_NUMBER_OF_FLOORS + i] = 1;
            }
        }
    }
    return orders;
}

int Elevator_at_floor()
{
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
    {
        if (hardware_read_floor_sensor(i) == 1)
        {
            return 1;
        }
    }
    return 0;
}

int Elevator_get_current_floor()
{
    return current_floor;
}

int direction_has_been_updated;
void Elevator_update_current_floor(int dir)
{
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
    {
        if (hardware_read_floor_sensor(i) == 1)
        {
            current_floor = i;
            hardware_command_floor_indicator_on(i);
            direction_has_been_updated = 0;
            return;
        }        
    } //Not at a floor
    if (dir == 0 && !direction_has_been_updated)
    {
        Elevator_is_above_current_floor = 1;
        direction_has_been_updated = 1;
    }
    else if (dir == 1 && !direction_has_been_updated)
    {
        Elevator_is_above_current_floor = 0;
        direction_has_been_updated = 1;
    }
}

void Elevator_open_doors()
{
    elevator_open_doors_flag = 1;
}

int Elevator_get_open_doors_flag()
{
    return elevator_open_doors_flag;
}

void Elevator_close_doors()
{
    elevator_open_doors_flag = 0;
}

void Elevator_finished_order(int dir)
{
    if (dir == 0)
    {
        hardware_command_order_light(Elevator_get_current_floor(), HARDWARE_ORDER_UP, 0);
        hardware_command_order_light(Elevator_get_current_floor(), HARDWARE_ORDER_INSIDE, 0);
    }
    else if (dir == 1)
    {
        hardware_command_order_light(Elevator_get_current_floor(), HARDWARE_ORDER_DOWN, 0);
        hardware_command_order_light(Elevator_get_current_floor(), HARDWARE_ORDER_INSIDE, 0);
    }
    else 
    {
        hardware_command_order_light(Elevator_get_current_floor(), HARDWARE_ORDER_UP, 0);
        hardware_command_order_light(Elevator_get_current_floor(), HARDWARE_ORDER_DOWN, 0);
        hardware_command_order_light(Elevator_get_current_floor(), HARDWARE_ORDER_INSIDE, 0);
    }
}

void Elevator_turn_off_all_lights()
{
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
    {
        hardware_command_order_light(i, HARDWARE_ORDER_DOWN, 0);
        hardware_command_order_light(i, HARDWARE_ORDER_UP, 0);
        hardware_command_order_light(i, HARDWARE_ORDER_INSIDE, 0);
    }
}