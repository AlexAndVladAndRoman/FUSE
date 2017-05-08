#ifndef MP3VECTOR_H
#define MP3VECTOR_H

#include <string>
#include <vector>

class mp3vector : public std::vector<std::string> {
public:
    mp3vector filter_artist(std::string artist);
    mp3vector filter_album(std::string album);
};

#endif /* end of include guard: MP3VECTOR_H */
