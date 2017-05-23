#include "mp3vector.h"
#include "context.h"
#include "mp3_file.h"

#include <algorithm>
#include <iostream>

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

mp3vector mp3vector::filter(std::string property, std::string value) {
    //TODO: THIS SHIT ISNT WORKING VLAD PLES FIX
    context::get()->log() << "FUCK" << std::endl;
    mp3vector temp;

    for (auto song : *this) {
        if (mp3_file(song).get_property(property) == value) {
            temp.push_back(song);
        }
    }
    return temp;
}

std::set<std::string> mp3vector::get_all(std::string property) {
    /* context::get()->log() << "get_all" << std::endl; */
    std::set<std::string> tmp;
    for (auto i : *this) {
        /* context::get()->log() << mp3_file(i).get_property(property) << std::endl; */
        tmp.insert(mp3_file(i).get_property(property));
    }
    for (auto i : tmp) {
        /* context::get()->log() << i << std::endl; */
    }
    return tmp;
}
