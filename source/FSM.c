#include "FSM.h"
#include "Orders.h"
#include "Timer.h"
#include "manage_elevator.h"

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
    Elevator_update_current_floor();
    Elevator_check_buttons();
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
    //acounting for the scenario where elevator wants to go current floor after a stop
    if (*current_state != IDLE)
    {
        Elevator_set_was_moving_up_at_stop(*current_state == MOVING_UP);
    }
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
    Elevator_check_buttons();
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
    if (Orders_down_order_over_floor(current_floor))
    {
        *current_state = MOVING_UP;
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
    }
    else if (Orders_up_order_under_floor(current_floor))
    {
        *current_state = MOVING_DOWN;
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
    else if (Orders_down_order_under_floor(current_floor))
    {
        *current_state = MOVING_DOWN;
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
    else if (Orders_up_order_over_floor(current_floor))
    {
        *current_state = MOVING_UP;
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
    }
    else
    {
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);

        if (Orders_floor_is_in_up_orders(current_floor) || Orders_floor_is_in_down_orders(current_floor))
        {
            if (Elevator_at_floor())
            {
                Elevator_finished_down_order();
                Elevator_finished_up_order();
                Elevator_open_doors();
                Timer_set();
            }
            else if (Elevator_was_moving_up_at_stop())
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
    if (Elevator_at_floor())
    {   
        Elevator_update_current_floor();
        //treffer en etasje i down orders
        if (Orders_floor_is_in_down_orders(current_floor))
        {
            Elevator_open_doors();
            Timer_set();
            Elevator_finished_down_order();
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }

        //Det finnes en bestilling lavere. Fortsett ned
        else if (Orders_down_order_under_floor(current_floor) || Orders_up_order_under_floor(current_floor))
        {
            *current_state = MOVING_DOWN;
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        }
        //Har ankommet den laveste up_order, og det er ingen flere down_orders under. Bytter da til opp modus
        else if (Orders_get_lowest_order() == current_floor)
        {
            *current_state = MOVING_UP;
            //ingen hardware action her fordi den må stoppe først
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
    //last_direction = HARDWARE_MOVEMENT_UP;
    if (Elevator_at_floor())
    {
        Elevator_update_current_floor();
        //treffer en etasje i up_orders. Stopp. Slett cf
        if (Orders_floor_is_in_up_orders(current_floor))
        {
            Elevator_open_doors();
            Timer_set();
            Elevator_finished_up_order();
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }

        //Den finnes en up order eller down order over cf. Fortsett opp
        else if (Orders_up_order_over_floor(current_floor) || Orders_down_order_over_floor(current_floor))
        {
            *current_state = MOVING_UP;
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
        }

        //Treffer den høyeste etasjen i down orders.
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
