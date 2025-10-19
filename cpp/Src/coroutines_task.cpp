#include <coroutines_task.h>
#include <simple.hpp>
#include <runtime_initial_suspend.hpp>
#include <task_example.hpp>
#include <delayed_execution.hpp>
#include <cstdio>
#include "FreeRTOS.h"

void coroutines_task(void)
{
  simple::execute();
  runtime_initial_suspend::execute();
  task_example::execute();
}
