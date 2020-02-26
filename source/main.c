#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "FSM.h"
#include "manage_elevator.h"

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
        FSM_update(&elevator);
    }
}
