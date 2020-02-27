#include "fsm.h"
#include "orders.h"
#include "timer.h"
#include "elevator.h"
#include "hardware.h"

#include <stdlib.h>

void FSM_update(Elevator *elevator, Orders *orders)
{
    Elevator_update(elevator, orders->up_orders, orders->down_orders);

    if (elevator->stop_button_is_pressed)
    {
        FSM_stop_button_engaged(elevator, orders);
    }
    else if (elevator->doors_are_open)
    {
        FSM_doors_open(elevator);
    }
    else
    {
        FSM_running(elevator, orders);
    }
}

void FSM_running(Elevator *elevator, Orders * orders)
{
    switch (elevator->running_state)
    {
    case IDLE:
        FSM_idle(elevator, orders);
        break;

    case MOVING_UP:
        FSM_moving_up(elevator, orders);
        break;

    case MOVING_DOWN:
        FSM_moving_down(elevator, orders);
        break;

    default:
        elevator->running_state = IDLE;
        break;
    }
}

void FSM_stop_button_engaged(Elevator *elevator, Orders * orders)
{
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    elevator->running_state = IDLE;
    
    Elevator_turn_off_all_lights();
    Orders_remove_all(orders);

    if (elevator->is_at_floor)
    {
        elevator->doors_are_open = 1;
        Timer_set();
    }
}

void FSM_doors_open(Elevator *elevator)
{
    if (elevator->doors_are_obstructed)
    {
        elevator->doors_are_open = 1;
        Timer_set();
    }
    else
    {
        if (Timer_get() >= STOPTIME)
        {
            elevator->doors_are_open = 0;
        }
    }
}

void FSM_idle(Elevator *elevator, Orders *orders)
{
    int current_floor = elevator->current_floor;
    if (Orders_order_over(orders, BOTH, current_floor))
    {
        elevator->running_state = MOVING_UP;
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
    }
    else if (Orders_order_under(orders, BOTH, current_floor))
    {
        elevator->running_state = MOVING_DOWN;
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
    else
    {
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);

        if (Orders_floor_is_in(orders, BOTH, current_floor))
        {
            if (elevator->is_at_floor)
            {
                FSM_stopping_sequence(elevator, orders);
            }
            else if (elevator->is_above_current_floor)
            {
                elevator->running_state = MOVING_DOWN;
                hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
            }
            else
            {
                elevator->running_state = MOVING_UP;
                hardware_command_movement(HARDWARE_MOVEMENT_UP);
            }
        }
    }
}

void FSM_moving_down(Elevator *elevator, Orders *orders)
{   
    int current_floor = elevator->current_floor;
    if (elevator->is_at_floor)
    {   
        if (Orders_floor_is_in(orders, DOWN, current_floor))
        {
            FSM_stopping_sequence(elevator, orders);
        }
        else if (Orders_order_under(orders, BOTH, current_floor))
        {
            elevator->running_state = MOVING_DOWN;
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        }
        else if (Orders_extreme_value_of(orders, UP) == current_floor)
        {
            FSM_stopping_sequence(elevator, orders);
            elevator->running_state = MOVING_UP;
        }
        else
        {
            elevator->running_state = IDLE;
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
    }
}

void FSM_moving_up(Elevator *elevator, Orders *orders)
{
    int current_floor = elevator->current_floor;
    if (elevator->is_at_floor)
    {
        if (Orders_floor_is_in(orders, UP, current_floor))
        {
            FSM_stopping_sequence(elevator, orders);
        }
        else if (Orders_order_over(orders, BOTH, current_floor))
        {
            elevator->running_state = MOVING_UP;
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
        }
        else if (Orders_extreme_value_of(orders, DOWN) == current_floor)
        {
            FSM_stopping_sequence(elevator, orders); 
            elevator->running_state = MOVING_DOWN;
        }
        else
        {
            elevator->running_state = IDLE;
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
    }
}

void FSM_stopping_sequence(Elevator *elevator, Orders *orders)
{
    elevator->doors_are_open = 1;
    Timer_set();
    Elevator_turn_off_lights_at(elevator->current_floor);
    Orders_remove(orders, BOTH, elevator->current_floor);
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
}