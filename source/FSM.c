#include "FSM.h"
#include "Orders.h"
#include "Timer.h"
#include "Input.h"
#include "manage_elevator.h"

void Elevator_update_current_floor(){
    current_floor=Input_getLastFloor();
}

void Elevator_initialize() {
    current_floor=-1;
    hardware_command_movement(HARDWARE_MOVEMENT_UP);
    while( current_floor == -1){
        for(int i = 0; i<HARDWARE_NUMBER_OF_FLOORS; i++ ){
            if (hardware_read_floor_sensor(i) == 1 )  {
                current_floor = i;
                break;
            }
        }
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    // set state?
}


void FSM_update(Elevator_state * current_state, Elevator_state * last_state) // call function with ( & state)
{
    if (hardware_read_stop_signal())
    {
        hardware_command_stop_light(1);
        *current_state=idle_with_door_open;
    }
    else
    {
        hardware_command_stop_light(0);
        Elevator_update_current_floor();
        switch (*current_state)
        {
        case idle:
            Elevator_idle(current_state);
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


void FSM_run(Elevator_state * elevator_state, Elevator_state * last_state){
    Elevator_initialize();
    
    while(1){
        if(!hardware_read_stop_signal()){
            Input_update();
            Orders_get_orders_from_IO();
            Elevator_update(elevator_state);
        }
        else {
            *elevator_state = idle;
            Input_setOrderLights();
            Orders_remove_all_orders();
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }

    }

}




