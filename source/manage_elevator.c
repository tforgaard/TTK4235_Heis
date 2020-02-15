#include "manage_elevator.h"
#include "Orders.h"
#include "Timer.h"
#include "Input.h"

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


void Elevator_moving_up_to_service(Elevator_state * current_state, Elevator_state * last_state){
    if (Orders_floor_is_in_up_orders(current_floor))
    {
        *last_state = current_state;
        *current_state = idle_with_door_open;
        Timer_set();
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    }
}

void Elevator_moving_down_to_service(Elevator_state * current_state, Elevator_state * last_state){
    if (Orders_floor_is_in_down_orders(current_floor))
    {
        *last_state = current_state;
        *current_state = idle_with_door_open;
        Timer_set();
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    }
}

void Elevator_stopping_on_down(Elevator_state * current_state, Elevator_state * last_state){
    Orders_remove_down_order(current_floor);
    hardware_command_order_light(current_floor, HARDWARE_ORDER_UP, 0);
    hardware_command_order_light(current_floor, HARDWARE_ORDER_DOWN, 0);
    hardware_command_order_light(current_floor, HARDWARE_ORDER_INSIDE, 0);
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

void Elevator_stopping_on_up(Elevator_state * current_state, Elevator_state * last_state){
    Orders_remove_up_order(current_floor);
    hardware_command_order_light(current_floor, HARDWARE_ORDER_UP, 0);
    hardware_command_order_light(current_floor, HARDWARE_ORDER_DOWN, 0);
    hardware_command_order_light(current_floor, HARDWARE_ORDER_INSIDE, 0);
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

void Elevator_moving_to_highest_order(Elevator_state * current_state, Elevator_state * last_state){
    if (Orders_get_highest_order() !=-1){
        if (Orders_get_highest_order() > current_floor)
        {
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
        }
        else if (Orders_get_highest_order() < current_floor)
        {
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        }
        else
        {
            *last_state = *current_state;
            *current_state = idle_with_door_open;
            Timer_set();
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
    }
    else
    {
        *current_state = idle;
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    }
}

void Elevator_moving_to_lowest_order(Elevator_state * current_state, Elevator_state * last_state){
    if (Orders_get_lowest_order() !=-1)
    {
        if (Orders_get_lowest_order() > current_floor)
        {
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
        }
        else if (Orders_get_lowest_order() < current_floor)
        {
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        }
        else
        {
            *last_state = *current_state;
            *current_state = idle_with_door_open;
            Timer_set();
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        }
    }
    else
    {
        *current_state = idle;
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    }
}

void Elevator_idle(Elevator_state * current_state){
    hardware_command_door_open(0);
    if (!Orders_up_orders_is_empty())
    {
        *current_state = moving_to_lowest_order;
    }
    else if (!Orders_down_orders_is_empty())
    {
        *current_state = moving_to_highest_order;  
    }
    else
    {
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    }
}

void Elevator_idle_with_door_open(Elevator_state * current_state, Elevator_state * last_state){
    if (hardware_read_stop_signal()){
        hardware_command_door_open(1);
        Timer_set();
        hardware_command_stop_light(1);
        //block reading buttons
    }
    else if(hardware_read_obstruction_signal()){
        hardware_command_door_open(1);
        Timer_set();
        //continue reading buttons
    }
    else if (Timer_get() >= 3000)
    {
        hardware_command_door_open(0);
        if(* last_state == moving_to_lowest_order || * last_state == moving_up_to_service)
        {
            *current_state = stopping_on_up;
        }
        else if(* last_state == moving_to_highest_order || * last_state == moving_down_to_service)
        {
            *current_state = stopping_on_down;
        }
        else
        {
            *current_state=idle;
        }
    }
    else
    {
        hardware_command_door_open(1);
        //continue reading buttons
    }   
}