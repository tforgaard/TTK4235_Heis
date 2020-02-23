#ifndef ORDERS_H
#define ORDERS_H

int *up_orders;
int *down_orders;

int floor_count;

int Orders_init(int number_of_floors);

int Orders_floor_is_in_up_orders(int floor);
int Orders_floor_is_in_down_orders(int floor);

int Orders_get_highest_order(); //among down orders
int Orders_get_lowest_order();  //among up orders

int Orders_up_orders_is_empty();
int Orders_down_orders_is_empty();

void Orders_remove_up_order(int floor);
void Orders_remove_down_order(int floor);

void Orders_remove_all_orders();

void Orders_set_up_order(int floor);
void Orders_set_down_order(int floor);

int Orders_up_order_over_floor(int current_floor);
int Orders_up_order_under_floor(int current_floor);

int Orders_down_order_over_floor(int current_floor);
int Orders_down_order_under_floor(int current_floor);

void Orders_recieve_and_set_orders(int * orders);

#endif
