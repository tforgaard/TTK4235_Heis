#include "Orders.h"
#include "Timer.h"
#include "Input.h"
#include "elevator.h"

void Elevator_update_current_floor(){
    current_floor=Input_getLastFloor();
}

void Elevator_initialize() {
    hardware_command_movement(HARDWARE_MOVEMENT_UP);
    while( current_floor == -1){
        for(int i = 0; i<HARDWARE_NUMBER_OF_FLOORS-1; i++ ){
            if (hardware_read_floor_sensor(i) == 1 )  {
                current_floor = i;
                break;
            }
        }
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    // set state?
}


void Elevator_update(Elevator_state * current_state) // call function with ( & state)
{
    Elevator_update_current_floor();

    switch (*current_state)
    {
    case idle:
        if (!Orders_up_orders_is_empty())
        {
            *current_state = moving_to_highest_order;
        }
        else if (!Orders_down_orders_is_empty())
        {
            *current_state = moving_to_lowest_order;  
        }
        else
        {
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
        break;

    case moving_to_highest_order:
        if (Orders_get_lowest_order() !=-1){
            if (Orders_get_lowest_order() > Elevator_get_current_floor())
            {
                hardware_command_movement(HARDWARE_MOVEMENT_UP);
            }
            else if (Orders_get_lowest_order() < Elevator_get_current_floor())
            {
                hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
            }
            else
            {
                *current_state = stopping_on_down;
                hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            }
        }
        else
        {
            *current_state = idle;
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
        break;

    case moving_to_lowest_order:
        if (Orders_get_highest_order() !=-1)
        {
            if (Orders_get_highest_order() > Elevator_get_current_floor())
            {
                hardware_command_movement(HARDWARE_MOVEMENT_UP); //switch?
            }
            else if (Orders_get_highest_order() < Elevator_get_current_floor())
            {
                hardware_command_movement(HARDWARE_MOVEMENT_DOWN); //switch?
            }
            else
            {
                *current_state = stopping_on_up;
                hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            }
        }
        else
        {
            *current_state = idle;
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
        break;

    case stopping_on_up:
        if (Timer_is_set())
        {
            if (Timer_get() >= 3000)
            {
                Timer_reset();
                Orders_remove_up_order(Elevator_get_current_floor());

                if (Orders_up_orders_is_empty())
                 {
                    *current_state = idle;
                    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
                 }
                 else 
                {
                    *current_state = moving_up_to_service;
                    hardware_command_movement(HARDWARE_MOVEMENT_UP);
                }
            }

        }
        else
        {
            Timer_set();
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
        break;


    case stopping_on_down:
        if (Timer_is_set())
        {
            if (Timer_get() >= 3000)
            {
                Timer_reset();
                Orders_remove_down_order(Elevator_get_current_floor());

                if (Orders_down_orders_is_empty())
                 {
                    *current_state = idle;
                    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
                 }
                 else 
                {
                    *current_state = moving_down_to_service;
                    hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
                }
            }

        }
        else
        {
            Timer_set();
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
        break;

    case moving_up_to_service:
        if (Orders_floor_is_in_up_orders(Elevator_get_current_floor()))
        {
            *current_state = stopping_on_up;
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
        break;

    case moving_down_to_service:
        if (Orders_floor_is_in_down_orders(Elevator_get_current_floor()))
        {
            *current_state = stopping_on_down;
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
        break;
    
    default:
        *current_state = idle;
        break;
    }
}

void Elevator_run(){
    Elevator_initialize();
    Elevator_state state=idle;
    while(1){
        if(!hardware_read_stop_signal()){
            Input_update();
            Orders_get_orders_from_IO();
            Elevator_update(& state);
        }
        else {
            state=idle;
            Input_setOrderLights();
            Orders_remove_all_orders();
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }

    }

}




