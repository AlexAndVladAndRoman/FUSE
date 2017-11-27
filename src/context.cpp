#include "context.h"

#include <dirent.h>
#include <fuse.h>
#include <algorithm>
#include <cstring>

context::context(char* rootdir) : rootdir(rootdir), log_stream(LOG_FILE) {
}

context* context::get() {
    return ((struct context*)fuse_get_context()->private_data);
}

std::ofstream& context::log() {
    return get()->log_stream;
}

char* context::get_root() {
    return rootdir;
}

//lul

mp3vector context::files() {
    mp3vector temp = files(rootdir);
    std::transform(temp.begin(), temp.end(), temp.begin(), [this](std::string file) { return std::string(rootdir) + "/" + file; });
    return temp;
}

mp3vector context::files(const char* path) {
    mp3vector result;
    DIR* dir;
    struct dirent* entry;
    if ((dir = opendir(path)) != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_REG) {
                if (strstr(entry->d_name, ".mp3")) {
                    result.emplace_back(entry->d_name);
                }
            } else if (entry->d_type == DT_DIR) {
                if (entry->d_name[0] != '.') {
                    auto temp = files((std::string(path) + "/" + entry->d_name).c_str());
                    std::transform(temp.begin(), temp.end(), temp.begin(), [entry](std::string file) { return std::string(entry->d_name) + "/" + file; });
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

    /* log() << "files: " << std::endl; */
    /* for (auto i : result) { */
        /* log() << i << std::endl; */
    /* } */
    /* log() << std::endl; */

    return result;
}
