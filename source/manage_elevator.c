#include "manage_elevator.h"
#include "Orders.h"

void Elevator_init()
{
    current_floor = -1;
    hardware_command_movement(HARDWARE_MOVEMENT_UP);
    while (current_floor == -1)
    {
        for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
        {
            if (hardware_read_floor_sensor(i) == 1)
            {
                current_floor = i;
                break;
            }
        }
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
}

void Elevator_check_buttons()
{
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
    {

        if (Elevator_get_open_doors_flag() && i == current_floor)
        {
            continue;
        }

        if (hardware_read_order(i, HARDWARE_ORDER_UP))
        {
            hardware_command_order_light(i, HARDWARE_ORDER_UP, 1);
            Orders_set_up_order(i);
        }

        if (hardware_read_order(i, HARDWARE_ORDER_DOWN))
        {
            hardware_command_order_light(i, HARDWARE_ORDER_DOWN, 1);
            Orders_set_down_order(i);
        }

        if (hardware_read_order(i, HARDWARE_ORDER_INSIDE))
        {
            hardware_command_order_light(i, HARDWARE_ORDER_INSIDE, 1);
            if (i > current_floor)
            {
                Orders_set_up_order(i);
            }
            else
            {
                Orders_set_down_order(i);
            }
        }
    }
}

int Elevator_at_floor()
{
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
    {
        if (hardware_read_floor_sensor(i) == 1)
        {
            return 1;
        }
    }
    return 0;
}

int Elevator_get_current_floor()
{
    return current_floor;
}

void Elevator_update_current_floor()
{
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
    {
        if (hardware_read_floor_sensor(i) == 1)
        {
            elevator_floor_signal[i] = 1; //fjern
            current_floor = i;
            hardware_command_floor_indicator_on(i);
        }
        else
        {
            elevator_floor_signal[i] = 0; //fjern
        }
    }
}

void Elevator_open_doors()
{
    elevator_open_doors_flag = 1;
}

int Elevator_get_open_doors_flag()
{
    return elevator_open_doors_flag;
}

void Elevator_close_doors()
{
    elevator_open_doors_flag = 0;
}

void Elevator_finished_up_order()
{
    Orders_remove_up_order(Elevator_get_current_floor());
    hardware_command_order_light(Elevator_get_current_floor(), HARDWARE_ORDER_UP, 0);
    hardware_command_order_light(Elevator_get_current_floor(), HARDWARE_ORDER_INSIDE, 0);

    if ((Orders_get_highest_order() == current_floor) && !Orders_up_order_over_floor(current_floor))
    {
        Elevator_finished_down_order();
    }
}

void Elevator_finished_down_order()
{
    Orders_remove_down_order(Elevator_get_current_floor());
    hardware_command_order_light(Elevator_get_current_floor(), HARDWARE_ORDER_DOWN, 0);
    hardware_command_order_light(Elevator_get_current_floor(), HARDWARE_ORDER_INSIDE, 0);

    if (Orders_get_lowest_order() == current_floor && !Orders_down_order_under_floor(current_floor))
    {
        Elevator_finished_up_order();
    }
}