#ifndef DELAYED_EXECUTION_H
#define DELAYED_EXECUTION_H 

#include <functional>
void delayed_execution(std::function<void(void)> && callback);

#endif // DELAYED_EXECUTION_H