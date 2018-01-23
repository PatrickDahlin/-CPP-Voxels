#include "../headers/core/Files.hpp"

std::string read_file(const char* path)
{
    std::ifstream file(path);

	std::string output = "";

    if(file.is_open())
    {
        std::string line;
        while(getline(file, line))
        {
            output += line;
        }
    }

	file.close();

    return output;
}