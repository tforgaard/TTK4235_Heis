#include "fsm.h"
#include "orders.h"
#include "timer.h"
#include "elevator.h"
#include "hardware.h"

#include <stdlib.h>

void FSM_update(Elevator *p_elevator, Orders *p_orders)
{
    Elevator_update(p_elevator, p_orders->p_up_orders, p_orders->p_down_orders);

    if (p_elevator->stop_button_is_pressed)
    {
        FSM_stop_button_engaged(p_elevator, p_orders);
    }
    else if (p_elevator->doors_are_open)
    {
        FSM_doors_open(p_elevator);
    }
    else
    {
        FSM_running(p_elevator, p_orders);
    }
}

void FSM_running(Elevator *p_elevator, Orders *p_orders)
{
    switch (p_elevator->running_state)
    {
    case IDLE:
        FSM_idle(p_elevator, p_orders);
        break;

    case MOVING_UP:
        FSM_moving_up(p_elevator, p_orders);
        break;

    case MOVING_DOWN:
        FSM_moving_down(p_elevator, p_orders);
        break;

    default:
        p_elevator->running_state = IDLE;
        break;
    }
}

void FSM_stop_button_engaged(Elevator *p_elevator, Orders *p_orders)
{
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    p_elevator->running_state = IDLE;
    
    Elevator_turn_off_all_lights();
    Orders_remove_all(p_orders);

    if (p_elevator->is_at_floor)
    {
        p_elevator->doors_are_open = 1;
        Timer_set();
    }
}

void FSM_doors_open(Elevator *p_elevator)
{
    if (p_elevator->doors_are_obstructed)
    {
        p_elevator->doors_are_open = 1;
        Timer_set();
    }
    else
    {
        if (Timer_get() >= STOPTIME)
        {
            p_elevator->doors_are_open = 0;
        }
    }
}

static void FSM_stopping_sequence(Elevator *p_elevator, Orders *p_orders)
{
    p_elevator->doors_are_open = 1;
    Timer_set();
    Elevator_update_current_floor(p_elevator);
    Elevator_turn_off_lights_at(p_elevator->current_floor);
    Orders_remove(p_orders, BOTH, p_elevator->current_floor);
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
}

void FSM_idle(Elevator *p_elevator, Orders *p_orders)
{
    int current_floor = p_elevator->current_floor;
    if (Orders_order_over(p_orders, BOTH, current_floor))
    {
        p_elevator->running_state = MOVING_UP;
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
    }
    else if (Orders_order_under(p_orders, BOTH, current_floor))
    {
        p_elevator->running_state = MOVING_DOWN;
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
    else
    {
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);

        if (Orders_floor_is_in(p_orders, BOTH, current_floor))
        {
            if (p_elevator->is_at_floor)
            {
                FSM_stopping_sequence(p_elevator, p_orders);
            }
            else if (p_elevator->is_above_current_floor)
            {
                p_elevator->running_state = MOVING_DOWN;
                hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
            }
            else
            {
                p_elevator->running_state = MOVING_UP;
                hardware_command_movement(HARDWARE_MOVEMENT_UP);
            }
        }
    }
}

void FSM_moving_down(Elevator *p_elevator, Orders *p_orders)
{   
    int current_floor = p_elevator->current_floor;
    if (p_elevator->is_at_floor)
    {   
        if (Orders_floor_is_in(p_orders, DOWN, current_floor))
        {
            FSM_stopping_sequence(p_elevator, p_orders);
        }
        else if (Orders_order_under(p_orders, BOTH, current_floor))
        {
            p_elevator->running_state = MOVING_DOWN;
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        }
        else if (Orders_extreme_value_of(p_orders, UP) == current_floor)
        {
            FSM_stopping_sequence(p_elevator, p_orders);
            p_elevator->running_state = MOVING_UP;
        }
        else
        {
            p_elevator->running_state = IDLE;
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
    }
}

void FSM_moving_up(Elevator *p_elevator, Orders *p_orders)
{
    int current_floor = p_elevator->current_floor;
    if (p_elevator->is_at_floor)
    {
        if (Orders_floor_is_in(p_orders, UP, current_floor))
        {
            FSM_stopping_sequence(p_elevator, p_orders);
        }
        else if (Orders_order_over(p_orders, BOTH, current_floor))
        {
            p_elevator->running_state = MOVING_UP;
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
        }
        else if (Orders_extreme_value_of(p_orders, DOWN) == current_floor)
        {
            FSM_stopping_sequence(p_elevator, p_orders); 
            p_elevator->running_state = MOVING_DOWN;
        }
        else
        {
            p_elevator->running_state = IDLE;
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
    }
}

