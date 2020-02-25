/**
 * @file
 * @brief Elevator module provide facilities to interact with hardware.
 */

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

/**
 * @brief Initialises elevator by moving up until it finds a floor. 
 * Current floor is stored.
*/
void Elevator_init();

/**
 * @brief Function responsible for polling hardware for orders and setting order lights.
 * Allocates memory to hold @c 2*HARDWARE_NUMBER_OF_FLOORS integers.
 * Orders are stored in this manner:
 * [up1, up2, ... , upNFLOORS, down1, down2, ... , downNFLOORS]. 
 *
 * @return @c int pointer to orders. 
 * 
 * @warning Receiving function most free memory! 
*/
int *Elevator_check_buttons();

/**
 * @brief Checks whether the elevator is at a floor or between floors.
 *
 * @return 1 if elevator is at a floor. 0 if not. 
*/
int Elevator_at_floor();

/**
 * @brief Get the last known floor.
 *
 * @return the last floor the elevator was at in range [0, NUMBER_OF_FLOORS)
*/
int Elevator_get_current_floor();

/**
 * @brief Will update the value of current floor.
 * Will also set the value @c Elevator_is_above_floor if the elevator is not at a floor.
 *
 * @param[in] direction The direction the elevator is moving at, can also be @c IDLE .
*/
void Elevator_update_current_floor(state direction);

//TODO: få disse inn i FSM.
void Elevator_open_doors();
int Elevator_get_open_doors_flag();
void Elevator_close_doors();

/**
 * @brief Will turn off order up lights and inside lights at current floor.
*/
void Elevator_finished_up_order(); //Change to up lights?

/**
 * @brief Will turn off order down lights and inside lights at current floor.
*/
void Elevator_finished_down_order();

/**
 * @brief Will turn off all up, down and inside lights.
*/
void Elevator_turn_off_all_lights();

#endif