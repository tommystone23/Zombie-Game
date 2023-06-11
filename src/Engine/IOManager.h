#ifndef IOMANAGER_H
#define IOMANAGER_H
#include <vector>
#include <string>

class IOManager
{
public:
    static bool read_file_to_buffer(std::string file_path, std::vector<unsigned char> &buffer);
};

#endif