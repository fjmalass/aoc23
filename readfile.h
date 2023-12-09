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
	std::vector<std::string> lines;
	if (!fs::exists(file_path)) 
	{
		std::cerr << "File '" << file_path << "' Does not exist\n";
		return lines;
	}

	std::ifstream file_stream{file_path};
	if (!file_stream.is_open()) 
	{
		std::cerr << "File '" << file_path << "' Failed to open\n";
		file_stream.close();
		return lines;
	}

	std::string line;
	while ( std::getline(file_stream, line) ) 
	{
		lines.push_back(line);
		#if DEBUG
		std::cout << line << std::endl;
		#endif //DEBUG
	}
	std::cout << "File "  << file_path << " : " << lines.size() << " Lines" << std::endl;
	
	file_stream.close();

	return lines;
}
