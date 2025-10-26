#include "parse_uart_command.h"
#include <string>
#include <cstdio>
#include "FreeRTOS.h"
#include <delayed_execution.hpp>
#include <coroutine>
#include <optional>
#include <handler.hpp>
#include <memory>
#include <coroutines/simple.hpp>
#include <coroutines/runtime_initial_suspend.hpp>
#include <coroutines/tasks/passing_data_over_promise.hpp>
#include <coroutines/tasks/delayed.hpp>
#include <coroutines/tasks/passing_data_over_coroutine_argument.hpp>
#include <coroutines/generators/Awaitable.hpp>
#include <coroutines/generators/fibonacci_co_yield.hpp>

namespace
{

    inline void handleCommand(const std::string cmd)
    {
        if (cmd == "CORO_SIMPLE")
        {
            coroutines::simple();
        }
        else if (cmd == "DELAYED")
        {
            delayed_execution([]()
                              { printf("Delayed execution after 5 seconds:\n"); 
                      if (xPortIsInsideInterrupt()) 
                        printf("interrupt contex\n"); 
                      else 
                        printf("Delayed execution after 5 seconds from task context\n"); });
        }
        else if (cmd == "CORO_DELAYED")
        {
            coroutines::tasks::delayed();
        }
        else if (cmd == "CORO_PASSING_DATA_OVER_PROMISE")
        {
            coroutines::tasks::passing_data_over_promise();
        }
        else if (cmd == "CORO_PASSING_DATA_OVER_COROUTINE_ARGUMENT")
        {
            coroutines::tasks::passing_data_over_coroutine_argument();
        }
        else if (cmd == "CORO_RUNTIME_INITIAL_SUSPEND")
        {
            coroutines::runtime_initital_suspend();
        }
        else if (cmd == "CORO_GENERATOR_AWAITABLE")
        {
            coroutines::generators::awaitable();
        }
        else if (cmd == "CORO_GENERATOR_CO_YIELD")
        {
            coroutines::generators::fibonacci_co_yield();
        }

        else

        {
            printf("Unknown command: %s\n", cmd.c_str());
        }
    }
}

void parse_uart_command(const char *input)
{
    handleCommand(input);
}