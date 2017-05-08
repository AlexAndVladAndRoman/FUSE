#include "context.h"

#include <dirent.h>
#include <fuse.h>

context* context::get() {
    return ((struct context*)fuse_get_context()->private_data);
}

std::ofstream& context::log() {
    return get()->log_stream;
}

char* context::get_root() {
    return rootdir;
}

mp3vector context::files() {
    return files(rootdir);
}

mp3vector context::files(const char* path) {
    mp3vector result;
    DIR* dir;
    struct dirent* entry;
    if ((dir = opendir(path)) != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == ::DT_REG) {
                result.emplace_back(entry->d_name);
            } else if (entry->d_type == ::DT_DIR) {
                if (entry->d_name[0] != '.') {
                    auto temp = files((std::string(path) + "/" + entry->d_name).c_str());
                    result.insert(result.end(), temp.begin(), temp.end());
                }
            } else {
                log() << "Here some strange files: " << entry->d_name << std::endl;
            }
        }
        closedir(dir);
    } else {
        // TODO: ERRORS
        log() << "ERROR CANT OPEN ROOTDIR" << std::endl;
    }
    return result;
}
