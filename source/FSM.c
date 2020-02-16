#include "FSM.h"
#include "Orders.h"
#include "Timer.h"
#include "manage_elevator.h"


void FSM_update(Elevator_state * current_state, Elevator_state * last_state) // call function with ( & state)
{
    if (hardware_read_stop_signal())
    {
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        hardware_command_stop_light(1);
        Elevator_set_order_lights();
        Orders_remove_all_orders();
        if ( Elevator_at_floor())
        {
            *current_state=idle;
            hardware_command_door_open(1);
            Elevator_open_doors();
        }
        else
        {
            current_state = idle;
        }
        
    }
    else if (Elevator_get_open_doors_flag())
    {
        Elevator_update_buttons();
        hardware_command_stop_light(0);
        hardware_command_door_open(1);

        if(hardware_read_obstruction_signal())
        {
            hardware_command_door_open(1); // unødvendig?
            Timer_set();
        }
        else
        {
            if(Timer_get() >= 3000)
            {
                Elevator_close_doors();
            }
        }
    }
    else
    {
        Elevator_update_buttons();
        hardware_command_stop_light(0);
        Elevator_update_current_floor();
        switch (*current_state)
        {
        case idle:
            Elevator_idle(current_state, last_state);
            break;
        
        case idle_with_door_open:
            Elevator_idle_with_door_open(current_state, last_state);

        case moving_to_highest_order:
            Elevator_moving_to_highest_order(current_state, last_state);
            break;

        case moving_to_lowest_order:
            Elevator_moving_to_lowest_order(current_state, last_state);
            break;

        case stopping_on_up:
            Elevator_stopping_on_up(current_state, last_state);
            break;

        case stopping_on_down:
            Elevator_stopping_on_down(current_state, last_state);
            break;

        case moving_up_to_service:
            Elevator_moving_up_to_service(current_state, last_state);
            break;

        case moving_down_to_service:
            Elevator_moving_down_to_service(current_state, last_state);
            break;
        
        default:
            *current_state = idle;
            break;
        }
    }    
}

void FSM_run(){
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    Elevator_initialize();
    Elevator_state current_state = idle;
    Elevator_state last_state = idle;
    
    while(1)
    {
        FSM_update(&current_state, &last_state);
    }

}
