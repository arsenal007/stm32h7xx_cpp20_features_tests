#include "parse_uart_command.h"
#include <string>
#include <cstdio>
#include "FreeRTOS.h"
#include <delayed_execution.hpp>

namespace
{
    inline void handleCommand(const std::string cmd)
    {
        if (cmd == "DELAYED")
        {
            delayed_execution([]()
                              { printf("Delayed execution after 5 seconds:\n"); 
                      if (xPortIsInsideInterrupt()) 
                        printf("interrupt contex\n"); 
                      else 
                        printf("Delayed execution after 5 seconds from task context\n"); });
        }
    }
}

void parse_uart_command(const char *input)
{
    handleCommand(input);
}