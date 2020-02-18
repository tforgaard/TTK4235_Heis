#include "manage_elevator.h"

#ifndef FSM_H
#define FSM_H

int current_floor;

void Elevator_update_current_floor();

void Elevator_initialize();

void FSM_update(Elevator_state * current_state, Elevator_state * last_state);

void FSM_run();

void FSM_stop();

#endif