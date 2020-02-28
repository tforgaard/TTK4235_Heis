/**
 * @file
 * @brief Elevator module provides an elevator structure and means to update it via hardware.
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
    int current_floor;          /** < The elevators last floor */
    int is_above_current_floor; /** < Flag to check if elevator is above or below the @c current_floor */
    int is_at_floor;            /** < Flag to check if elevator is at a floor */

    int doors_are_open;       /** < Flag to check if elevator doors are open */
    int doors_are_obstructed; /** < Flag to check if elevator doors are obstructed */

    int stop_button_is_pressed; /** < Flag to check if elevator stop button is pressed */

    RunningState running_state; /** < Enum containing the running state */
} Elevator;

/**
 * @brief Will initialise elevator by moving up until it finds a floor. 
 * Also sets @c current_floor in @p p_elevator object.
 * 
 * @param[out] p_elevator A pointer to an Elevator struct.
*/
void Elevator_init(Elevator *p_elevator);

/**
 * @brief Will react to hardware events. Updates elevator values, sets orders and lights.
 * 
 * @param[in, out] elevator Elevator object to be updated.
 * 
 * @param[out] up_orders Pointer to @p p_up_orders array to be updated.
 * 
 * @param[out] down_orders Pointer to @p p_down_orders array to be updated. 
*/
void Elevator_update(Elevator *p_elevator, int *p_up_orders, int *p_down_orders);

/// @private
void Elevator_update_at_floor(Elevator *p_elevator);

/**
 * @brief Will update the value of @c current_floor and @c Elevator_is_above_floor in @p p_elevator object.
 * 
 * @param[out] p_elevator A pointer to an Elevator struct.
 * 
*/
void Elevator_update_current_floor(Elevator *p_elevator);

/// @private
void Elevator_update_obstruction_signal(Elevator *p_elevator);

/**
 * @brief Will turn off order up, down and inside lights at a given @c floor.
 * 
 * @param[in] floor the @p floor to turn off lights at.
*/
void Elevator_turn_off_lights_at(int floor);

/**
 * @brief Will turn off all elevator lights.
*/
void Elevator_turn_off_all_lights();

#endif