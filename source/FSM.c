#include "FSM.h"
#include "Orders.h"
#include "Timer.h"
#include "manage_elevator.h"
#include "hardware.h"

void FSM_init()
{
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    Orders_init(HARDWARE_NUMBER_OF_FLOORS);
    Elevator_init();
}

void FSM_update(state *current_state)
{
    if (hardware_read_stop_signal())
    {
        FSM_stop_button_engaged(current_state);
    }
    else if (Elevator_get_open_doors_flag())
    {
        FSM_doors_open(current_state);
    }
    else
    {
        FSM_running(current_state);
    }
}

void FSM_running(state *current_state)
{
    Elevator_update_current_floor(*current_state);
    Orders_recieve_and_set_orders(Elevator_check_buttons());
    hardware_command_stop_light(0);

    switch (*current_state)
    {
    case IDLE:
        FSM_idle(current_state);
        break;

    case MOVING_UP:
        FSM_moving_up(current_state);
        break;

    case MOVING_DOWN:
        FSM_moving_down(current_state);
        break;

    default:
        *current_state = IDLE;
        break;
    }
}

void FSM_stop_button_engaged(state *current_state)
{
    *current_state = IDLE;

    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    hardware_command_stop_light(1);

    Elevator_turn_off_all_lights();
    Orders_remove_all_orders();

    if (Elevator_at_floor())
    {
        hardware_command_door_open(1);
        Elevator_open_doors();
        Timer_set();
    }
}

void FSM_doors_open(state *current_state)
{
    Orders_recieve_and_set_orders(Elevator_check_buttons());
    hardware_command_stop_light(0);
    hardware_command_door_open(1);

    if (hardware_read_obstruction_signal())
    {
        hardware_command_door_open(1);
        Timer_set();
    }
    else
    {
        if (Timer_get() >= 3000)
        {
            Elevator_close_doors();
            hardware_command_door_open(0);
        }
    }
}

void FSM_idle(state *current_state)
{
    if (Orders_over_floor(current_floor, BOTH))
    {
        *current_state = MOVING_UP;
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
    }
    else if (Orders_under_floor(current_floor, BOTH))
    {
        *current_state = MOVING_DOWN;
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
    else
    {
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);

        if (Orders_floor_is_in_orders(current_floor, BOTH))
        {
            if (Elevator_at_floor())
            {
                Elevator_finished_down_order();
                Elevator_finished_up_order();
                Orders_remove_order(current_floor, BOTH);
                Elevator_open_doors();
                Timer_set();
            }
            else if (Elevator_is_above_current_floor)
            {
                *current_state = MOVING_DOWN;
                hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
            }
            else
            {
                *current_state = MOVING_UP;
                hardware_command_movement(HARDWARE_MOVEMENT_UP);
            }
        }
    }
}

void FSM_moving_down(state *current_state)
{   
    Elevator_update_current_floor(*current_state);
    if (Elevator_at_floor())
    {   
        if (Orders_floor_is_in_orders(current_floor, DOWN))
        {
            Elevator_open_doors();
            Timer_set();
            Elevator_finished_down_order();
            Orders_remove_order(current_floor, DOWN);
            if ((Orders_get_lowest_order() == current_floor) && !Orders_under_floor(current_floor, DOWN))
            {
                Elevator_finished_up_order();
                Orders_remove_order(current_floor, UP);
            }
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
        else if (Orders_under_floor(current_floor, BOTH))
        {
            *current_state = MOVING_DOWN;
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        }
        else if (Orders_get_lowest_order() == current_floor)
        {
            *current_state = MOVING_UP;
        }
        else
        {
            *current_state = IDLE;
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
    }
}

void FSM_moving_up(state *current_state)
{
    Elevator_update_current_floor(*current_state);
    if (Elevator_at_floor())
    {
        if (Orders_floor_is_in_orders(current_floor, UP))
        {
            Elevator_open_doors();
            Timer_set();
            Elevator_finished_up_order();
            Orders_remove_order(current_floor, UP);
            if ((Orders_get_highest_order() == current_floor) && !Orders_over_floor(current_floor, UP))
            {
                Elevator_finished_down_order();
                Orders_remove_order(current_floor, DOWN);
            }
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
        else if (Orders_over_floor(current_floor, BOTH))
        {
            *current_state = MOVING_UP;
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
        }
        else if ((Orders_get_highest_order() == current_floor))
        {
            *current_state = MOVING_DOWN;
        }
        else
        {
            *current_state = IDLE;
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
    }
}
