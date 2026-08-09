#ifndef ROTOR_H
#define ROTOR_H

#include <iostream>
#include <vector>

class Rotor
{
private:
	std::string name;

	//show that each alphabet is connected to which one
	std::vector<char> wiring;

	//initial rotation of rotor
	int ringSetting;
	int position;

	//the position that next rotor rotate
	int turnoverPosition;

public:
	static const std::string ALPHABET;

	Rotor(std::string _name, std::vector<char> _wiring, 
		  int _ringSetting = 0, int _position = 0, int _turnoverPosition = 0);

	std::string getName() const;
	int getPosition() const;
    void setPosition(int _position);
	int getTurnoverPosition() const;
	std::string getWiringString() const;
	void setWiring(const std::string& newWiring);
	int getRingSetting() const;

	/*
	rotate:
		move the position forward if get to turnover return true
	*/
	bool rotate();

	/*
	transform:
		transform the character with respect to direction(by enabling position and ringSetting):
			a <-> wiring[0]
			b <-> wiring[1]
			   .
			   .
			   .
			z <-> wiring[25]
	*/
	char transform(char takenCharacter, bool isBackward = false) const;

	//overloaded operator++ for rotation
	bool operator++(int);

};

#endif