#pragma once
#include <chrono>
#include <ctime>

#define INFO 0
#define WARNING 1
#define ERROR 2

void log_message(const int& level, const std::string& message) {
    std::string color;
    std::string logLevelStr = "INFO";
    
    if (level == INFO) {
        color = "\033[37m";
    }
    else if (level == WARNING) {
        color = "\033[33m";
        logLevelStr = "WARNING";
    }
    else if (level == ERROR) {
        color = "\033[31m";
        logLevelStr = "ERROR";
    }
    else {
        color = "\033[0m";
    }

    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm = *std::localtime(&time);

    std::cout << color << "[" << std::put_time(&local_tm, "%H:%M:%S") << "] " << logLevelStr << ": " << message << "\033[0m" << std::endl;
}

#ifdef LOG_LEVEL_ALL
    #define LOG(msg) log_message(DEFAULT, msg)
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


