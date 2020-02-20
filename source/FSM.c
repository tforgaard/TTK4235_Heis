#include "FSM.h"
#include "Orders.h"
#include "Timer.h"
#include "manage_elevator.h"

    state current_state = IDLE;
    state next_state = IDLE;

void FSM_init() {
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    Orders_init(HARDWARE_NUMBER_OF_FLOORS);
    Elevator_init();
}

void FSM_stop()
{
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    hardware_command_stop_light(1);

    for (int i=0; i<HARDWARE_NUMBER_OF_FLOORS; i++){
        Orders_remove_up_order(i);
        Orders_remove_down_order(i);
        hardware_command_order_light(i,HARDWARE_ORDER_DOWN,0);
        hardware_command_order_light(i,HARDWARE_ORDER_UP,0);
        hardware_command_order_light(i,HARDWARE_ORDER_INSIDE,0);
    }
}

void FSM_run(){
    FSM_init();


    
    while(1)
    {
        FSM_update(&current_state, &next_state);
    }
}

void FSM_update(state * current_state, state * next_state)
{
    if (hardware_read_stop_signal())
    {
        FSM_stop();

        if ( Elevator_at_floor())
        {
            *current_state = IDLE;
            hardware_command_door_open(1);
            Elevator_open_doors();
            Timer_set();
        }
        else
        {
            *current_state = IDLE;
        }
    }
    else if (Elevator_get_open_doors_flag())
    {
        Elevator_check_buttons();
        hardware_command_stop_light(0);
        hardware_command_door_open(1);

        if(hardware_read_obstruction_signal())
        {
            hardware_command_door_open(1);
            Timer_set();
        }
        else
        {
            if(Timer_get() >= 3000)
            {
                Elevator_close_doors();
                hardware_command_door_open(0);
            }
        }
    }
    else
    {
        Elevator_update_current_floor();
        Elevator_check_buttons();
        hardware_command_stop_light(0);

        switch (*current_state)
        {
        case IDLE:
            FSM_idle(current_state);
            break;
        
        case STOPPING:
            FSM_stopping(current_state, next_state);
            break;
        
        case MOVING_UP:
            FSM_moving_up(current_state, next_state);
            break;
        
        case MOVING_DOWN:
            FSM_moving_down(current_state, next_state);
            break;

        default:
            *current_state = IDLE;
            break;
        }
    }
}

void FSM_idle(state * current_state)
{
    if (Orders_down_order_over_floor(current_floor))
    {
        *current_state = MOVING_UP;
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
    }
    else if (Orders_up_order_under_floor(current_floor))
    {
        *current_state = MOVING_DOWN;
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
    else if (Orders_down_order_under_floor(current_floor))
    {
        *current_state = MOVING_DOWN;
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
    else if (Orders_up_order_over_floor(current_floor))
    {
        *current_state = MOVING_UP;
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
    }
    else
    {
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    }
}

void FSM_stopping(state * current_state, state * next_state)
{
    if (*next_state == MOVING_DOWN)
    {   
        if (!Orders_down_orders_is_empty())
        {
            *current_state = MOVING_DOWN;
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        }
        else
        {
            *current_state = IDLE;
            hardware_command_movement(HARDWARE_MOVEMENT_STOP); 
        }
            
    } 
    else if (*next_state == MOVING_UP)
    {
        if (!Orders_up_orders_is_empty())
        {
            *current_state = MOVING_UP;
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
        }
        else
        {
            *current_state = IDLE;
            hardware_command_movement(HARDWARE_MOVEMENT_STOP); 
        } 
    }   
}

void FSM_moving_down(state * current_state, state * next_state)
{
    if (Elevator_at_floor())
    {
        if (Orders_floor_is_in_down_orders(current_floor) || (Orders_get_lowest_order() == current_floor && !Orders_down_order_under_floor(current_floor)))
        {
          
            Elevator_open_doors();
            Timer_set();
            *current_state = STOPPING;
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);

            if (Orders_down_order_under_floor(current_floor) || Orders_up_order_under_floor(current_floor))
            {
                *next_state = MOVING_DOWN;
            }
            else
            {
                *next_state = MOVING_UP;
            }
            Elevator_finished_down_order();
        }
    }
}

void FSM_moving_up(state * current_state, state * next_state)
{
    if (Elevator_at_floor())
    {       
        if (Orders_floor_is_in_up_orders(current_floor) || (Orders_get_highest_order() == current_floor && !Orders_up_order_over_floor(current_floor)))
        {
            
            Elevator_open_doors();
            Timer_set();
            *current_state = STOPPING;
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);

            if (Orders_up_order_over_floor(current_floor) || Orders_down_order_over_floor(current_floor))
            {
                *next_state = MOVING_UP;
            }
            else
            {
                *next_state = MOVING_DOWN;
            }
            Elevator_finished_up_order();
        }
    }
}

/*
Bugs:
1. Etter et stoptrykk, tror heisen den fortsatt er i forrige etasje
Fikset: 2. Bestillinger blir tatt imot til etasjen den er i.
Fikset: 3. Dersom den går ned til 1. og skal opp etterpå, vil den fortsette å gå ned. Gjelder bare når man får down order fra outside. Gjelder bare når den ikke er mellom idle.
4. Sletter feil bestilling. Går ned fra 2 etasje til 1. Trykker på ned knappen ute i 2. etasje. Da blir denne slettet. 
Fikset: 5. Står i 1. etasje, noen trykker ned ute i 3, noen trykker opp ute i 2. Stopper i 2. etasje og forsetter ned. 
6. Venter av og til 6 sekunder.
7. Theo testen: går tilbake til 1. etasje, selvom lyset i 1. etasje går av i starten. 


*/