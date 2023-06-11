#include "IOManager.h"
#include <fstream>

bool read_file_to_buffer(std::string file_path, std::vector<unsigned char> &buffer)
{
    std::ifstream file(file_path, std::ios::binary);
    if(file.fail())
    {
        perror(file_path.c_str());
        return false;
    }

    // Seek to the end
    file.seekg(0, std::ios::end);
     
    // Get the file size
    int file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Reduce file size by any header bytes that might be present
    file_size -= file.tellg();

    buffer.resize(file_size);

    file.read((char*)&(buffer[0]), file_size);

    file.close();
    return true;
}