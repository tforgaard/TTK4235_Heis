#include "hardware.h"
#include "Input.h"


Input input;

void Input_check_elevator_buttons(){
    for (int i = 0; i<HARDWARE_NUMBER_OF_FLOORS; i++){
        if(i<3){
            if(hardware_read_order(i,HARDWARE_ORDER_UP)) {
                input.buttonOrderUp[i]=1;
            }
        }
        if(i>0){
            if(hardware_read_order(i,HARDWARE_ORDER_DOWN)) {
                input.buttonOrderDown[i]=1;
            }
        }

        if (hardware_read_order(i,HARDWARE_ORDER_INSIDE)) {
            input.buttonOrderInside[i]=1;
        } 

    }

}

void Input_readStopSignal(){
    input.stopSignal=hardware_read_stop_signal();
    if ( input.stopSignal == 1 ) {
        hardware_command_stop_light(1);
    }
    else {
        hardware_command_stop_light(0);
    }
}

void Input_setOrderLights() {
    for (int i = 0; i<HARDWARE_NUMBER_OF_FLOORS; i++ ) {
        if (i>0){
            if ( input.buttonOrderDown[i] == 1 ){
                hardware_command_order_light(i, HARDWARE_ORDER_DOWN, 1);
            }
            else {
                hardware_command_order_light(i, HARDWARE_ORDER_DOWN, 0);
            }
        }

        if(i<3){
            if ( input.buttonOrderUp[i] == 1 ){
                hardware_command_order_light(i, HARDWARE_ORDER_UP, 1);
            }
            else {
                hardware_command_order_light(i, HARDWARE_ORDER_UP, 0);
            }
        }
        if ( input.buttonOrderInside[i] == 1 ){
            hardware_command_order_light(i, HARDWARE_ORDER_INSIDE, 1);
        }
        else {
            hardware_command_order_light(i, HARDWARE_ORDER_INSIDE, 0);
        }
    }
}

void Input_setFloorSignal() {
    for(int i = 0; i<HARDWARE_NUMBER_OF_FLOORS; i++ ){
            if (hardware_read_floor_sensor(i) == 1) {
                input.floorSignal[i] = 1;
                input.lastFloor = i;
                hardware_command_floor_indicator_on(i);
            }
            else {
                input.floorSignal[i] = 0;
            }
        }
}

void Input_removeButtonOrderDown(int floor) {
    input.buttonOrderDown[floor]=0;
}

void Input_removeButtonOrderUp(int floor) {
    input.buttonOrderUp[floor]=0;
}

void Input_removeButtonOrderInside(int floor) {
    input.buttonOrderInside[floor]=0;
}

int Input_getLastFloor() {
    return input.lastFloor;
}

int * Input_get_buttonOrderUp()
{
    return input.buttonOrderUp;
}

int * Input_get_buttonOrderDown()
{
    return input.buttonOrderDown;
}

int * Input_get_buttonOrderInside()
{
    return input.buttonOrderInside;
}


void Input_update(){
    Input_check_elevator_buttons();
    Input_readStopSignal();
    Input_setOrderLights();
    Input_setFloorSignal();
    Input_getLastFloor();
}