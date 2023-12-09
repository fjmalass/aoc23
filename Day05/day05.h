#include <locale>
#include <ranges>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <numeric>
#include <functional>
#include <map>

#include "../timer.h"
#include "../readfile.h"


constexpr char NULLCHAR = '\0';
constexpr char EMPTYCHAR = '.';

// Should implement a depth first search

using CardIndex = int;


template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& array)  
{
	os << "[" << array.size() << "]=";
	os << "[";
	os << array[0];
	for (int i : std::views::iota(1, static_cast<int>(array.size()))) 
	{
		os << ", " << array[i]; 
	}
	os << "]";
	return os;
}


struct SeedGroup 
{
	int start {-1};
	int range {-1};
};

std::ostream& operator<<(std::ostream& os, const SeedGroup& group)
{
	os << "Seed: [";
	os << "Start: " << group.start << ", ";
	os << "Range: " << group.range << "]";

	return os;
};

	// could make it a string_view
std::vector<SeedGroup> parse_seeds(const std::string& line)
{
	std::istringstream line_stream{line};
	std::string seed_hdr, seeds_str;
	std::getline(line_stream, seed_hdr, ':');
	std::getline(line_stream, seeds_str);

	std::istringstream seeds_stream{seeds_str};
	int start, range;
	std::vector<SeedGroup> seed_groups{};
	while ( seeds_stream >> start >> range ) 
	{
		seed_groups.emplace_back(SeedGroup{.start=start, .range=range});
	}

	return seed_groups;
}


struct SourceTargetItem 
{
	int source{-1};
	int target{-1};
	int offset{-1};
};

std::ostream& operator<<(std::ostream& os, const SourceTargetItem& item)
{
	os << "Item: [";
	os << "Source: " << item.source << ", ";
	os << "Target: " << item.target << "]";
	os << "Offset: " << item.offset << "]";

	return os;
};

std::istream& hyphen(std::istream& is) 
{
	if ((is >> std::ws).peek() == '-') 
		is.ignore();
	return is;
}


#if false
std::pair<CardIndex, Card> parse_line( const std::string& line)
{
	std::istringstream line_stream{line};
	std::string card_hdr, card_str;
	std::getline(line_stream, card_hdr, ':');
	std::getline(line_stream, card_str);

	// Card Index
	std::string card_name_str;
	CardIndex index;
	std::istringstream card_hdr_stream(card_hdr);
	card_hdr_stream >> card_name_str >> index;

	// Get Card
	std::string winners_str;
	std::vector<int> winners;
	std::string drawns_str;
	std::vector<int> drawns;
	std::istringstream card_stream{card_str};
	std::getline(card_stream, winners_str, '|');
	std::getline(card_stream, drawns_str);

	std::istringstream winners_stream{winners_str};
	int value;
	while ( winners_stream >> value ) 
	{
		winners.push_back(value);		
	}

	std::istringstream drawns_stream{drawns_str};
	while ( drawns_stream >> value ) 
	{
		drawns.push_back(value);		
	}

	Card card{ .winners=winners, .drawns=drawns, .card_index = index};

	#if DEBUG
	std::cout << index << " -> " << card << " #Winning: " << card.n_winning() << ", Value: " << card.winning_value() << std::endl;
	#endif // DEBUG


	return {index, card};
}

std::map<CardIndex, Card> parse_lines(const std::vector<std::string>& lines)
{
	// return lines 
	// | std::views::transform(parse_line) 
	// | std::ranges::to<std::map<CardIndex, Card>>();
	std::map<CardIndex, Card> cards;
	 for (const auto& pair : lines | std::views::transform(parse_line) )
	{
		cards.insert(pair);
	}
	return cards;
}
#endif


namespace Day05A 
{
	inline void PrintSolution() 
	{
		fs::path file_path("Day05/Input05A.txt");
		// fs::path file_path("Day05/Test05A.txt");
		std::cout << "Day05 Part A" << ": " << file_path.string() <<  std::endl;
		std::vector<std::string> input = read_lines_from_file(file_path);
		
	// Get the card
		std::vector<SeedGroup> seed_groups = parse_seeds(input[0]);
	#if DEBUG
		std::cout << "seeds" << seed_groups << std::endl;
	#endif // DEBUG

	bool new_map {true};
		for (const std::string& line : input | std::views::drop(1) ) 
		{

	       #if DEBUG
		       std::cout << "line : " << line << std::endl;
	       #endif //DEBUG
			if (line.empty())
			{
				new_map = true;
				continue;
			}
			std::istringstream iss{line};
			if (new_map) {

				std::string sourcetotarget;
				if ( iss >> sourcetotarget)
				{
					std::cout << "sourcetotarget : " << sourcetotarget << std::endl;
					std::istringstream s2t_str{sourcetotarget};

					std::string source, target;
					std::getline( s2t_str, source, '-');
					std::getline( s2t_str, target, '-'); // skip	
					std::getline( s2t_str, target, '-');
				#if DEBUG
			       std::cout << "source : " << source << std::endl;
			       std::cout << "target : " << target << std::endl;
				#endif // DEBUG
					new_map = false;
				}
			}
		       else {
			}

       #if DEGUG
	       std::cout << source << ", " << target << std::endl;
       #endif //DEGUG


		}



		auto to_winning = [] (auto& pair ) -> int { return pair.second.winning_value();};
		// int sum = std::ranges::fold_left( cards | std::views::transform(to_winning), 0, std::plus<>());
		//
		std::cout << "--------------\n";
		// std::cout << "Sum: " << sum << std::endl;
		std::cout << "--------------" << std::endl;
	}
}
	
namespace Day05B 
{
	inline void PrintSolution() 
	{
		fs::path file_path("Day05/Input05A.txt");
		// fs::path file_path("Day05/Test05A.txt");
		std::cout << "Day05 Part B" << ": " << file_path.string() <<  std::endl;
		std::vector<std::string> input = read_lines_from_file(file_path);

	


		// int sum = std::ranges::fold_left( std::views::values(won_cards), 0, std::plus<>());
		//
		std::cout << "--------------\n";
		// std::cout << "Number of Cards: " << cards.size() << std::endl;
		// std::cout << "Number of unique WonCards: " << won_cards.size() << std::endl;
		// std::cout << "Sum : " << sum << std::endl;
		std::cout << "--------------" << std::endl;
	}
}
