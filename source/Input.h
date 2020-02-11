#include <hardware.h>
#include <Orders.h>

typedef struct Input
{
    int stopSignal;
    int obstructionSignal;
    int floorSignal[4];
    int lastFloor;
    int buttonOrderDown[3];
    int buttonOrderUp[3];
    int buttonOrderInside[4];

} Input;

void Input_check_elevator_buttons();

void Input_readStopSignal();

void Input_setOrderLights();

void Input_setFloorSignal();

void Input_removeButtonOrderDown(int floor);

void Input_removeButtonOrderUp(int floor);

void Input_removeButtonOrderInside(int floor);

int Input_getLastFloor();

// void Input_manageAll();
// int Input_getStopSignal();
// int Input_getObstructionSignal();

// void Input_ManageOrders() 
// Må ha en god måte å gå fra bestillingssignalene buttonOrder... til Orders_set_..._order