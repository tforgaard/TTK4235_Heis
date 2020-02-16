#include "manage_elevator.h"
#include "Orders.h"
#include "Timer.h"

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
        if (last_state == moving_up_to_service) // moving_down_to_service ?
        {
            *current_state = *last_state;

            hardware_command_movement(HARDWARE_MOVEMENT_UP);
        }
        else
        {
            *current_state = moving_down_to_service;
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        }
    }
    *last_state = stopping_on_down;
}

void Elevator_stopping_on_up(Elevator_state * current_state, Elevator_state * last_state){
    Orders_remove_up_order(current_floor);
    hardware_command_order_light(current_floor, HARDWARE_ORDER_UP, 0);
    hardware_command_order_light(current_floor, HARDWARE_ORDER_DOWN, 0);
    hardware_command_order_light(current_floor, HARDWARE_ORDER_INSIDE, 0); // disse må flyttes inn i last state løkken
    // for å skru av riktig lys 
    if (Orders_up_orders_is_empty() && last_state != moving_to_highest_order)
    {
        *current_state = idle_with_door_open;
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    }
    else 
    {
        if (last_state == moving_down_to_service) // moving_up_to_service?  // moving_to_highest_order?
        {
            *current_state = *last_state;
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        }
        else
        {
            *current_state = moving_up_to_service;
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
        }
    }
    *last_state = stopping_on_up;
}

void Elevator_moving_to_highest_order(Elevator_state * current_state, Elevator_state * last_state){
    if (Orders_get_highest_order() !=-1){
        if (Orders_get_highest_order() > current_floor)
        {
            if (Orders_floor_is_in_up_orders(current_floor))
            {
                *last_state = *current_state;
                *current_state = stopping_on_up;
                Timer_set();
                hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            }
            else
            {            
                hardware_command_movement(HARDWARE_MOVEMENT_UP);
            }

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


void Elevator_check_buttons(){
    int current_floor = Elevator_get_last_floor();
    for (int i = 0; i<HARDWARE_NUMBER_OF_FLOORS; i++){
        if(i<3){
            if(hardware_read_order(i,HARDWARE_ORDER_UP)) {
                Orders_add_order_from_button(HARDWARE_ORDER_UP, i, current_floor);
            }
        }
        if(i>0)
        {
            if(hardware_read_order(i,HARDWARE_ORDER_DOWN)) 
            {
                Orders_add_order_from_button(HARDWARE_ORDER_DOWN, i, current_floor);
            }
        }

        if (hardware_read_order(i,HARDWARE_ORDER_INSIDE)) 
        {
            Orders_add_order_from_button(HARDWARE_ORDER_INSIDE, i, current_floor);

        } 
    }
}

int Elevator_at_floor(){
    for(int i = 0; i<HARDWARE_NUMBER_OF_FLOORS; i++ )
    {
        if (hardware_read_floor_sensor(i) == 1)
        {
            return 1;
        }
    }
    return 0;
}

int elevator_floor_signal[HARDWARE_NUMBER_OF_FLOORS]; //fjern?
int elevator_last_floor;

int elevator_get_last_floor(){
    return elevator_last_floor;
}

void Elevator_update_current_floor(){
    for(int i = 0; i<HARDWARE_NUMBER_OF_FLOORS; i++ ){
        if (hardware_read_floor_sensor(i) == 1) 
        {
            elevator_floor_signal[i] = 1; //fjern
            elevator_last_floor = i;
            hardware_command_floor_indicator_on(i);
        }
        else
        {
        elevator_floor_signal[i] = 0; //fjern
        }
    }
}