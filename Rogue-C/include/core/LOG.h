#pragma once
#include <iostream>
#include <chrono>
#include <ctime>

#define INFO 0
#define WARNING 1
#define ERROR 2

#if LOG_LEVEL_ALL || LOG_LEVEL_WARNING || LOG_LEVEL_ERROR
    #define LOG_MESSAGE(level, ...) {\
        std::string color;\
        std::string logLevelStr = "INFO";\
        if (level == INFO) {\
            color = "\033[37m";\
        }\
        else if (level == WARNING) {\
            color = "\033[33m";\
            logLevelStr = "WARNING";\
        }\
        else if (level == ERROR) {\
            color = "\033[31m";\
            logLevelStr = "ERROR";\
        }\
        else {\
            color = "\033[0m";\
        }\
        auto now = std::chrono::system_clock::now();\
        std::time_t time = std::chrono::system_clock::to_time_t(now);\
        std::tm local_tm = *std::localtime(&time);\
        std::cout << color << "[" << std::put_time(&local_tm, "%H:%M:%S") << "] " << logLevelStr << ": " << std::format(__VA_ARGS__) << std::endl << \
        " |=> in " << __FILE__ << ":" << __LINE__ << "\033[0m" << std::endl;\
    }
#endif

#ifdef LOG_TO_FILE
    #include <fstream>
    void open_log();
    void close_log();
    
    #define OPEN_LOG() open_log()
    #define CLOSE_LOG() close_log()
#else
    #define OPEN_LOG()
    #define CLOSE_LOG()
#endif

#ifdef LOG_LEVEL_ALL
    #define LOG(...) LOG_MESSAGE(INFO, __VA_ARGS__)
    #define LOG_WARNING(...) LOG_MESSAGE(WARNING, __VA_ARGS__)
    #define LOG_ERROR(...) LOG_MESSAGE(ERROR, __VA_ARGS__)
#elif LOG_LEVEL_WARNING
    #define LOG(...)
    #define LOG_WARNING(...) LOG_MESSAGE(WARNING, __VA_ARGS__)
    #define LOG_ERROR(...) LOG_MESSAGE(ERROR, __VA_ARGS__)
#elif LOG_LEVEL_ERROR
    #define LOG(...)
    #define LOG_WARNING(...)
    #define LOG_ERROR(...) LOG_MESSAGE(ERROR, __VA_ARGS__)
#else
    #define LOG(...)
    #define LOG_WARNING(...)
    #define LOG_ERROR(...)
#endif


