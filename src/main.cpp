#include "mp3fs.h"

#include <unistd.h>
#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[]) {
    if ((getuid() == 0) || (geteuid() == 0)) {
        fprintf(stderr, "Running BBFS as root opens unnacceptable security holes\n");
        return 1;
    }

    if ((argc < 3) || (argv[argc - 2][0] == '-') || (argv[argc - 1][0] == '-')) {
        std::cout << "usage: main [FUSE and mount options] rootDir mountPoint\n";
    }

    char* path = realpath(argv[argc - 2], nullptr);

    argv[argc - 2] = argv[argc - 1];
    argv[argc - 1] = nullptr;
    argc--;

    /* init_operations(); */
    /* fuse_main(argc, argv, &bb_oper, new state(path, log_open())); */
    return mp3fs::main(argc, argv, path);
}
