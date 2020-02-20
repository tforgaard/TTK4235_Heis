#include "FSM.h"
#include "Orders.h"
#include "Timer.h"
#include "manage_elevator.h"

void FSM_update(Elevator_state * current_state, Elevator_state * last_state) // call function with ( & state)
{
    if (hardware_read_stop_signal())
    {
        FSM_stop();

        if ( Elevator_at_floor())
        {
            *last_state= idle; 
            *current_state=idle;
            hardware_command_door_open(1);
            Elevator_open_doors();
            Timer_set();
        }
        else
        {
            *last_state= idle; 
            *current_state = idle;
        }
    
    }
    else if (Elevator_get_open_doors_flag())
    {
        Elevator_check_buttons();
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
                hardware_command_door_open(0);
            }
        }
    }
    else
    {
        Elevator_update_current_floor();
        Elevator_check_buttons();
        hardware_command_stop_light(0);

        switch (*current_state)
        {
        case idle:
            Elevator_idle(current_state, last_state);
            break;

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

        case moving_to_service:
            Elevator_moving_to_service(current_state, last_state);
            break;
        
        default:
            *last_state = *current_state; //idle?
            *current_state = idle;
            break;
        }
    }    
}

void FSM_stop()
{
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    hardware_command_stop_light(1);

    for (int i=0; i<HARDWARE_NUMBER_OF_FLOORS; i++){
        Orders_remove_up_order(i);
        Orders_remove_down_order(i);
        hardware_command_order_light(i,HARDWARE_ORDER_DOWN,0);
        hardware_command_order_light(i,HARDWARE_ORDER_UP,0);
        hardware_command_order_light(i,HARDWARE_ORDER_INSIDE,0);
    }
}

void FSM_run(Elevator_state * current_state, Elevator_state * last_state){
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    Elevator_initialize();
    *current_state = idle;
    *last_state = idle;
    
    while(1)
    {
        FSM_update(current_state, last_state);
    }

}

void Elevator_initialize() {
    Orders_init(HARDWARE_NUMBER_OF_FLOORS);

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
}

void Elevator_moving_to_service(Elevator_state * current_state, Elevator_state * last_state){
    if (Orders_floor_is_in_up_orders(Elevator_get_current_floor()) && Elevator_at_floor() && last_direction == HARDWARE_MOVEMENT_UP)
    {
        *last_state = *current_state;
        *current_state = stopping_on_up;


        Elevator_finished_up_order();
        Elevator_open_doors();
        Timer_set();
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    }
    else if (Orders_floor_is_in_down_orders(Elevator_get_current_floor()) && Elevator_at_floor() && last_direction == HARDWARE_MOVEMENT_DOWN)
    {
        *last_state = *current_state;
        *current_state = stopping_on_down;


        Elevator_finished_down_order();
        Elevator_open_doors();
        Timer_set();
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    }
    else
    {   
        *last_state = *current_state;
        *current_state = idle;
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    }

}

void Elevator_stopping_on_down(Elevator_state * current_state, Elevator_state * last_state){
        Elevator_finished_down_order();
        if (Orders_down_orders_is_empty())
        {
            *current_state = idle;
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
        else 
        {
            *current_state = moving_to_service;
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
            last_direction = HARDWARE_MOVEMENT_DOWN;
        }
    *last_state = stopping_on_down;
}

void Elevator_stopping_on_up(Elevator_state * current_state, Elevator_state * last_state){
        Elevator_finished_up_order();
        if (Orders_up_orders_is_empty())
        {
            *current_state = idle;
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
        else
        {
            *current_state = moving_to_service;
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
            last_direction = HARDWARE_MOVEMENT_UP;
        }
    *last_state = stopping_on_up;
}

void Elevator_moving_to_highest_order(Elevator_state * current_state, Elevator_state * last_state){
    if (Orders_get_highest_order() !=-1)
    {
        if (Orders_get_highest_order() >= Elevator_get_current_floor())
        {
            if (Orders_get_highest_order() == Elevator_get_current_floor() && Elevator_at_floor())
            {
                *last_state = *current_state;
                *current_state = stopping_on_down;


                Elevator_finished_down_order();
                Timer_set();
                Elevator_open_doors();
                hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            }
            else if (Orders_floor_is_in_up_orders(Elevator_get_current_floor()) && Elevator_at_floor())
            {
                *last_state = *current_state;
                *current_state = stopping_on_up;


                Elevator_finished_up_order();
                Timer_set();
                Elevator_open_doors();
                hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            }
            else
            {            
                hardware_command_movement(HARDWARE_MOVEMENT_UP);
            }

        }
        else if (Orders_get_highest_order() < Elevator_get_current_floor())
        {
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        }
    }
    else
    {
        *last_state = *current_state;
        *current_state = idle;
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    }
}

void Elevator_moving_to_lowest_order(Elevator_state * current_state, Elevator_state * last_state){
    if (Orders_get_lowest_order() !=-1)
    {
        if (Orders_get_lowest_order() <= Elevator_get_current_floor())
        {
            if( Orders_get_lowest_order() == Elevator_get_current_floor() && Elevator_at_floor())
            {
                *last_state = *current_state;
                *current_state = stopping_on_up;


                Elevator_finished_up_order();
                Timer_set();
                Elevator_open_doors();
                hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            }
            else if ( Orders_floor_is_in_down_orders(Elevator_get_current_floor()) && Elevator_at_floor())
            {
                *last_state = *current_state;
                *current_state = stopping_on_down;


                Elevator_finished_down_order();
                Timer_set();
                Elevator_open_doors();
                hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            }
            else
            {
                hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
            } 
        }
        else if (Orders_get_lowest_order() > Elevator_get_current_floor())
        {
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
        }
    }
    else
    {
        *last_state = *current_state;
        *current_state = idle;
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    }
}

void Elevator_idle(Elevator_state * current_state, Elevator_state * last_state){
    if (!Orders_up_orders_is_empty())
    {
        *last_state = *current_state;
        *current_state = moving_to_lowest_order;
    }
    else if (!Orders_down_orders_is_empty())
    {
        *last_state = *current_state;
        *current_state = moving_to_highest_order;  
    }
    else
    {
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    }
}