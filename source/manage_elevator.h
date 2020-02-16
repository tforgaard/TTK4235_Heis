#include "hardware.h"

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
} Elevator_state;

int current_floor;

int elevator_floor_signal[HARDWARE_NUMBER_OF_FLOORS]; //fjern?
int elevator_last_floor;

int elevator_open_doors_flag;


void Elevator_initialize();


void Elevator_moving_up_to_service(Elevator_state * current_state, Elevator_state * last_state);

void Elevator_moving_down_to_service(Elevator_state * current_state, Elevator_state * last_state);

void Elevator_stopping_on_down(Elevator_state * current_state, Elevator_state * last_state);

void Elevator_stopping_on_up(Elevator_state * current_state, Elevator_state * last_state);

void Elevator_moving_to_highest_order(Elevator_state * current_state, Elevator_state * last_state);

void Elevator_moving_to_lowest_order(Elevator_state * current_state, Elevator_state * last_state);

void Elevator_idle(Elevator_state * current_state, Elevator_state * last_state);


void Elevator_check_buttons();

int Elevator_at_floor();

int Elevator_get_last_floor();

void Elevator_update_current_floor();

void Elevator_open_doors();

int Elevator_get_open_doors_flag();

void Elevator_close_doors();

#endif