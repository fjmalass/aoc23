#include <locale>
#include <ranges>
#include <algorithm>
#include <iostream>
#include <vector>
#include <set>
#include <numeric>
#include <functional>

#include "../timer.h"
#include "../readfile.h"


constexpr char NULLCHAR = '\0';
constexpr char EMPTYCHAR = '.';


bool is_inside( int row, int col, int rows, int cols ) 
{
   return (row >= 0) && (row < rows) && (col >= 0) && (col < cols);
};


// Get the 8 neighbors (exclude center) within the range
template<typename Int>
std::vector<std::pair<Int, Int>> get_neighbors(Int row, Int col, Int rows, Int cols)
{
	std::vector<std::pair<Int, Int>> neighbors;
	neighbors.reserve(8);

	// std::cout << "GetNeighbors: " <<  row << ", " << col << " Size " << rows << ", " << cols << std::endl;

	auto is_center = [row, col] (int new_row, int new_col) -> bool
	{
		return static_cast<int> (row) == new_row && static_cast<int>(col) == new_col;
	};

	for ( int i = -1; i < 2; i++)
	{
		const int new_row = static_cast<int>(row) + i;
		for ( int j = -1; j < 2; j++)
		{
			const int new_col = static_cast<int>(col) + j;
			// std::cout << " .... GetNeighbors: " <<  new_row << ", " << new_col << " origin " << row << ", " << col << std::endl;
			if (is_inside(new_row, new_col, rows, cols) &&  !is_center(new_row, new_col))
			{
				neighbors.emplace_back(static_cast<Int>(new_row), static_cast<Int>(new_col));
			}
		}
	}

	neighbors.shrink_to_fit();
	// std::cout << "neighbors: " << neighbors.size() << std::endl;
	return neighbors;
};

struct Part
{
	int sn {0};
	char symbol{NULLCHAR}; 

	Part(int part_number) : sn{part_number}
	{
		#if DEBUG
		std::cout << "Creating:  SN" << part_number  << std::endl;
		#endif // DEBUG
	}

	Part(char ch) : symbol{ch}
	{
		#if DEBUG
		std::cout << "Creating: Symbol  " << ch << std::endl;
		#endif // DEBUG
	}


	bool is_part() const 
	{ 
		return sn > 0;
	};

	bool is_symbol() const 
	{ 
		return symbol != NULLCHAR;
	};

	bool is_empty() const 
	{
		return !(is_part() || is_symbol());
	}

	void add_digit(int digit) 
	{
		sn = sn * 10 + digit;

		#if DEBUG
		std::cout << "digit: " << digit << ", sn: " << sn << std::endl;
		#endif // DEBUG
	}
};


inline std::ostream& operator<<(std::ostream& os, const Part& part) 
{
	if (part.is_part())
	{
		os << "Part: " << part.sn;
	}
	else if (part.is_symbol())
	{
		os << "Symbol: " << part.symbol;
	}
	else {
		os << "EMPTY";
	}
	return os;
}

// PARSE GRID
std::vector<std::vector<std::shared_ptr<Part>>> parse_grid(const std::vector<std::string>& lines) 
{
	const size_t n_rows = lines.size();
	const size_t n_cols = lines[0].size();

	size_t n_parts { 0ull };
	size_t n_symbols { 0ull };


	std::vector<std::vector<std::shared_ptr<Part>>> grid(n_rows, std::vector<std::shared_ptr<Part>>(n_cols));
	std::cout << "Allocated Grid size: RxC: " << n_rows << "x" << n_cols << std::endl;

	for (auto row_idx : std::views::iota(0ull, n_rows)) 
	{
		const std::string& line = lines[row_idx];
      #if DEBUG
	       std::cout << row_idx << ": " << line << std::endl;
      #endif // DEBUG

		std::vector<std::shared_ptr<Part>>& row = grid[row_idx];
		for (auto col_idx : std::views::iota(0ull, n_cols)) 
		{
			const char ch = line[col_idx];
			if (ch == EMPTYCHAR) 
			{
				row[col_idx] = nullptr;
			}
			else if (std::isdigit(ch))
			{
				int digit = ch - '0';
			#if DEBUG
std::cout << "DIGIT " << digit << std::endl;
			#endif // DEBUG
				if ( ( col_idx == 0 ) 
				       || ( row[col_idx -1] == nullptr ) 
				       || ( row[col_idx-1]->is_symbol() ) )
				{

					n_parts++;
					row[col_idx] = std::make_shared<Part>(digit);
			#if DEBUG
std::cout << "Createing shared pointer " << *row[col_idx] << std::endl;
			#endif // DEBUG
				}
				else 
				{
					row[col_idx] = row[col_idx -1];
					row[col_idx]->add_digit(digit);
			#if DEBUG
std::cout << "Adding to pointer " << *row[col_idx] << std::endl;
			#endif // DEBUG
				}
			}
			else 
			{
			#if DEBUG
std::cout << "Symbol " << ch << std::endl;
			#endif // DEBUG
				n_symbols++;
				row[col_idx] = std::make_shared<Part>(ch);
			}
#if DEBUG
if (row[col_idx]) std::cout << row_idx << "," << col_idx <<  ": " << ch << " -> " << *row[col_idx] << std::endl;
#endif // DEBUG
		}

	}

	std::cout << "Parts: " << n_parts << std::endl;
	std::cout << "Symbols: " << n_symbols << std::endl;
	return grid;
}

