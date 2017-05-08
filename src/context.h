#ifndef CONTEXT_H
#define CONTEXT_H

#include "mp3vector.h"

#include <fstream>
#include <string>
#include <vector>

constexpr char LOG_FILE[12] = "mp3fs.log";

struct context {
    context(char* rootdir) : rootdir(rootdir), log_stream(LOG_FILE) {
    }

    static context* get();

    static std::ofstream& log();

    mp3vector files();

    char* get_root();

private:
    char* rootdir;
    std::ofstream log_stream;

    mp3vector files(const char* path);
};

#endif  // CONTEXT_H
