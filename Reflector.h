#ifndef REFLECTOR_H
#define REFLECTOR_H

#include <vector>
#include <iostream>

class Reflector
{
private:
	std::vector<char> wiring;

public:
	//string contain all english letter
	static const std::string ALPHABET;

//-------------------------constructor-------------------------
	Reflector(const std::vector<char>& _wiring);

//-------------------------getter-------------------------
	std::string getWiringString() const;

//-------------------------cryptographic_method-------------------------
	/*
	reflect:
		transform the takenChar by symetrical wiring
	*/
    char reflect(char takenChar) const;
	
};

#endif