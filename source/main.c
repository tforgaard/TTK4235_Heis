#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "FSM.h"
#include "manage_elevator.h"
#include "Timer.h"



void printElevator(Elevator * elevator)
{
    printf("floor: %d \n", elevator->current_floor);
    printf("above: %d \n", elevator->is_above_current_floor);
    printf("at: %d \n", elevator->is_at_floor);
    printf("doors_open: %d \n", elevator->doors_are_open);
    printf("doors_obstructed: %d \n", elevator->doors_are_obstructed);
    printf("stop_button: %d \n", elevator->stop_button_is_pressed);
    printf("state: %d \n\n\n", elevator->running_state);
}

int main()
{
    int error = hardware_init();
    if (error != 0)
    {
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    Elevator elevator;

   
    

    FSM_init(&elevator);

    while (1)
    {
        if (Timer_get() % 1000 == 0)
        {
            printElevator(&elevator);
        }

        FSM_update(&elevator);
    }
}
