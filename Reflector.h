#ifndef REFLECTOR_H
#define REFLECTOR_H

#include <vector>
#include <iostream>

class Reflector
{
private:
	std::vector<char> wiring;

public:
	static const std::string ALPHABET;

	Reflector(const std::vector<char>& _wiring);

	/*
	reflect:
		transform the takenChar by symetrical wiring
	*/
    char reflect(char takenChar) const;
	std::string getWiringString() const;

};

#endif