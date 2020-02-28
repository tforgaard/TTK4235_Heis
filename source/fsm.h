/**
 * @file
 * @brief FSM module updates the state of the elevator.
 */

#include "elevator.h" //Need this for some reason.. why?
#include "orders.h"

#ifndef FSM_H
#define FSM_H
#define STOPTIME 3000

void FSM_update(Elevator *elevator, Orders *orders);

void FSM_running(Elevator *elevator, Orders *orders);

void FSM_stop_button_engaged(Elevator *elevator, Orders *orders);

void FSM_doors_open(Elevator *elevator);

void FSM_idle(Elevator *elevator, Orders *orders);

void FSM_moving_down(Elevator *elevator, Orders *orders);

void FSM_moving_up(Elevator *elevator, Orders *orders);

//static void FSM_stopping_sequence(Elevator *elevator, Orders *orders);

#endif