namespace Day03A 
{
	inline void PrintSolution() 
	{
		fs::path file_path("Day03/Input03A.txt");
		// fs::path file_path("Day03/Test03A.txt");
		std::cout << "Day03 Part A" << ": " << file_path.string() <<  std::endl;
		std::vector<std::string> input = read_lines_from_file(file_path);


		std::vector<std::vector<std::shared_ptr<Part>>> grid = parse_grid(input);
 	        const size_t n_rows = grid.size();
		const size_t n_cols = grid[0].size();

	// Select parts
		std::set<std::shared_ptr<Part>> selected_parts;
		for (size_t row_idx: std::views::iota(0ull, n_rows) )
		{
			const auto& row	= grid[row_idx];
			for (size_t col_idx: std::views::iota(0ull, n_cols) )
			{
				const auto& el = row[col_idx];
				if (el && el->is_symbol())
				{
					const auto neighbor_indices = get_neighbors(row_idx, col_idx, n_rows, n_cols);
				#if DEBUG
	  std::cout << "Element: " << row_idx << "x" << col_idx << ": " << *el <<  ", neighbors: " << neighbor_indices.size() << std::endl;
				#endif // DEBUG
					for (auto[nrow_idx, ncol_idx] : neighbor_indices)
					{
						const auto neighbor = grid[nrow_idx][ncol_idx];
						if (neighbor && neighbor->is_part())
						{
							selected_parts.insert(neighbor);
				#if DEBUG
				  std::cout << "Adding " << nrow_idx << "x" << ncol_idx << ": " << *neighbor <<  ", Selected : " << selected_parts.size() << std::endl;
				#endif // DEBUG
						}
					}
				}
			}
		}

		auto to_sn = [] (auto part) -> int { return part->sn;};
		int sum = std::ranges::fold_left( selected_parts | std::views::transform(to_sn), 0, std::plus<>());

		std::cout << "--------------\n";
		std::cout << "Number of Parts connected to Symbols: " << selected_parts.size() << std::endl;
		std::cout << "Sum: " << sum << std::endl;
		std::cout << "--------------" << std::endl;
	}
}
	
namespace Day03B 
{
	inline void PrintSolution() 
	{
		fs::path file_path("Day03/Input03A.txt");
		// fs::path file_path("Day03/Test03A.txt");
		std::cout << "Day03 Part B" << ": " << file_path.string() <<  std::endl;
		std::vector<std::string> input = read_lines_from_file(file_path);
		std::vector<std::vector<std::shared_ptr<Part>>> grid = parse_grid(input);

 	        const size_t n_rows = grid.size();
		const size_t n_cols = grid[0].size();

		// Select parts
		std::set<std::pair<std::shared_ptr<Part>, std::shared_ptr<Part>>> selected_parts;
		for (size_t row_idx: std::views::iota(0ull, n_rows) )
		{
			const auto& row	= grid[row_idx];
			for (size_t col_idx: std::views::iota(0ull, n_cols) )
			{
				std::set<std::shared_ptr<Part>> local_parts;
				const auto& el = row[col_idx];
				if (el && el->is_symbol())
				{
					const auto neighbor_indices = get_neighbors(row_idx, col_idx, n_rows, n_cols);
				#if DEBUG
				  std::cout << "Element: " << row_idx << "x" << col_idx << ": " << *el <<  ", neighbors: " << neighbor_indices.size() << std::endl;
				#endif // DEBUG
					for (auto[nrow_idx, ncol_idx] : neighbor_indices)
					{
						const auto neighbor = grid[nrow_idx][ncol_idx];
						if (neighbor && neighbor->is_part())
						{
							local_parts.insert(neighbor);
						}
					}
				}
				// Get pair
				if ( local_parts.size() == 2)
				{
					auto it = local_parts.begin();
					auto first = *it;
					++it; 
					auto second = *it;
					auto pair = std::make_pair<>(first, second);
					if (!selected_parts.contains(pair))
					{
						selected_parts.insert(pair); 
					}
					else {
						std::cout << "Already there" << *first << ", " << *second << std::endl;
					}
				}
			}
		}

		auto to_prod = [](auto pair) -> int { return pair.first->sn * pair.second->sn;};
		int sum = std::ranges::fold_left( selected_parts | std::views::transform(to_prod), 0, std::plus<>());

		std::cout << "--------------\n";
		std::cout << "Number of Pairs: " << selected_parts.size() << std::endl;
		std::cout << "Sum Product: " << sum << std::endl;
		std::cout << "--------------" << std::endl;
	}
}
