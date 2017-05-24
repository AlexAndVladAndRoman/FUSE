#ifndef MP3VECTOR_H
#define MP3VECTOR_H

#include <string>
#include <vector>
#include <set>

class mp3vector : public std::vector<std::string> {
public:
    mp3vector filter_artist(std::string artist);
    mp3vector filter_album(std::string album);

    mp3vector filter(std::string property, std::string value);

    std::set<std::string> get_all(std::string property);
};

#endif /* MP3VECTOR_H */
