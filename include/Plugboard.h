#ifndef PLUGBOARD_H
#define PLUGBOARD_H

#include <array>
#include <vector>
#include <utility>
#include <stdexcept>

class Plugboard
{
private:
	std::array<char, 26> mapping;
	bool isUsed[26];

public:
	Plugboard();
	Plugboard(const std::vector<std::pair<char, char>>& pairs);

	char swap(char character) const; 
    void addPair(char firstC, char secondC);
    void reset();
};



#endif