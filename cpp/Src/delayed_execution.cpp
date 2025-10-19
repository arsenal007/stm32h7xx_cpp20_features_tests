
#include <functional>
#include "tim.h"
#include <delayed_execution.hpp>
#include <interrupt_simple_timer.h>

namespace
{
    std::function<void(void)> g_interrupt_handler{[](void) {}};
}

void interrupt_simple_timer(void)
{
    if (g_interrupt_handler)
        g_interrupt_handler();
}


void delayed_execution(std::function<void(void)> && callback)
{
    MX_TIM7_Init();
    // start timer with interrupt 5sec
    HAL_TIM_Base_Start_IT(&htim7); 
    g_interrupt_handler = callback;
}
