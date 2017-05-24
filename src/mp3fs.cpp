#include "mp3fs.h"
#include "context.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <dirent.h>

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

        for (auto file : files) {
            statbuf->st_nlink++;
        }
    } else {
        if (std::count(path, path + strlen(path), '/') == 2) {
            statbuf->st_mode = S_IFDIR | 0644;
            statbuf->st_nlink = 1;
            context::get()->log() << "  path// -> " << path << std::endl;
            auto files = context::get()->files().filter("album", std::string(path).substr(strrchr(path, '/') - path + 1, strlen(path) - 1));
            
            for (auto file : files) {
                statbuf->st_nlink++;
            }
        }
        else {
            statbuf->st_mode = S_IFLNK | 0644;
            statbuf->st_nlink = 1;
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
    ::strcpy(link, (*std::find_if(files.begin() , files.end(), [name](std::string file) { return std::string::npos != file.find(name); })).c_str());
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
            auto albums = context::get()->files().filter("artist", name).get_all("album");
            for (auto album : albums) {
                context::get()->log() << "  albomchiki  -> " << album << std::endl;
                filler(buf, album.c_str(), nullptr, 0);
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
    

    operations->access = [](const char *path, int mask) {
        context::get()->log() << "access" << std::endl;
        access(path, mask);
        return 0;
    };
    
    operations->opendir = [](const char *path, struct fuse_file_info *fi){
        context::get()->log() << "opendir" << std::endl;
        DIR *dp;
        dp = opendir(path);
        fi->fh = (intptr_t)dp;
        return 0;
    };
    operations->releasedir = [](const char *path, struct fuse_file_info *fi){
        context::get()->log() << "releasedir" << std::endl;
      //  closedir((DIR *)(uintptr_t)fi->fh);
        return 0;
    };
    operations->statfs = [](const char *path, struct statvfs *statv){
        context::get()->log() << "statfs" << std::endl;
        statvfs(path, statv);
        return 0;
    };
    
    //releasedir opendir access statfs
    
    
    operations->mknod = [](const char *path, mode_t mode, dev_t dev){
        context::get()->log() << "mknod" << std::endl;
        return 0;
    };
    operations->mkdir = [](const char *path, mode_t mode){
        context::get()->log() << "mkdir" << std::endl;
        return 0;
    };
    operations->unlink = [](const char *path){
        context::get()->log() << "unlink" << std::endl;
        return 0;
    };
    operations->rmdir = [](const char *path){
        context::get()->log() << "rmdir" << std::endl;
        return 0;
    };
    operations->symlink = [](const char *path, const char *link){
        context::get()->log() << "symlink" << std::endl;
        return 0;
    };
    operations->rename = [](const char *path, const char *newpath){
        context::get()->log() << "rename" << std::endl;
        return 0;
    };
    operations->link = [](const char *path, const char *newpath){
        context::get()->log() << "link" << std::endl;
        return 0;
    };
    operations->chmod = [](const char *path, mode_t mode){
        context::get()->log() << "chmod" << std::endl;
        return 0;
    };
    operations->chown = [](const char *path, uid_t uid, gid_t gid){
        context::get()->log() << "chown" << std::endl;
        return 0;
    };
    operations->truncate = [](const char *path, off_t newsize){
        context::get()->log() << "truncate" << std::endl;
        return 0;
    };
    operations->utime = [](const char *path, struct utimbuf *ubuf){
        context::get()->log() << "utime" << std::endl;
        return 0;
    };
    operations->open = [](const char *path, struct fuse_file_info *fi){
        context::get()->log() << "open" << std::endl;
        return 0;
    };
    operations->read = [](const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
        context::get()->log() << "read" << std::endl;
        return 0;
    };
    operations->write = [](const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
        context::get()->log() << "write" << std::endl;
        return 0;
    };
    operations->statfs = [](const char *path, struct statvfs *statv){
        context::get()->log() << "statfs" << std::endl;
        return 0;
    };
    operations->flush = [](const char *path, struct fuse_file_info *fi){
        context::get()->log() << "flush" << std::endl;
        return 0;
    };
    operations->release = [](const char *path, struct fuse_file_info *fi){
        context::get()->log() << "release" << std::endl;
        return 0;
    };
    operations->fsync = [](const char *path, int datasync, struct fuse_file_info *fi){
        context::get()->log() << "fsync" << std::endl;
        return 0;
    };

    return operations;
}
