#include "mp3_file.h"
#include "context.h"

#include <fstream>
#include <iostream>

mp3_file::mp3_file(std::string path) : path(path) {
    TAGdata tagStruct;
    std::ifstream mp3File;
    mp3File.open(path);
    if (!mp3File) {
        std::cerr << "Can't open " << path << std::endl;
        return;
    }
    mp3File.seekg(-(mp3TagSize), std::ios::end);
    if (mp3File.fail()) {
        std::cerr << "Can't seek" << std::endl;
        return;
    }
    mp3File.read(reinterpret_cast<char *>(&tagStruct), mp3TagSize);
    if (mp3File.fail()) {
        std::cerr << "Can't read" << std::endl;
        return;
    }

    mp3File.close();
    artist = tagStruct.artist;
    genre = tagStruct.genre;
    album = tagStruct.album;
    year = tagStruct.year;
    tag = tagStruct.tag;
    comment = tagStruct.comment;
    title = tagStruct.title;

    /* context::get()->log() << "file = \"" << path << "\" " << " album = \"" << album << "\"" << " artist = \"" << artist << "\"" << std::endl; */
}

std::string mp3_file::get_artist() {
    return artist;
}

std::string mp3_file::get_album() {
    /* context::get()->log() << "file = \"" << path << "\" " << "album = \"" << album << "\"" << std::endl; */
    return album;
}

std::string mp3_file::get_year() {
    return year;
}

std::string mp3_file::get_tag() {
    return tag;
}

std::string mp3_file::get_comment() {
    return comment;
}

std::string mp3_file::get_title() {
    return title;
}

std::string mp3_file::get_genre() {
    switch (genre) {
        case 0:
            return "Blues";
            break;
        case 1:
            return "Classic Rock";
            break;
        // TODO add more genres
        /*  0.Blues
         1.Classic Rock
         2.Country
         3.Dance
         4.Disco
         5.Funk
         6.Grunge
         7.Hip - Hop
         8.Jazz
         9.Metal
         10.New Age
         11.Oldies
         12.Other
         13.Pop
         14.R&B
         15.Rap
         16.Reggae
         17.Rock
         18.Techno
         19.Industrial
         20.Alternative
         21.Ska
         22.Death Metal
         23.Pranks
         24.Soundtrack
         25.Euro - Techno
         26.Ambient
         27.Trip - Hop
         28.Vocal
         29.Jazz + Funk
         30.Fusion
         31.Trance
         32.Classical
         33.Instrumental
         34.Acid
         35.House
         36.Game
         37.Sound Clip
         38.Gospel
         39.Noise
         40.AlternRock
         41.Bass
         42.Soul
         43.Punk
         44.Space
         45.Meditative
         46.Instrumental Pop
         47.Instrumental Rock
         48.Ethnic
         49.Gothic
         50.Darkwave
         51.Techno - Industrial
         52.Electronic
         53.Pop - Folk
         54.Eurodance
         55.Dream
         56.Southern Rock
         57.Comedy
         58.Cult
         59.Gangsta
         60.Top 40
         61.Christian Rap
         62.Pop / Funk
         63.Jungle
         64.Native American
         65.Cabaret
         66.New Wave
         67.Psychadelic
         68.Rave
         69.Showtunes
         70.Trailer
         71.Lo - Fi
         72.Tribal
         73.Acid Punk
         74.Acid Jazz
         75.Polka
         76.Retro
         77.Musical
         78.Rock & Roll
         79.Hard Rock*/
        default:
            return "Ethnic";
            break;
    }
}

std::string mp3_file::get_property(std::string property) {
    if (property == "artist") {
        return get_artist();
    } else if (property == "album") {
        return get_album();
    } else if (property == "title") {
        return get_title();
    } else if (property == "year") {
        return get_year();
    } else if (property == "tag") {
        return get_tag();
    } else if (property == "comment") {
        return get_comment();
    } else if (property == "genre") {
        return get_genre();
    } else {
        throw "happens";  
    }
}
