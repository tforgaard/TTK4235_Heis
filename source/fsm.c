#include "fsm.h"
#include "orders.h"
#include "timer.h"
#include "elevator.h"
#include "hardware.h"

#include <stdlib.h>

void FSM_init(Elevator *elevator)
{
    Elevator_init(elevator);
    Orders_init(HARDWARE_NUMBER_OF_FLOORS);
}

void FSM_update(Elevator *elevator)
{
    int * orders = Elevator_update(elevator);

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

void FSM_running(Elevator *elevator, int * orders)
{
    Orders_recieve_and_set_orders(orders);

    switch (elevator->running_state) //*elevator.running_state
    {
    case IDLE:
        FSM_idle(elevator);
        break;

    case MOVING_UP:
        FSM_moving_up(elevator);
        break;

    case MOVING_DOWN:
        FSM_moving_down(elevator);
        break;

    default:
        elevator->running_state = IDLE;
        break;
    }
}

void FSM_stop_button_engaged(Elevator *elevator, int * orders)
{
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    elevator->running_state = IDLE;
    
    Elevator_turn_off_all_lights();
    Orders_remove_all_orders();
    free(orders);

    if (elevator->is_at_floor)
    {
        elevator->doors_are_open = 1;
        Timer_set();
    }
}

void FSM_doors_open(Elevator *elevator, int * orders)
{
    Orders_recieve_and_set_orders(orders);

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

void FSM_idle(Elevator *elevator)
{
    int current_floor = elevator->current_floor;
    if (Orders_down_order_over_floor(current_floor) || Orders_up_order_over_floor(current_floor))
    {
        elevator->running_state = MOVING_UP;
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
    }
    else if (Orders_up_order_under_floor(current_floor) || Orders_down_order_under_floor(current_floor))
    {
        elevator->running_state = MOVING_DOWN;
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
    else
    {
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);

        if (Orders_floor_is_in_up_orders(current_floor) || Orders_floor_is_in_down_orders(current_floor))
        {
            if (elevator->is_at_floor)
            {
                Elevator_finished_down_order(current_floor);
                Elevator_finished_up_order(current_floor);
                Orders_remove_down_order(current_floor);
                Orders_remove_up_order(current_floor);
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

void FSM_moving_down(Elevator *elevator)
{   
    int current_floor = elevator->current_floor;
    if (elevator->is_at_floor)
    {   
        if (Orders_floor_is_in_down_orders(current_floor))
        {
            elevator->doors_are_open = 1;
            Timer_set();
            Elevator_finished_down_order(current_floor);
            Orders_remove_down_order(current_floor);
            if ((Orders_get_lowest_order() == current_floor) && !Orders_down_order_under_floor(current_floor))
            {
                Elevator_finished_up_order(current_floor);
                Orders_remove_up_order(current_floor);
            }
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
        else if (Orders_down_order_under_floor(current_floor) || Orders_up_order_under_floor(current_floor))
        {
            elevator->running_state = MOVING_DOWN;
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        }
        else if (Orders_get_lowest_order() == current_floor)
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

void FSM_moving_up(Elevator *elevator)
{
    int current_floor = elevator->current_floor;
    if (elevator->is_at_floor)
    {
        if (Orders_floor_is_in_up_orders(current_floor))
        {
            elevator->doors_are_open = 1;
            Timer_set();
            Elevator_finished_up_order(current_floor);
            Orders_remove_up_order(current_floor);
            if ((Orders_get_highest_order() == current_floor) && !Orders_up_order_over_floor(current_floor))
            {
                Elevator_finished_down_order(current_floor);
                Orders_remove_down_order(current_floor);
            }
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
        else if (Orders_up_order_over_floor(current_floor) || Orders_down_order_over_floor(current_floor))
        {
            elevator->running_state = MOVING_UP;
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
        }
        else if ((Orders_get_highest_order() == current_floor))
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
