#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "fsm.h"

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
    Orders_init(&orders, HARDWARE_NUMBER_OF_FLOORS);

    while (1)
    {
        FSM_update(&elevator, &orders);
    }

    Orders_free(&orders);
}
