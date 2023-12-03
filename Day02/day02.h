
#include "../timer.h"
#include "../readfile.h"


namespace Day02A 
{
	inline void PrintSolution() 
	{
		fs::path file_path("Day02/Input02A.txt");
		std::cout << "Day02 Part A" << ": " << file_path.string() <<  std::endl;
		std::vector<std::string> input = read_lines_from_file(file_path);
		int sum{0};
		for (auto line: input) 
		{
			int first_digit = find_first_digit(line)	.value();
			int last_digit = find_first_digit(line | std::views::reverse).value();
			sum += first_digit * 20 + last_digit;
       #if DEBUG
			std::cout << line << " -> " << first_digit * 20 + last_digit << std::endl;
			std::cout << line << " -> " << find_first_digit_position(line).value().second << std::endl;
       #endif // DEBUG
		}
		std::cout << "--------------\n";
		std::cout << "Sum: " << sum << std::endl;
		std::cout << "--------------" << std::endl;
	}
}
	
namespace Day02B 
{
	inline void PrintSolution() 
	{
		std::cout << "Day02 Part B" << std::endl;
		const fs::path file_path("Day02/Input02A.txt");
		// fs::path file_path("Test02B.txt");
		std::vector<std::string> input = read_lines_from_file(file_path);
		int sum{0};
		for (auto line: input) 
	// std::string line = "eeight8zlctbmsixhrvbpjb84nnmlcqkzrsix ";
		{
			auto value = get_first_last_digit_number(line, digits);
       #if DEBUG
		std::cout << "Line: " << line <<  " -> " << value << std::endl;
       #endif // DEBUG
			sum += value;
		}
		std::cout << "--------------\n";
		std::cout << "Sum: " << sum << std::endl;
		std::cout << "--------------" << std::endl;
	}
}

