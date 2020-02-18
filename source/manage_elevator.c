#include "manage_elevator.h"
#include "Orders.h"

void Elevator_check_buttons(){
    for (int i = 0; i<HARDWARE_NUMBER_OF_FLOORS; i++){

        if(hardware_read_order(i,HARDWARE_ORDER_UP)) {
            Orders_add_order_from_button(HARDWARE_ORDER_UP, i, Elevator_get_current_floor());
        }
        
        if(hardware_read_order(i,HARDWARE_ORDER_DOWN)) 
        {
            Orders_add_order_from_button(HARDWARE_ORDER_DOWN, i, Elevator_get_current_floor());
        }

        if (hardware_read_order(i,HARDWARE_ORDER_INSIDE)) 
        {
            Orders_add_order_from_button(HARDWARE_ORDER_INSIDE, i, Elevator_get_current_floor());

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

int Elevator_get_current_floor(){
    return current_floor;
}

void Elevator_update_current_floor(){
    for(int i = 0; i<HARDWARE_NUMBER_OF_FLOORS; i++ ){
        if (hardware_read_floor_sensor(i) == 1) 
        {
            elevator_floor_signal[i] = 1; //fjern
            current_floor = i;
            hardware_command_floor_indicator_on(i);
        }
        else
        {
        elevator_floor_signal[i] = 0; //fjern
        }
    }
}

void Elevator_open_doors(){
    elevator_open_doors_flag=1;
}

int Elevator_get_open_doors_flag(){
    return elevator_open_doors_flag;
}

void Elevator_close_doors(){
    elevator_open_doors_flag=0;
}