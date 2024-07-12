#pragma once
#include <iostream>
#include <chrono>
#include <ctime>

#define INFO 0
#define WARNING 1
#define ERROR 2

void log_message(const int& level, const std::string& message);

#ifdef LOG_LEVEL_ALL
    #define LOG(msg) log_message(INFO, msg)
    #define LOG_WARNING(msg) log_message(WARNING, msg)
    #define LOG_ERROR(msg) log_message(ERROR, msg)
#elif LOG_LEVEL_WARNING
    #define LOG(msg)
    #define LOG_WARNING(msg) log_message(WARNING, msg)
    #define LOG_ERROR(msg) log_message(ERROR, msg)
#elif LOG_LEVEL_ERROR
    #define LOG(msg)
    #define LOG_WARNING(msg)
    #define LOG_ERROR(msg) log_message(ERROR, msg)
#else
    #define LOG(msg)
    #define LOG_WARNING(msg)
    #define LOG_ERROR(msg)
#endif


