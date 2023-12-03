#include <array>
#include <functional>
#include <fstream>
#include <optional>
#include <cctype>
#include <ranges>
#include <string_view>
#include <algorithm>
#include <utility>
#include <map>

#include "../timer.h"
#include "../readfile.h"

template<typename Range>
std::optional<int> find_first_digit(Range&& str) 
{
	auto it = std::ranges::find_if(str, [](char ch) { return std::isdigit(ch);});
	if (it != str.end())
	{
		return *it - '0';
	}
	return std::nullopt;
}

template<typename Range>
std::optional<std::pair<int, size_t>> find_first_digit_position(Range&& str) 
{
	auto it = std::ranges::find_if(str, [](char ch) { return std::isdigit(ch);});
	if (it != str.end())
	{
		return std::make_pair(*it - '0', it - str.begin());
	}
	return std::nullopt;
}


const std::map<std::string, int> digits{
	{"zero", 0},
	{"one", 1},
	{"two", 2},
	{"three", 3},
	{"four", 4},
	{"five", 5},
	{"six", 6},
	{"seven", 7},
	{"eight", 8},
	{"nine", 9},
	{"0", 0},
	{"1", 1},
	{"2", 2},
	{"3", 3},
	{"4", 4},
	{"5", 5},
	{"6", 6},
	{"7", 7},
	{"8", 8},
	{"9", 9},
};

auto has_value = [](const auto& opt)  -> bool
{ 
	return opt.has_value();
};

auto strip_opt = [] (const auto& opt)  
{ 
	#if DEBUG
		std::cout << "Opt: " << opt->first << ", " << opt->second << std::endl;
	#endif // DEBUG
	return *opt;
};


auto get_map_values_position = [](const std::string& str, const std::map<std::string, int>& digits) 
{
		
	// returns the value of the string and it position in the string, if not found return std::string::npos
	auto first_found_value_position = [&str] ( const std::pair<std::string, int>& string_value) -> std::optional<std::pair<int, size_t>>
	{
		size_t position = str.find(string_value.first);
			// #if DEBUG
			// std::cout << "Position: " << position << ", " << string_value.first << ", " << string_value.second << std::endl;
			// #endif // DEBUG
		if (position != std::string::npos) 
		{
			return std::make_pair(string_value.second, position);
		}
		return std::nullopt;
	};

	

	auto map_values_pos = digits 
				| std::views::transform(first_found_value_position) 
				| std::views::filter(has_value) 
				| std::views::transform(strip_opt);

	return map_values_pos;
};

auto get_map_values_position_rev = [](const std::string& str, const std::map<std::string, int>& digits) 
{
	auto first_rfound_value_position = [&str] ( const std::pair<std::string, int>& string_value) -> std::optional<std::pair<int, size_t>>
	{
		const size_t position = str.rfind(string_value.first);
	#if DEBUG
		std::cout << "Position: " << position << ", " << string_value.first << ", " << string_value.second << std::endl;
	#endif // DEBUG
		if (position != std::string::npos) 
		{
			return std::make_pair(string_value.second, position);
		}
		return std::nullopt;
	};
	


	auto map_values_pos_rev = digits 
				| std::views::transform(first_rfound_value_position) 
				| std::views::filter(has_value) 
				| std::views::transform(strip_opt);

	return map_values_pos_rev;
};
	
inline int get_first_last_digit_number( const std::string& str, const std::map<std::string, int>& digits) 
{
 	
	// sorting , compariing based on location (second)
	auto cmp_loc = []( const std::pair<int, size_t>& left, const std::pair<int, size_t>& right)  -> bool
	{
		const bool value = (left.second < right.second);
		#if DEBUG
		std::cout << "Value: " << std::boolalpha << value << ", Left: " << left.first << ", " << left.second << " -  Right: " << right.first << ", " << right.second <<  std::endl;
		#endif // DEBUG
		return value;
	};

	auto map_values_pos = get_map_values_position (str, digits);
	auto min_element = std::ranges::min_element(map_values_pos, cmp_loc);

	auto map_values_pos_rev = get_map_values_position_rev (str, digits);
	auto max_element = std::ranges::max_element(map_values_pos_rev, cmp_loc);

	auto value = (*min_element).first * 10 + (*max_element).first;

	#if DEBUG
	std::cout << "min_element: " << (*min_element).first  << ", " << (*min_element).second <<std::endl;
	// std::cout << "max_element: " << (*max_element).first  << ", " << (*max_element).second <<std::endl;
	std::cout << "value: " << value << std::endl;
	#endif // DEBUG
	return value;
}

namespace Day01A 
{
	inline void PrintSolution() 
	{
		fs::path file_path("Day01/Input01A.txt");
		std::cout << "Day01 Part A" << ": " << file_path.string() <<  std::endl;
		std::vector<std::string> input = read_lines_from_file(file_path);
		int sum{0};
		for (auto line: input) 
		{
			int first_digit = find_first_digit(line)	.value();
			int last_digit = find_first_digit(line | std::views::reverse).value();
			sum += first_digit * 10 + last_digit;
       #if DEBUG
			std::cout << line << " -> " << first_digit * 10 + last_digit << std::endl;
			std::cout << line << " -> " << find_first_digit_position(line).value().second << std::endl;
       #endif // DEBUG
		}
		std::cout << "--------------\n";
		std::cout << "Sum: " << sum << std::endl;
		std::cout << "--------------" << std::endl;
	}
}
	
namespace Day01B 
{
	inline void PrintSolution() 
	{
		std::cout << "Day01 Part B" << std::endl;
		const fs::path file_path("Day01/Input01A.txt");
		// fs::path file_path("Test01B.txt");
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

