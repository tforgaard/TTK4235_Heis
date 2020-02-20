#ifndef FSM2_H
#define FSM2_H

typedef enum state 
{
IDLE,
STOPPING,
MOVING_UP,
MOVING_DOWN
} state;

void FSM_init();

void FSM_update(state * current_state, state * next_state);

void FSM_run();

void FSM_stop();

void FSM_idle(state * current_state);

void FSM_stopping(state * current_state, state * next_state);

void FSM_moving_down(state * current_state, state * next_state);

void FSM_moving_up(state * current_state, state * next_state);

#endif