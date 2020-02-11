#include "hardware.h"

int up_orders[HARDWARE_NUMBER_OF_FLOORS];
int down_orders[HARDWARE_NUMBER_OF_FLOORS];

/* int get_up_orders();
int get_down_orders(); */

int Orders_floor_is_in_up_orders(int floor);
int Orders_floor_is_in_down_orders(int floor);

int Orders_get_highest_order(); //among down orders
int Orders_get_lowest_order(); //among up orders

int Orders_up_orders_is_empty(); //bool
int Orders_down_orders_is_empty();

void Orders_set_up_order(int floor);
void Orders_set_down_order(int floor);

void Orders_remove_up_order(int floor);
void Orders_remove_down_order(int floor);

/* void remove_all_orders();

void get_orders_from_hardware(); */

