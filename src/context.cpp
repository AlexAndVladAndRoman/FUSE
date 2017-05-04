#include "context.h"

#include <fuse.h>

context* context::get() {
    return ((struct context*)fuse_get_context()->private_data);
}

std::ofstream& context::log() {
    return get()->log_stream;
}
