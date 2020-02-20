#include "hardware.h"

#ifndef MANAGE_ELEVATOR_H
#define MANAGE_ELEVATOR_H

int current_floor;

int elevator_floor_signal[HARDWARE_NUMBER_OF_FLOORS]; //fjern?

int elevator_open_doors_flag;

void Elevator_init();

void Elevator_check_buttons();

int Elevator_at_floor();

int Elevator_get_current_floor();

void Elevator_update_current_floor();

void Elevator_open_doors();

int Elevator_get_open_doors_flag();

void Elevator_close_doors();

void Elevator_finished_up_order();

void Elevator_finished_down_order();

#endif