#include <stdio.h>
#include <stdlib.h>
#include "elevator.c"


int main(){
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    Elevator_initialize();
    /*
    while(1){

        }
    }
    */
}
