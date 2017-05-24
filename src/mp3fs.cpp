#include "mp3fs.h"
#include "context.h"

/* #include "bbfs.h" */

#include <algorithm>
#include <cstring>
#include <iostream>

int mp3fs::getattr(const char *path, struct stat *statbuf) {
    context::log() << "getattr path =\"" << path << "\"" << std::endl;

    if (std::string(path) == "/") {
        statbuf->st_mode = S_IFDIR | 0644;
        statbuf->st_nlink = 1;
        auto files = context::get()->files();
        for (auto file : files) {
            statbuf->st_nlink++;
        }
    } else if (std::count(path, path + strlen(path), '/') == 1) {
        statbuf->st_mode = S_IFDIR | 0644;
        statbuf->st_nlink = 1;
        auto files = context::get()->files().filter("artist", std::string(path).substr(1, strlen(path) - 1));

        context::log() << "FICK " << files.size() << std::endl;

        for (auto file : files) {
            context::log() << "FICK " << file << std::endl;
            statbuf->st_nlink++;
        }
    } else {
        if (std::count(path, path + strlen(path), '/') == 2) {
            statbuf->st_mode = S_IFLNK | 0644;
            statbuf->st_nlink = 1;
            context::get()->log() << "  path// -> " << path << std::endl;
            auto files = context::get()->files().filter("album", std::string(path).substr(strrchr(path, '/') - path + 1, strlen(path) - 1));
            
            for (auto file : files) {
                context::log() << "FOCK " << file << std::endl;
                statbuf->st_nlink++;
            }
        }
    }

    return 0;
}

int mp3fs::readlink(const char *path, char *link, size_t size) {
    context::log() << "readlink path =\"" << path << "\"" << std::endl;

    auto path_str = std::string(path);
    auto name = path_str.substr(path_str.find_last_of("/") + 1);

    auto files = context::get()->files();
    if (name == ".hidden") {
        link = ".hidden";
        return 0;
    }
    ::strcpy(link, (*std::find_if(files.begin(), files.end(), [name](std::string file) { return std::string::npos != file.find(name); })).c_str());
    context::log() << "link = " << link << std::endl << std::endl;

    return 0;
}

int mp3fs::readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    context::log() << "readdir path =\"" << path << "\"" << std::endl;

    filler(buf, ".", nullptr, 0);
    filler(buf, "..", nullptr, 0);

    if (std::string(path) == "/") {
        /* auto files = context::get()->files(); */
        /* for (auto file : files) { */
        /* filler(buf, file.substr(file.find_last_of("/") + 1).c_str(), nullptr, 0); */
        /* } */
        auto artists = context::get()->files().get_all("artist");
        for (auto artist : artists) {
            context::get()->log() << "  " << artist << std::endl;
            filler(buf, artist.c_str(), nullptr, 0);
        }
    } else {
        if (std::count(path, path + strlen(path), '/') == 1) {
            std::string name = std::string(path).substr(1, strlen(path) - 1);
            context::get()->log() << "  name -> " << name << std::endl;
            auto files = context::get()->files().filter("artist", name);
            for (auto file : files) {
                filler(buf, file.substr(file.find_last_of("/") + 1).c_str(), nullptr, 0);
            }
        }
        else {
            context::get()->log() << "  path// -> " << path << std::endl;
            std::string name = std::string(path).substr(strrchr(path, '/') - path + 1, strlen(path) - 1);
            context::get()->log() << "  name -> " << name << std::endl;
            auto files = context::get()->files().filter("album", name);
            for (auto file : files) {
                filler(buf, file.substr(file.find_last_of("/") + 1).c_str(), nullptr, 0);
            }
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
