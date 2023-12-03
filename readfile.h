#pragma once 

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <locale>
namespace fs = std::filesystem;

inline std::vector<std::string> read_lines_from_file( const fs::path& file_path )
{
	if (!fs::exists(file_path)) 
	{
		std::cerr << "File '" << file_path << "' Does not exist\n";
		return {};
	}
	std::ifstream file{file_path};
	if (!file.is_open()) 
	{
		std::cerr << "File '" << file_path << "' Failed to open\n";
		file.close();
		return {};
	}
	std::vector<std::string> lines;
	std::string line;
	while ( std::getline(file, line) ) 
	{
		lines.push_back(line);
		#if DEBUG
		// std::cout << line << std::endl;
		#endif //DEBUG
	}
	std::cout << "Read " << lines.size() << " Lines" << std::endl;
	
	file.close();

	return lines;
}
