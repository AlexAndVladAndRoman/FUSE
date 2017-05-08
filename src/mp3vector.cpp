#include "mp3vector.h"
#include "mp3_file.h"

#include <algorithm>

mp3vector mp3vector::filter_album(std::string album) {
    mp3vector temp;
    std::copy_if(this->begin(), this->end(), temp.begin(), [album](std::string a) { return mp3_file(a).get_album() == album; });
    return temp;
}

mp3vector mp3vector::filter_artist(std::string artist) {
    mp3vector temp;
    std::copy_if(this->begin(), this->end(), temp.begin(), [artist](std::string a) { return mp3_file(a).get_artist() == artist; });
    return temp;
}
