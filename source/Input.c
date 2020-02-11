#include <Input.h>


    // HARDWARE_ORDER_UP,
    // HARDWARE_ORDER_INSIDE,
    // HARDWARE_ORDER_DOWN
Input input;

void Input_check_elevator_buttons(){
    for (int i = 0; i<HARDWARE_NUMBER_OF_FLOORS-1; i++){
        if(hardware_read_order(i,HARDWARE_ORDER_UP)) {
            input.buttonOrderUp[i]=1;
        }
        if(hardware_read_order(i,HARDWARE_ORDER_DOWN)) {
            input.buttonOrderDown[i]=1;
        }
        if (hardware_read_order(i,HARDWARE_ORDER_INSIDE)) {
            input.buttonOrderInside[i]=1;
        } 

    }

}

void readStopSignal(){
    input.stopSignal=hardware_read_stop_signal();
    if ( input.stopSignal == 1 ) {
        hardware_command_stop_light(1);
    }
    else {
        hardware_command_stop_light(0);
    }
}

void setOrderLights() {
    for (int i = 0; i<HARDWARE_NUMBER_OF_FLOORS-2; i++ ) {
        if ( input.buttonOrderDown[i] == 1 ){
            hardware_command_order_light(i, HARDWARE_ORDER_DOWN, 1);
        }
        else {
            hardware_command_order_light(i, HARDWARE_ORDER_DOWN, 0);
        }

        if ( input.buttonOrderUp[i] == 1 ){
            hardware_command_order_light(i, HARDWARE_ORDER_UP, 1);
        }
        else {
            hardware_command_order_light(i, HARDWARE_ORDER_UP, 0);
        }

        if ( input.buttonOrderInside[i] == 1 ){
            hardware_command_order_light(i, HARDWARE_ORDER_INSIDE, 1);
        }
        else {
            hardware_command_order_light(i, HARDWARE_ORDER_INSIDE, 0);
        }
    }
    if ( input.buttonOrderInside[HARDWARE_NUMBER_OF_FLOORS-1] == 1 ){
        hardware_command_order_light(HARDWARE_NUMBER_OF_FLOORS-1, HARDWARE_ORDER_INSIDE, 1);
    }
    else {
        hardware_command_order_light(HARDWARE_NUMBER_OF_FLOORS-1, HARDWARE_ORDER_INSIDE, 0);
    }
}

void setFloorSignal() {
    int detectFloor;
    for(int i = 0; i<HARDWARE_NUMBER_OF_FLOORS-1; i++ ){
            detectFloor=hardware_read_floor_sensor(i);
            if (detectFloor == 1) {
                current_floor = i;
                break;
            }
        }
}

void removeButtonOrderDown(int floor) {
    input.buttonOrderDown[floor]=0;
}

void removeButtonOrderUp(int floor) {
    input.buttonOrderUp[floor]=0;
}

void removeButtonOrderInside(int floor) {
    input.buttonOrderInside[floor]=0;
}