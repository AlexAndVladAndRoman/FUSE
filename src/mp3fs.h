#ifndef MP3FS_H
#define MP3FS_H

#define FUSE_USE_VERSION 26

#include <fuse.h>

class mp3fs {
    static int getattr(const char *path, struct stat *statbuf);
    static int readlink(const char *path, char *link, size_t size);
    static int readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi);

    static fuse_operations *get_operations();

public:
    static int main(int argc, char *argv[], char *path);
};

#endif  // MP3FS_H
