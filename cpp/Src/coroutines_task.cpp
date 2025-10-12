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
  delayed_execution([]()
                    { printf("Delayed execution after 5 seconds:\n"); 
                      if (xPortIsInsideInterrupt()) 
                        printf("interrupt contex\n"); 
                      else 
                        printf("Delayed execution after 5 seconds from task context\n"); });
}
