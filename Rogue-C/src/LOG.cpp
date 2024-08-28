#include "./include/core/LOG.h"

#ifdef LOG_TO_FILE
std::ofstream outfile;

void open_log() {
    //outfile = std::ofstream(std::format("log_{}.txt", std::chrono::system_clock::now()));
    outfile = std::ofstream("log.txt");
    outfile.clear();
}

void close_log() {
    outfile.close();
}
#endif
