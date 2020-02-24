#include "manage_elevator.h" //Need this for some reason.. why?

#ifndef FSM_H
#define FSM_H

void FSM_init();

void FSM_update(state *current_state);

void FSM_running(state *current_state);

void FSM_stop_button_engaged(state *current_state);

void FSM_doors_open(state *current_state);

void FSM_idle(state *current_state);

void FSM_moving_down(state *current_state);

void FSM_moving_up(state *current_state);

#endif