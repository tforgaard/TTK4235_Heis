#include <time.h>
#include "timer.h"

static time_t m_timer;

void Timer_set(){
    m_timer = clock();
}
int Timer_get(){
    return (clock()-m_timer) * 1000 / CLOCKS_PER_SEC;
}