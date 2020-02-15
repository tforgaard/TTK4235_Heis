#include "hardware.h"

#ifndef INPUT_H
#define INPUT_H

typedef enum button{
    HARDWARE_ORDER_UP,
    HARDWARE_ORDER_INSIDE,
    HARDWARE_ORDER_DOWN
} button;

typedef struct Input
{
    int stopSignal;
    int obstructionSignal;
    int floorSignal[4];
    int lastFloor;
    int buttonOrderDown[HARDWARE_NUMBER_OF_FLOORS];
    int buttonOrderUp[HARDWARE_NUMBER_OF_FLOORS];
    int buttonOrderInside[HARDWARE_NUMBER_OF_FLOORS];

} Input;

void Input_check_elevator_buttons();

void Input_readStopSignal();

void Input_setOrderLights();

void Input_setFloorSignal();

void Input_removeButtonOrderDown(int floor);

void Input_removeButtonOrderUp(int floor);

void Input_removeButtonOrderInside(int floor);

int Input_getLastFloor();

int * Input_get_buttonOrderUp();

int * Input_get_buttonOrderDown();

int * Input_get_buttonOrderInside();

void Input_update();

#endif