#include "Orders.h"
#include "Timer.h"

typedef enum Elevator_state 
{
moving_up_to_service,
moving_down_to_service,
stopping_on_down,
stopping_on_up,
moving_to_highest_order,
moving_to_lowest_order,
idle
} Elevator_state;

int current_floor = -1;
int Elevator_get_current_floor(){return current_floor;}
void Elevator_update_current_floor();

void Elevator_initialize() {
    hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    int detectFloor = 0;

    while( current_floor == -1){
        for(int i = 0; i<HARDWARE_NUMBER_OF_FLOORS-1; i++ ){
            detectFloor=hardware_read_floor_sensor(i);
            if (detectFloor == 1) {
                current_floor = i;
                break;
            }
        }
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    // set state?
}


void Elevator_update(Elevator_state & current_state)
{
    Elevator_update_current_floor();

    switch (current_state)
    {
    case idle:
        if (!Orders_up_orders_is_empty)
        {
            current_state = moving_to_highest_order;
        }
        else if (!Orders_down_orders_is_empty)
        {
            current_state = moving_to_lowest_order;  
        }
        else
        {
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
        break;

    case moving_to_highest_order:
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
                current_state = stopping_on_down;
                hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            }
        break;

    case moving_to_lowest_order:
        if (Orders_get_highest_order() > Elevator_get_current_floor())
        {
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
        }
        else if (Orders_get_highest_order() < Elevator_get_current_floor())
        {
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        }
        else
        {
            current_state = stopping_on_up;
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
                    current_state = idle;
                    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
                 }
                 else 
                {
                    current_state = moving_up_to_service;
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
            if (Timer_get() >= 3)
            {
                Orders_remove_down_order(Elevator_get_current_floor());

                if (Orders_down_orders_is_empty())
                 {
                    current_state = idle;
                    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
                 }
                 else 
                {
                    current_state = moving_down_to_service;
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
            current_state = stopping_on_up;
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
        break;

    case moving_down_to_service:
        if (Orders_floor_is_in_down_orders(Elevator_get_current_floor()))
        {
            current_state = stopping_on_down;
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
        break;
    
    default:
        current_state = idle;
        break;
    }
}




