#include <hardware.h>
#include <Orders.h>

typedef struct Input
{
    int stopSignal;
    int obstructionSignal;
    int floorSignal[4];
    int buttonOrderDown[3];
    int buttonOrderUp[3];
    int buttonOrderInside[4];

} Input;

void Input_check_elevator_buttons();

void readStopSignal();

void setOrderLights();

void setFloorSignal();

void removeButtonOrderDown(int floor);

void removeButtonOrderUp(int floor);

void removeButtonOrderInside(int floor);