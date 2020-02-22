#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "FSM.h"

int main()
{
    int error = hardware_init();
    if (error != 0)
    {
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    state current_state = IDLE;

    FSM_init();

    while (1)
    {
        FSM_update(&current_state);
    }
}
