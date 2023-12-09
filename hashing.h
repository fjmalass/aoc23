# pragma once

// Macro to implement std::has to a type to create unordered_map and unordered_sets
//
// Sample usage
// struct Point{ int x, int y};
// IMPLEMENT_STD_HASH(Point, value.x, value.y);
#define IMPLEMENT_STD_HASH(T, ...)
namespace std\
{\
	template<>\
	struct hash<T>\
	{\
		size_t operator()(const T& value) const\
		{\	
			size_t result = 0ull;\
			Utilities::HashCombine(result, __VA_ARGS__);\
			return result;
		}\
	};\
}
