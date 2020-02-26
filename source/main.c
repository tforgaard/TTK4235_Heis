#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "fsm.h"
#include "elevator.h"
#include "timer.h"
#include "orders.h"



void printElevator(Elevator * elevator)
{
    printf("floor: %d \n", elevator->current_floor);
    printf("above: %d \n", elevator->is_above_current_floor);
    printf("at: %d \n", elevator->is_at_floor);
    printf("doors_open: %d \n", elevator->doors_are_open);
    printf("doors_obstructed: %d \n", elevator->doors_are_obstructed);
    printf("stop_button: %d \n", elevator->stop_button_is_pressed);
    printf("state: %d \n", elevator->running_state);
}

void printOrders(Orders * orders)
{
    for (int i = 0; i < 4; i++)
    {
        printf("up: %d \n", orders->up_orders[i]);
        printf("down: %d \n\n\n", orders->down_orders[i]);
    }
    

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
    Orders orders;

    Elevator_init(&elevator);
    Orders_init(HARDWARE_NUMBER_OF_FLOORS, &orders);

    while (1)
    {
        if (Timer_get() % 1000 == 0)
        {
            printElevator(&elevator);
            printOrders(&orders);
        }


        FSM_update(&elevator, &orders);
    }
}
