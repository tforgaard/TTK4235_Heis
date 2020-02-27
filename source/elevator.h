/**
 * @file
 * @brief Elevator module provide facilities to interact with hardware.
 */

#ifndef ELEVATOR_H
#define ELEVATOR_H

typedef enum RunningState
{
    IDLE,
    MOVING_UP,
    MOVING_DOWN
} RunningState;

typedef struct Elevator
{
    int current_floor;
    int is_above_current_floor;
    int is_at_floor;

    int doors_are_open;
    int doors_are_obstructed;
    
    int stop_button_is_pressed;

    RunningState running_state;
} Elevator;


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
void Elevator_update(Elevator *elevator, int * up_orders, int * down_orders);

/**
 * @brief Checks whether the elevator is at a floor or between floors.
 *
 * @return 1 if elevator is at a floor. 0 if not. 
*/
void Elevator_update_at_floor(Elevator *elevator);


/**
 * @brief Will update the value of current floor.
 * Will also set the value @c Elevator_is_above_floor if the elevator is not at a floor.
 *
 * @param[in] direction The direction the elevator is moving at, can also be @c IDLE .
*/
void Elevator_update_current_floor(Elevator *elevator);


/**
 * @brief Will update the value of obstruction signal.
 * 
 * @param[in] direction The direction the elevator is moving at, can also be @c IDLE .
*/
void Elevator_update_obstruction_signal(Elevator *elevator);

/**
 * @brief Will turn off order up lights and inside lights at current floor.
*/
void Elevator_turn_off_lights_at(int floor);
void Elevator_turn_off_all_lights();

#endif