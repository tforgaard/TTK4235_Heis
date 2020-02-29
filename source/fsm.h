/**
 * @file
 * @brief FSM module controls the elevator.
 */

#include "elevator.h"
#include "orders.h"

#ifndef FSM_H
#define FSM_H
#define STOPTIME 3000

/**
 * @brief Updates the finite state machine
 * 
 * @param[out] p_elevator pointer to the Elevator struct
 * 
 * @param[out] p_orders pointer to the Orders struct containing order arrays
 * 
 */
void FSM_update(Elevator *p_elevator, Orders *p_orders);

/// @private
void FSM_running(Elevator *p_elevator, Orders *p_orders);

/// @private
void FSM_stop_button_engaged(Elevator *p_elevator, Orders *p_orders);

/// @private
void FSM_doors_open(Elevator *p_elevator);

/// @private
void FSM_idle(Elevator *p_elevator, Orders *p_orders);

/// @private
void FSM_moving_down(Elevator *p_elevator, Orders *p_orders);

/// @private
void FSM_moving_up(Elevator *p_elevator, Orders *p_orders);

#endif