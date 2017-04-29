#ifndef STATE_H
#define STATE_H

#include <cstdio>
#include <fuse.h>

struct state {
    char* rootdir;
    FILE* logfile;

    state(char* rootdir, FILE* logfile) : rootdir(rootdir), logfile(logfile) {
    }

    static state* get() {
        return ((struct state*)fuse_get_context()->private_data);
    }
};

#endif  // STATE_H