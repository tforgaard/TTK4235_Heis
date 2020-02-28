/**
 * @file
 * @brief Timer module is a stopwatch.
 */

#ifndef TIMER_H
#define TIMER_H

/**
 * @brief Resets the stopwatch and starts it.
 *
*/
void Timer_set();

/**
 * @brief Returns the time since the timer was set.
 *
 * @return the time in milliseconds.
 * 
*/
int Timer_get();

#endif