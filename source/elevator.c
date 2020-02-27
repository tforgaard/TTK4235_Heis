#include "hardware.h"
#include "elevator.h"

#include <stdlib.h>

void Elevator_init(Elevator *elevator)
{
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

void Elevator_update(Elevator *elevator, int * up_orders, int * down_orders)
{    
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
        return;
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
            up_orders[i] = 1;
        }

        if (hardware_read_order(i, HARDWARE_ORDER_DOWN))
        {
            hardware_command_order_light(i, HARDWARE_ORDER_DOWN, 1);
            down_orders[i] = 1;
        }

        if (hardware_read_order(i, HARDWARE_ORDER_INSIDE))
        {
            hardware_command_order_light(i, HARDWARE_ORDER_INSIDE, 1);
            if (i > elevator->current_floor)
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


static void Elevator_update_at_floor(Elevator *elevator)
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

static void Elevator_update_obstruction_signal(Elevator *elevator)
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

static void Elevator_update_current_floor(Elevator *elevator)
{
    static int direction_has_been_updated = 0;

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

void Elevator_turn_off_lights_at(int floor)
{
    hardware_command_order_light(floor, HARDWARE_ORDER_UP, 0);
    hardware_command_order_light(floor, HARDWARE_ORDER_DOWN, 0);
    hardware_command_order_light(floor, HARDWARE_ORDER_INSIDE, 0);
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