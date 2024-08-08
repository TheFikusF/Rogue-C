#pragma once
#include "LOG.h"

void my_assert(bool condition, std::string msg);

#ifdef LOG_LEVEL_ALL
    #define ASSERT(condition, msg) my_assert(condition, msg);
#elif LOG_LEVEL_WARNING
    #define ASSERT(condition, msg) my_assert(condition, msg);
#elif LOG_LEVEL_ERROR
    #define ASSERT(condition, msg) my_assert(condition, msg);
#else
    #define ASSERT(condition, msg);
#endif