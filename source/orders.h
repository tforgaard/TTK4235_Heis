/**
 * @file
 * @brief Order module keeps track of up and down orders,
 * and provide operations on them. 
 */

#ifndef ORDERS_H
#define ORDERS_H

/**
 * @brief A structure to contain pointers to the two order arrays and the floor count.
 */
typedef struct Orders
{
    int *p_up_orders;   ///< Pointer to up orders array
    int *p_down_orders; ///< Pointer to up orders array
    int floor_count;    ///< The number of floors
} Orders;

/**
 * @brief Order types, either up, down or both.
 */
typedef enum OrderType
{
    UP,
    DOWN,
    BOTH
} OrderType;

/**
 * @brief Initializes an Orders struct with the given number of floors,
 * makes two arrays for orders with given length.
 * 
 * @param[out] p_orders A pointer to an @c Orders struct to initialize.
 * 
 * @param[in] number_of_floors The number of floors to make order arrays for.
 * 
 * @return 0 on success, -1 if either of the pointers isn't initialized properly.
 * 
 */
int Orders_init(Orders *p_orders, int number_of_floors);

/**
 * @brief Frees the dynamically allocated order arrays.
 * 
 * @param[out] p_orders A pointer to an @c Orders struct containing pointers to the order arrays.
 * 
 */
void Orders_free(Orders *p_orders);

/**
 * @brief Checks if the given floor has an order.
 * 
 * @param[in] p_orders A pointer to an @c Orders struct containing pointers to the order arrays.
 * 
 * @param[in] type which @c OrderType to check in the given floor.
 * 
 * @param[in] floor the @p floor to check in the order arrays.
 * 
 * @return 1 if the floor contains an order of the specific type, 0 if not.
 * 
 */
int Orders_floor_is_in(Orders *p_orders, OrderType type, int floor);

/**
 * @brief Looks for either the lowest up order, or the highest down order.
 * 
 * @param[in] p_orders A pointer to an @c Orders struct containing pointers to the order arrays.
 * 
 * @param[in] type which @c OrderType to check for.
 * 
 * @return the floor number of highest/lowest order, -1 if there is no order.
 * 
 */
int Orders_extreme_value_of(Orders *p_orders, OrderType type);

/**
 * @brief Removes a given order from the order arrays.
 * 
 * @param[in, out] p_orders A pointer to an @c Orders struct containing pointers to the order arrays.
 * 
 * @param[in] type which @c OrderType to check for.
 * 
 * @param[in] floor the @p floor to remove order from.
 * 
 */
void Orders_remove(Orders *p_orders, OrderType type, int floor);

/**
 * @brief Removes all orders.
 * 
 * @param[out] p_orders A pointer to an @c Orders struct containing pointers to the order arrays.
 * 
 */
void Orders_remove_all(Orders *p_orders);

/**
 * @brief Checks if there is a specific order over the floor.
 * 
 * @param[in] p_orders A pointer to an @c Orders struct containing pointers to the order arrays.
 * 
 * @param[in] type which @c OrderType to check for.
 * 
 * @param[in] floor the @p floor to check above.
 * 
 * @return 1 if there exists an order over the floor, 0 if not.
 * 
 */
int Orders_order_over(Orders *p_orders, OrderType type, int floor);

/**
 * @brief Checks if there is a specific order under the floor.
 * 
 * @param[in] p_orders A pointer to an @c Orders struct containing pointers to the order arrays.
 * 
 * @param[in] type which @c OrderType to check for.
 * 
 * @param[in] floor the @p floor to check under.
 * 
 * @return 1 if there exists an order under the floor, 0 if not.
 * 
 */
int Orders_order_under(Orders *p_orders, OrderType type, int floor);

#endif