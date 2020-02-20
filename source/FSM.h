#include "manage_elevator.h"

#ifndef FSM_H
#define FSM_H

typedef enum Elevator_state 
{
moving_up_to_service, //0
moving_down_to_service, //1
stopping_on_down, //2
stopping_on_up, //3
moving_to_highest_order, //4
moving_to_lowest_order, // 5
idle, // 6
} Elevator_state;


Elevator_state current_state;
Elevator_state last_state;

void Elevator_update_current_floor();

void Elevator_initialize();

void FSM_update(Elevator_state * current_state, Elevator_state * last_state);

void FSM_run(Elevator_state * current_state, Elevator_state * last_state);

void FSM_stop();

void Elevator_moving_up_to_service(Elevator_state * current_state, Elevator_state * last_state);

void Elevator_moving_down_to_service(Elevator_state * current_state, Elevator_state * last_state);

void Elevator_stopping_on_down(Elevator_state * current_state, Elevator_state * last_state);

void Elevator_stopping_on_up(Elevator_state * current_state, Elevator_state * last_state);

void Elevator_moving_to_highest_order(Elevator_state * current_state, Elevator_state * last_state);

void Elevator_moving_to_lowest_order(Elevator_state * current_state, Elevator_state * last_state);

void Elevator_idle(Elevator_state * current_state, Elevator_state * last_state);

#endif