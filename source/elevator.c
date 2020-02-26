#include "hardware.h"
#include "elevator.h"

#include <stdlib.h>

void Elevator_init(Elevator *elevator)
{
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    
    elevator->is_at_floor = 0;
    elevator->is_above_current_floor = 0;
    elevator->stop_button_is_pressed = 0;
    elevator->doors_are_obstructed = 0;
    elevator->running_state = IDLE;
    

    elevator->current_floor = -1;
    hardware_command_movement(HARDWARE_MOVEMENT_UP);
    while (elevator->current_floor == -1)
    {
        for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
        {
            if (hardware_read_floor_sensor(i) == 1)
            {
                elevator->current_floor = i;
                break;
            }
        }
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
}

int * Elevator_update(Elevator *elevator)
{
    //[up1, up2, ... upNFLOORS, down1, down2, .. downNFLOORS]
    //memory freed in recieving function
    int *orders = (int*)malloc(sizeof(int) * 2*HARDWARE_NUMBER_OF_FLOORS);

    for (int i = 0; i < 2*HARDWARE_NUMBER_OF_FLOORS; i++)
    {
        orders[i] = 0;
    }
    
    if(elevator->doors_are_open)
    {
        hardware_command_door_open(1);
    }
    else
    {
       hardware_command_door_open(0); 
    }
    
    if (hardware_read_stop_signal())
    {
        elevator->stop_button_is_pressed = 1;
        hardware_command_stop_light(1);

        Elevator_turn_off_all_lights();
        return orders; //empty orders
    }
    else
    {
        hardware_command_stop_light(0);
        elevator->stop_button_is_pressed = 0;
    }

    Elevator_update_current_floor(elevator);
    Elevator_update_at_floor(elevator);
    Elevator_update_obstruction_signal(elevator);


    //Read orders and set corresponding lights
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
    {

        if (elevator->doors_are_open && i == elevator->current_floor)
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
            if (i > elevator->current_floor)
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


void Elevator_update_at_floor(Elevator *elevator)
{
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
    {
        if (hardware_read_floor_sensor(i) == 1)
        {
            elevator->is_at_floor = 1;
            return;
        }
    }
    elevator->is_at_floor = 0;
}

void Elevator_update_obstruction_signal(Elevator *elevator)
{
    if (hardware_read_obstruction_signal())
    {
        elevator->doors_are_obstructed = 1;
    }
    else
    {
        elevator->doors_are_obstructed = 0;
    }
}

int direction_has_been_updated;
void Elevator_update_current_floor(Elevator *elevator)
{
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
    {
        if (hardware_read_floor_sensor(i) == 1)
        {
            elevator->current_floor = i;
            hardware_command_floor_indicator_on(i);
            direction_has_been_updated = 0;
            return;
        }        
    } //Not at a floor
    if (elevator->running_state == MOVING_UP && !direction_has_been_updated)
    {
        elevator->is_above_current_floor = 1;
        direction_has_been_updated = 1;
    }
    else if (elevator->running_state == MOVING_DOWN && !direction_has_been_updated)
    {
        elevator->is_above_current_floor = 0;
        direction_has_been_updated = 1;
    }
}

void Elevator_finished_up_order(int current_floor)
{
    hardware_command_order_light(current_floor, HARDWARE_ORDER_UP, 0);
    hardware_command_order_light(current_floor, HARDWARE_ORDER_INSIDE, 0);
}

void Elevator_finished_down_order(int current_floor)
{
    hardware_command_order_light(current_floor, HARDWARE_ORDER_DOWN, 0);
    hardware_command_order_light(current_floor, HARDWARE_ORDER_INSIDE, 0);
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