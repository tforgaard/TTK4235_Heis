#include "hardware.h"
#include "elevator.h"

void Elevator_init(Elevator *p_elevator)
{
    p_elevator->is_at_floor = 0;
    p_elevator->is_above_current_floor = 0;
    p_elevator->stop_button_is_pressed = 0;
    p_elevator->doors_are_obstructed = 0;
    p_elevator->running_state = IDLE;

    p_elevator->current_floor = -1;
    hardware_command_movement(HARDWARE_MOVEMENT_UP);
    while (p_elevator->current_floor == -1)
    {
        for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
        {
            if (hardware_read_floor_sensor(i) == 1)
            {
                p_elevator->current_floor = i;
                break;
            }
        }
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
}

void Elevator_update(Elevator *p_elevator, int *p_up_orders, int *p_down_orders)
{
    if (p_elevator->doors_are_open)
    {
        hardware_command_door_open(1);
    }
    else
    {
        hardware_command_door_open(0);
    }

    if (hardware_read_stop_signal())
    {
        p_elevator->stop_button_is_pressed = 1;
        hardware_command_stop_light(1);
        Elevator_turn_off_all_lights();
        return;
    }
    else
    {
        hardware_command_stop_light(0);
        p_elevator->stop_button_is_pressed = 0;
    }

    Elevator_update_current_floor(p_elevator);
    Elevator_update_at_floor(p_elevator);
    Elevator_update_obstruction_signal(p_elevator);

    //Read orders and set corresponding lights
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
    {

        if (p_elevator->doors_are_open && i == p_elevator->current_floor)
        {
            continue;
        }

        if (hardware_read_order(i, HARDWARE_ORDER_UP))
        {
            hardware_command_order_light(i, HARDWARE_ORDER_UP, 1);
            p_up_orders[i] = 1;
        }

        if (hardware_read_order(i, HARDWARE_ORDER_DOWN))
        {
            hardware_command_order_light(i, HARDWARE_ORDER_DOWN, 1);
            p_down_orders[i] = 1;
        }

        if (hardware_read_order(i, HARDWARE_ORDER_INSIDE))
        {
            hardware_command_order_light(i, HARDWARE_ORDER_INSIDE, 1);
            if (i > p_elevator->current_floor)
            {
                p_up_orders[i] = 1;
            }
            else
            {
                p_down_orders[i] = 1;
            }
        }
    }
}

void Elevator_update_at_floor(Elevator *p_elevator)
{
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
    {
        if (hardware_read_floor_sensor(i) == 1)
        {
            p_elevator->is_at_floor = 1;
            return;
        }
    }
    p_elevator->is_at_floor = 0;
}

void Elevator_update_obstruction_signal(Elevator *p_elevator)
{
    if (hardware_read_obstruction_signal())
    {
        p_elevator->doors_are_obstructed = 1;
    }
    else
    {
        p_elevator->doors_are_obstructed = 0;
    }
}

void Elevator_update_current_floor(Elevator *p_elevator)
{
    static int direction_has_been_updated = 0;

    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
    {
        if (hardware_read_floor_sensor(i) == 1)
        {
            p_elevator->current_floor = i;
            hardware_command_floor_indicator_on(i);
            direction_has_been_updated = 0;
            return;
        }
    } //Not at a floor
    if (p_elevator->running_state == MOVING_UP && !direction_has_been_updated)
    {
        p_elevator->is_above_current_floor = 1;
        direction_has_been_updated = 1;
    }
    else if (p_elevator->running_state == MOVING_DOWN && !direction_has_been_updated)
    {
        p_elevator->is_above_current_floor = 0;
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