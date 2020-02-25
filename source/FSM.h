/**
 * @file
 * @brief FSM module updates the state of the elevator.
 */

#include "manage_elevator.h" //Need this for some reason.. why?
#include "Orders.h"


#ifndef FSM_H
#define FSM_H

void FSM_init();

void FSM_update(state *current_state, order *current_order);

void FSM_running(state *current_state, order *current_order);

void FSM_stop_button_engaged(state *current_state);

void FSM_doors_open(state *current_state);

void FSM_idle(state *current_state, order *current_order);

void FSM_moving(state *current_state, order *current_order);

#endif