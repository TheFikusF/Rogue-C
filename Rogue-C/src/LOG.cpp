#include "LOG.h"

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