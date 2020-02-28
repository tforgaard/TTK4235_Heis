/**
 * @file
 * @brief Elevator module provide facilities to interact with hardware.
 */

#ifndef ELEVATOR_H
#define ELEVATOR_H

/**
 * @brief Running states.
 */
typedef enum
{
    IDLE,
    MOVING_UP,
    MOVING_DOWN
} RunningState;

/**
 * @brief Type that holds all relevant information about the elvator.
 */
typedef struct
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
 * @brief Will initialise elevator by moving up until it finds a floor. 
 * Also sets @c current_floor in @p elevator object.
*/
void Elevator_init(Elevator *elevator);

/**
 * @brief Will reacting to harware events. Updates elevator values, sets orders and lights.
 * 
 * @param[in, out] elevator Elevator object to be updated.
 * 
 * @param[out] up_orders Pointer to up orders array to be updated.
 * 
 * @param[out] down_orders Pointer to down orders array to be updated. 
*/
void Elevator_update(Elevator *elevator, int * up_orders, int * down_orders);

/**
 * @brief Will update the value of @c is_at_floor in @p elevator object.
*/
void Elevator_update_at_floor(Elevator *elevator);


/**
 * @brief Will update the value of @c current_floor and @c Elevator_is_above_floor in @p elevator object.
*/
void Elevator_update_current_floor(Elevator *elevator);


/**
 * @brief Will update the value of @c doors_are_obstructed in @p elevator object.
 * 
*/
void Elevator_update_obstruction_signal(Elevator *elevator);

/**
 * @brief Will turn off order up, down and inside lights at current @p floor.
*/
void Elevator_turn_off_lights_at(int floor);

/**
 * @brief Will turn off all elevator lights.
*/
void Elevator_turn_off_all_lights();

#endif