#include "hardware.h"

int destination_up_array[HARDWARE_NUMBER_OF_FLOORS];
int destination_down_array[HARDWARE_NUMBER_OF_FLOORS];

int get_up_orders();
int get_down_orders();

int up_orders_is_empty(); //bool
int down_orders_is_empty();

void remove_up_order(int floor);
void remove_down_order(int floor);

void remove_all_orders()

