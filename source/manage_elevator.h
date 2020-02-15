#ifndef MANAGE_ELEVATOR_H
#define MANAGE_ELEVATOR_H

typedef enum Elevator_state 
{
moving_up_to_service,
moving_down_to_service,
stopping_on_down,
stopping_on_up,
moving_to_highest_order,
moving_to_lowest_order,
idle,
idle_with_door_open
} Elevator_state;

int current_floor;

void Elevator_update_current_floor();

void Elevator_initialize();

void Elevator_moving_up_to_service(Elevator_state * current_state, Elevator_state * last_state);

void Elevator_moving_down_to_service(Elevator_state * current_state, Elevator_state * last_state);

void Elevator_stopping_on_down(Elevator_state * current_state, Elevator_state * last_state);

void Elevator_stopping_on_up(Elevator_state * current_state, Elevator_state * last_state);

void Elevator_moving_to_highest_order(Elevator_state * current_state, Elevator_state * last_state);

void Elevator_moving_to_lowest_order(Elevator_state * current_state, Elevator_state * last_state);

void Elevator_idle_with_door_open(Elevator_state * current_state, Elevator_state * last_state);

void Elevator_idle(Elevator_state * current_state);

#endif