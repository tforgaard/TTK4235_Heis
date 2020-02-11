#include <Timer.h>

time_t Timer_time;
int Timer_flag;

void Timer_set(){
    if (!Timer_flag){
        Timer_time = clock();
        Timer_flag = 1;
    }
}
int Timer_is_set(){
    return Timer_flag;
}
int Timer_get(){
    return (clock()-Timer_time) * 1000 / CLOCKS_PER_SEC;
}
void Timer_reset(){
    Timer_flag = 0;
}