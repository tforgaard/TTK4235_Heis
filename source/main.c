#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "FSM.h"
#include "Orders.h"
#include "manage_elevator.h"

int main()
{
    int error = hardware_init();
    if (error != 0)
    {
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    state current_state = IDLE;
    order current_order = BOTH; // må fikses

    FSM_init();

    while (1)
    {
        FSM_update(&current_state, &current_order);
    }
}
