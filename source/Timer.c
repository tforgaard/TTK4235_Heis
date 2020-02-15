#include <time.h>
#include "Timer.h"

time_t Timer_time;

void Timer_set(){
    Timer_time = clock();
}
int Timer_get(){
    return (clock()-Timer_time) * 1000 / CLOCKS_PER_SEC;
}