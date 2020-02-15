#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "FSM.h"


int main(){
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }
    Elevator_state current_state=idle;
    Elevator_state last_state=idle;
    
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    FSM_run(& current_state, & last_state);

}
