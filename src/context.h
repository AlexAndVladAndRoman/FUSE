#ifndef CONTEXT_H
#define CONTEXT_H

#include <fstream>

constexpr char LOG_FILE[12] = "mp3fs.log";

struct context {
    char* rootdir;
    std::ofstream log_stream;

    context(char* rootdir) : rootdir(rootdir), log_stream(LOG_FILE) {
    }

    static context* get();

    static std::ofstream& log();
};

#endif  // CONTEXT_H
