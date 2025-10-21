#include <coroutines_task.h>
#include <runtime_initial_suspend.hpp>
#include <delayed_execution.hpp>
#include <cstdio>
#include "FreeRTOS.h"

void coroutines_task(void)
{
  runtime_initial_suspend::execute();
}
