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

#include "aoc.h"
#include "timer.h"
#include "readfile.h"

namespace fs = std::filesystem;

// #include "Day01/day01.h"
// #include "Day02/day02.h"
#include "Day03/day03.h"


int main()
{
	constexpr auto partASolver = &Day03A::PrintSolution;
	constexpr auto partBSolver = &Day03B::PrintSolution;

	const std::array<std::function<void()>, 2> solvers{partASolver, partBSolver};
	const std::array<std::string, 2> parts{"partA", "partB"};

	
	for (size_t i = 0; i < solvers.size(); ++i) 
	{
		ScopedTimer timer{parts[i]};
		solvers[i]();
	}
	// for (std::tuple<const std::function<void()>& , std::string&> pair : std::ranges::zip_view(solvers, parts)) 
	// {
	// 	ScopedTimer timer {std::get<1>(pair)};
	// 	std::get<0>(pair)();
	// }
}
