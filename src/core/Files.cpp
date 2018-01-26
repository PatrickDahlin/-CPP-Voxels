#include "../headers/core/Files.hpp"
#include "../headers/core/Errors.hpp"
#include "../headers/core/Macros.hpp"

std::string read_file(const char* path)
{
	std::ifstream infile { path };
	std::string file_contents { std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>() };

    return file_contents;
}