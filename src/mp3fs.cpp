#include "mp3fs.h"
#include "context.h"

/* #include "bbfs.h" */

#include <iostream>
#include <cstring>

int mp3fs::getattr(const char *path, struct stat *statbuf) {
    context::log() << "getattr path =\"" << path << "\"" << std::endl;

    if (std::string(path) == "/") {
        statbuf->st_mode = S_IFDIR | 644;
        statbuf->st_nlink = 1;
        auto files = context::get()->files();
        for (auto file : files) {
            statbuf->st_nlink++;
        }
    } else {
        statbuf->st_mode = S_IFLNK | 644;
        statbuf->st_nlink = 1;
    }

    return 0;
}

int mp3fs::readlink(const char *path, char *link, size_t size) {
    context::log() << "readlink path =\"" << path << "\"" << std::endl;

    ::strcpy(link, (std::string(context::get()->get_root()) + "/test.txt").c_str());

    return 0;
}

int mp3fs::readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    context::log() << "readdir path =\"" << path << "\"" << std::endl;

    if (std::string(path) == "/") {
        auto files = context::get()->files();
        for (auto file : files) {
            filler(buf, file.c_str(), nullptr, 0);
        }
    }

    return 0;
}

int mp3fs::main(int argc, char *argv[], char *path) {
    std::cerr << "About to call fuse_main" << std::endl;
    int result = fuse_main(argc, argv, get_operations(), new context(path));
    std::cerr << "fuse_main returned = " << result << std::endl;
    return result;
}

fuse_operations *mp3fs::get_operations() {
    fuse_operations *operations = new fuse_operations();
    operations->getattr = getattr;
    operations->readlink = readlink;
    operations->readdir = readdir;

    /* operations->release = [](const char *, fuse_file_info *) { return 0; }; */

    /* operations->fsync = [](const char *, int, fuse_file_info *) { return 0; }; */

    /* operations->opendir = [](const char *, fuse_file_info *) { return 0; }; */

    /* operations->statfs = [](const char *path, struct statvfs* b) { */
    /* debug::log("fsync"); */
    /* return 0; */
    /* }; */

    return operations;
}
