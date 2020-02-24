#ifndef MANAGE_ELEVATOR_H
#define MANAGE_ELEVATOR_H

int current_floor;
int Elevator_is_above_current_floor;
int elevator_open_doors_flag;

typedef enum state
{
    IDLE,
    MOVING_UP,
    MOVING_DOWN
} state; 

void Elevator_init();

int * Elevator_check_buttons();

int Elevator_at_floor();

int Elevator_get_current_floor();

void Elevator_update_current_floor(state direction);

void Elevator_open_doors();

int Elevator_get_open_doors_flag();

void Elevator_close_doors();

void Elevator_finished_up_order();

void Elevator_finished_down_order();

void Elevator_turn_off_all_lights();

#endif