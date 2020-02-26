/**
 * @file
 * @brief FSM module updates the state of the elevator.
 */

#include "manage_elevator.h" //Need this for some reason.. why?

#ifndef FSM_H
#define FSM_H

void FSM_init(Elevator *elevator);

void FSM_update(Elevator *elevator);

void FSM_running(Elevator *elevator, int * orders);

void FSM_stop_button_engaged(Elevator *elevator, int * orders);

void FSM_doors_open(Elevator *elevator, int * orders);

void FSM_idle(Elevator *elevator);

void FSM_moving_down(Elevator *elevator);

void FSM_moving_up(Elevator *elevator);

#endif