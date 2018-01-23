#include "../headers/core/Files.hpp"
#include "../headers/core/Errors.hpp"
#include "../headers/core/Macros.hpp"

std::string read_file(const char* path)
{
    std::ifstream file(path);

	if(!file.good())
	{
		error("File couldn't be found!");
		cout("File: "); coutln(path);
		file.close();
		return "";
	}	

	std::string output = "";

    if(file.is_open())
    {
        std::string line;
        while(getline(file, line))
        {
            output += line;
			output += "\n";
        }
    }

	file.close();

    return output;
}