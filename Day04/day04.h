#include <locale>
#include <ranges>
#include <algorithm>
#include <iostream>
#include <vector>
#include <set>
#include <numeric>
#include <functional>
#include <map>

#include "../timer.h"
#include "../readfile.h"


constexpr char NULLCHAR = '\0';
constexpr char EMPTYCHAR = '.';


using CardIndex = int;

struct Card 
{
	std::vector<int> winners;
	std::vector<int> drawns;
	int card_index{-1};

	int n_winning () const {
		auto is_winning = [this](int drawn) -> bool
		{ 
			return std::ranges::contains(winners, drawn);
		};

		return std::ranges::count_if(drawns, is_winning);
	}

	int winning_value() const 
	{
		const int winning_count = n_winning();
		return (winning_count > 0) ? 1 << (winning_count - 1) : 0;
	}
	
};


std::ostream& operator<<(std::ostream& os, const std::vector<int>& array)  
{
	os << "[";
	os << array[0];
	for (int i : std::views::iota(1, static_cast<int>(array.size()))) 
	{
			os << ", " << array[i]; 
	}
	os << "]";
	return os;
}


std::ostream& operator<<(std::ostream& os, const Card& card) 
{
	os << "Card: " << card.card_index;
	os << ", Winners: " << card.winners;
	os << ", Drawns: " << card.drawns;
	return os;
}


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


namespace Day04A 
{
	inline void PrintSolution() 
	{
		fs::path file_path("Day04/Input04A.txt");
		// fs::path file_path("Day04/Test04A.txt");
		std::cout << "Day04 Part A" << ": " << file_path.string() <<  std::endl;
		std::vector<std::string> input = read_lines_from_file(file_path);
		// Get cards
		const std::map<CardIndex, Card> cards = parse_lines(input);

		// total number of winnings
		auto to_winning = [] (auto& pair ) -> int { return pair.second.winning_value();};
		int sum = std::ranges::fold_left( cards | std::views::transform(to_winning), 0, std::plus<>());
		//
		std::cout << "--------------\n";
		std::cout << "Sum: " << sum << std::endl;
		std::cout << "--------------" << std::endl;
	}
}
	
namespace Day04B 
{
	inline void PrintSolution() 
	{
		fs::path file_path("Day04/Input04A.txt");
		// fs::path file_path("Day04/Test04A.txt");
		std::cout << "Day04 Part B" << ": " << file_path.string() <<  std::endl;
		std::vector<std::string> input = read_lines_from_file(file_path);
		// Get the cards
		const std::map<CardIndex, Card> cards = parse_lines(input);

		std::map<CardIndex, int> won_cards;
		// We always have first cars
		for (const auto& [ card_index, card] : cards) 
		{
			if (!won_cards.contains(card_index)) 
			{ 
				won_cards[card_index] = 1;
			}
			int winning = card.n_winning(); 
       #if !DEBUG
       std::cout << "Card: " << card_index << " Card: " << card.n_winning() << " -> " << card <<  std::endl;
       # endif

			if (winning > 0) {
				for ( int next_index : std::views::iota( card_index + 1,  card_index + 1 + winning ) ) {
					if (won_cards.contains(next_index))
				       {
						won_cards[next_index] = won_cards[next_index] + won_cards[card_index];
					}
					else {
						won_cards[next_index] = 1 + won_cards[card_index]; 
					}
					// take care of duplicate cards
				}
			}
	       }


		int sum = std::ranges::fold_left( std::views::values(won_cards), 0, std::plus<>());
		//
		std::cout << "--------------\n";
		std::cout << "Number of Cards: " << cards.size() << std::endl;
		std::cout << "Number of unique WonCards: " << won_cards.size() << std::endl;
		std::cout << "Sum : " << sum << std::endl;
		std::cout << "--------------" << std::endl;
	}
}
