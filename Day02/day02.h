#include <iostream>
#include <string>
#include <ranges>
#include <numeric>

#include "../timer.h"
#include "../readfile.h"

using GameIndex = int;

struct Draw 
{
	int red{0};
	int green{0};
	int blue{0};

	int count() 
	{
		return red + green + blue;
	}

	int power() 
	{
		return red * green * blue;
	}
};


inline std::ostream& operator<<(std::ostream& os, const Draw& draw) 
{
	os << "R: " << draw.red << ", ";
	os << "G: " << draw.green << ", ";
	os << "B: " << draw.blue ;
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const std::vector<Draw>& draws) 
{
	os << "Draws [" << draws.size() << "]=[";
	auto append = [&os](const Draw& draw) -> void 
	{
		os << ", (" << draw << ")";
	};
	os << "(" <<  draws[0] << ")";
	std::ranges::for_each(draws | std::views::drop(1), append);
	os << "]";
	return os;
}

struct Game {
	std::vector<Draw> draws{};
	GameIndex index;

	size_t draw_count()
	{
		return draws.size();
	}
};

inline std::ostream& operator<<(std::ostream& os, const Game& game) 
{
	os << "Game: " << game.index << ", ";
	os << game.draws;
	return os;
}

Draw parse_draw(std::istringstream& segment_stream) 
{
	int count;
	std::string color;
	Draw draw;
	while(segment_stream >> count >> color) 
	{
		if (color.find("green") != std::string::npos) 
		{
			draw.green += count;
		}
		else if (color.find("red") != std::string::npos) 
		{
			draw.red += count;
		}
		else if (color.find("blue") != std::string::npos) 
		{
			draw.blue += count;
		}
		else 
		{
			std::cerr << "not ";
		}
	}

	return draw;
}
 
std::vector<Draw> parse_draws(const std::string& str) 
{
	std::vector<Draw> draws;
	std::string segment;
	std::istringstream str_stream(str);
	while (std::getline(str_stream, segment, ';'))
	{
		std::istringstream segment_stream{segment};
		draws.push_back(parse_draw(segment_stream));
	}
	return draws;
}

std::pair<GameIndex, Game> parse_line(const std::string& line) 
{
	std::istringstream line_stream{line};
	std::string game_hdr, draws_str;
	std::getline(line_stream, game_hdr, ':');
	std::getline(line_stream, draws_str);

	// Game Index
	std::string game_str;
	GameIndex index;
	std::istringstream game_hrd_stream(game_hdr);
	game_hrd_stream >> game_str >> index;

	// Get draws
	const std::vector<Draw> draws = parse_draws(draws_str);

	const Game game{.draws=draws, .index=index};
	
	return std::make_pair(index, game);
}

const Draw TotalDraw{.red=12, .green=13, .blue=14};


bool is_valid(const Draw& sub_draw, const Draw& total_draw) 
{
	return (sub_draw.red <= total_draw.red)
	&& (sub_draw.green <= total_draw.green)
	&& (sub_draw.blue <= total_draw.blue);
};

bool is_valid(const Game& game, const Draw& total_draw)
{
	auto valid = [&total_draw](const Draw& draw) -> bool
	{
		return is_valid(draw, total_draw);
	};
	
	return std::ranges::all_of(game.draws, valid );
}

auto max_draw = [](const Draw& left, const Draw& right) -> Draw {
	return {.red=std::max(left.red, right.red),
	.green=std::max(left.green, right.green),
	.blue=std::max(left.blue, right.blue)};
};

Draw min_total_draw(const std::vector<Draw>& draws)
{
	return std::accumulate( draws.begin(), draws.end(), Draw(), max_draw);
};



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
			const auto& [index, game]  = parse_line(line); 

			if ( is_valid(game, TotalDraw) )
			{
				sum +=index ;
			}
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
		{
			const auto& [index, game]  = parse_line(line); 
			Draw min_draw = min_total_draw(game.draws);	
	
       #if DEBUG
			std::cout << line << " -> " <<  index << ": " << min_draw << std::endl;
       #endif // DEBUG
			sum += min_draw.power();

		}
		std::cout << "--------------\n";
		std::cout << "Sum: " << sum << std::endl;
		std::cout << "--------------" << std::endl;
	}
}

