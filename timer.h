#pragma once		
	
#include <chrono>
#include <iostream>
#include <string>

class ScopedTimer 
{
private:
	std::chrono::high_resolution_clock::time_point start;
	std::string name;

public:
	ScopedTimer(const std::string& timerName = "") : start{std::chrono::high_resolution_clock::now()}, 
		name{timerName} {};

	~ScopedTimer() 
	{
		std::cout << *this << std::endl;
	}

	friend std::ostream& operator<<(std::ostream& os, const ScopedTimer& timer);
};

inline std::ostream& operator<<(std::ostream& os, const ScopedTimer& timer)
{
	auto elapsed = std::chrono::high_resolution_clock::now() - timer.start;
	auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
	os << " -- Timer '" << timer.name << "': " << duration_ms.count() << " ms";
	return os;
}
