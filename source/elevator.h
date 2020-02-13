#ifndef ELEVATOR_H
#define ELEVATOR_H


typedef enum Elevator_state 
{
moving_up_to_service,
moving_down_to_service,
stopping_on_down,
stopping_on_up,
moving_to_highest_order,
moving_to_lowest_order,
idle
} Elevator_state;

int current_floor = -1;



int Elevator_get_current_floor(){return current_floor;}
void Elevator_update_current_floor();

void Elevator_initialize();

void Elevator_update(Elevator_state * current_state);

void Elevator_run();

#endif