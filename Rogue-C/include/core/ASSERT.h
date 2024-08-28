#pragma once
#include "./include/core/LOG.h"

//void my_assert(bool condition, const char* msg);

#ifdef LOG_LEVEL_ALL
    #define ASSERT(condition, ...) if(condition == false) {\
        LOG_ERROR(__VA_ARGS__);\
    }
#elif LOG_LEVEL_WARNING
    #define ASSERT(condition, ...) if(condition == false) {\
        LOG_ERROR(__VA_ARGS__);\
    }
#elif LOG_LEVEL_ERROR
    #define ASSERT(condition, ...) if(condition == false) {\
        LOG_ERROR(__VA_ARGS__);\
    }
#else
    #define ASSERT(condition, ...)
#endif