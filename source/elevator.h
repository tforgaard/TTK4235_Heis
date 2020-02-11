#include "orderManager.h"

typedef enum Elevator_state 
{
idle, 
moving_up,
moving_down,
stopping_on_down,
stopping_on_up,
moving_to_highest,
moving_to_lowest 
};

void Elevator_next_state(Elevator_state * current_state)
{
    switch (*current_state)
    {
    case idle:
            if (!up_orders_is_empty)
            {
                *current_state = moving_to_highest;
            }

            else if (!down_orders_is_empty)
            {
               *current_state = moving_to_lowest; 
            }
            
            
            
        break;
    
    default:
        break;
    }
}
