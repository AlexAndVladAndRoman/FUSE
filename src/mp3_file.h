#ifndef mp3_file_h
#define mp3_file_h
#include <string>

const int mp3TagSize = 128;

class mp3_file {
private:
    struct TAGdata {
        char tag[3];
        char title[30];
        char artist[30];
        char album[30];
        char year[4];
        char comment[30];
        int genre;
    };
    
    std::string artist, album, year, tag, comment, title;
    std::string path;
    int genre;
    
public:
    mp3_file(std::string path);
    
    std::string get_artist();
    std::string get_album();
    std::string get_year();
    std::string get_tag();
    std::string get_comment();
    std::string get_title();
    std::string get_genre();

    std::string get_property(std::string property);
};

#endif /* mp3_file_h */
