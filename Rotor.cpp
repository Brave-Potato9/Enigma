#include "Rotor.h"
#include <algorithm>

using namespace std;

const string Rotor::ALPHABET = "abcdefghijklmnopqrstuvwxyz";

Rotor::Rotor(string _name, vector<char> _wiring, 
		    int _ringSetting, int _position, int _turnoverPosition):
	name(_name),
	wiring(_wiring),
	ringSetting(_ringSetting),
	position(_position),
	turnoverPosition(_turnoverPosition)
{}

string Rotor::getName() const
{
	return name;
}
	
int Rotor::getPosition() const
{
	return position;
}

void Rotor::setPosition(int _position)
{
	position = _position;
}

int Rotor::getTurnoverPosition() const
{
	return turnoverPosition;
}

std::string Rotor::getWiringString() const 
{
    return std::string(wiring.begin(), wiring.end());
}

void Rotor::setWiring(const std::string& newWiring) 
{
    if (newWiring.length() != 26) 
	{
        throw std::invalid_argument("Wiring must be exactly 26 characters");
    }

	std::string sorted = newWiring;
    std::sort(sorted.begin(), sorted.end());
    if (std::unique(sorted.begin(), sorted.end()) != sorted.end()) 
	{
        throw std::invalid_argument("All letters in wiring must be unique");
    }
	
    wiring.assign(newWiring.begin(), newWiring.end());
}

int Rotor::getRingSetting() const {
    return ringSetting;
}

bool Rotor::rotate()
{
	position = (position + 1) % 26;

	//if position is in turnover next roter must rotate
	return (position == turnoverPosition);
}
	

char Rotor::transform(char takenChar, bool isBackward) const 
{
    
    size_t index = ALPHABET.find(takenChar);
    
    //enable the position and setting 
    int shiftedIndex = (index + position - ringSetting + 26) % 26;
    
    char result;
    if (!isBackward) 
	{
        result = wiring[shiftedIndex];
    } 
	else 
	{
        auto it = find(wiring.begin(), wiring.end(), ALPHABET[shiftedIndex]);
        int wiringIndex = it - wiring.begin();
        result = ALPHABET[wiringIndex];
    }
    
	//reverse the shift
    int finalIndex = (ALPHABET.find(result) - position + ringSetting + 26) % 26;
    return ALPHABET[finalIndex];
}

bool Rotor::operator++(int)
{
	return this->rotate();
}