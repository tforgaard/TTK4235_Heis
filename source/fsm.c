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
        FSM_doors_open(elevator, orders);
    }
    else
    {
        FSM_running(elevator, orders);
    }
}

void FSM_running(Elevator *elevator, Orders * orders)
{
    switch (elevator->running_state) //*elevator.running_state
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
    Orders_remove_all_orders(orders);

    if (elevator->is_at_floor)
    {
        elevator->doors_are_open = 1;
        Timer_set();
    }
}

void FSM_doors_open(Elevator *elevator, Orders * orders)
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
    if (Orders_down_order_over_floor(current_floor, orders) || Orders_up_order_over_floor(current_floor, orders))
    {
        elevator->running_state = MOVING_UP;
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
    }
    else if (Orders_up_order_under_floor(current_floor, orders) || Orders_down_order_under_floor(current_floor, orders))
    {
        elevator->running_state = MOVING_DOWN;
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
    else
    {
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);

        if (Orders_floor_is_in_up_orders(current_floor, orders) || Orders_floor_is_in_down_orders(current_floor, orders))
        {
            if (elevator->is_at_floor)
            {
                Elevator_finished_down_order(current_floor, orders);
                Elevator_finished_up_order(current_floor, orders);
                Orders_remove_down_order(current_floor, orders);
                Orders_remove_up_order(current_floor, orders);
                elevator->doors_are_open = 1;
                Timer_set();
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
        if (Orders_floor_is_in_down_orders(current_floor, orders))
        {
            elevator->doors_are_open = 1;
            Timer_set();
            Elevator_finished_down_order(current_floor, orders);
            Orders_remove_down_order(current_floor, orders);
            if ((Orders_get_lowest_order(orders) == current_floor) && !Orders_down_order_under_floor(current_floor, orders))
            {
                Elevator_finished_up_order(current_floor, orders);
                Orders_remove_up_order(current_floor, orders);
            }
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
        else if (Orders_down_order_under_floor(current_floor, orders) || Orders_up_order_under_floor(current_floor, orders))
        {
            elevator->running_state = MOVING_DOWN;
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        }
        else if (Orders_get_lowest_order(orders) == current_floor)
        {
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
        if (Orders_floor_is_in_up_orders(current_floor, orders))
        {
            elevator->doors_are_open = 1;
            Timer_set();
            Elevator_finished_up_order(current_floor, orders);
            Orders_remove_up_order(current_floor, orders);
            if ((Orders_get_highest_order(orders) == current_floor) && !Orders_up_order_over_floor(current_floor, orders))
            {
                Elevator_finished_down_order(current_floor, orders);
                Orders_remove_down_order(current_floor, orders);
            }
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
        else if (Orders_up_order_over_floor(current_floor, orders) || Orders_down_order_over_floor(current_floor, orders))
        {
            elevator->running_state = MOVING_UP;
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
        }
        else if ((Orders_get_highest_order(orders) == current_floor))
        {
            elevator->running_state = MOVING_DOWN;
        }
        else
        {
            elevator->running_state = IDLE;
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
    }
}
