#include "../include/Plugboard.h"
#include <cctype>

using namespace std;

Plugboard::Plugboard()
{
	reset();
}

Plugboard::Plugboard(const std::vector<std::pair<char, char>>& pairs)
{
	reset();
	for (const auto& p: pairs)
	{
		addPair(p.first, p.second);
	}
	
}

char Plugboard::swap(char character) const
{
	bool isLower = islower(character);
	if(!isalpha(character))
	{
		return character;
	}
	else if (!islower(character))
	{
		return toupper(mapping[tolower(character) - 'a']);
	}
	else
	{
		return mapping[character - 'a'];
	}

} 

void Plugboard::addPair(char firstC, char secondC)
{
	firstC = tolower(firstC);
	secondC = tolower(secondC);

	if(firstC == secondC)
	{
		throw std::invalid_argument("Both letters are same.");
	}

	int firstInt = firstC - 'a';
	int secondInt = secondC - 'a';

	if(isUsed[firstInt] || isUsed[secondInt])
	{
		throw std::invalid_argument("One of the letters is already used in another pair.");
	}

	mapping[firstInt] = secondC ;
	mapping[secondInt] = firstC ;
	isUsed[firstInt] = isUsed[secondInt] = true;

}

void Plugboard::reset()
{
	for (int i = 0; i < 26; ++i) 
	{
        mapping[i] = 'a' + i;
        isUsed[i] = false;
    }
}