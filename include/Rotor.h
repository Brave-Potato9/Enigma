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
	std::vector<char> inverseWiring;

	//initial rotation of rotor
	int ringSetting;
	int position;

	//the position that next rotor rotate
	int turnoverPosition;

public:
	//string contain all english letter
	static const std::string ALPHABET;

//-------------------------constructor-------------------------
	Rotor(std::string _name, std::vector<char> _wiring, 
		  int _ringSetting = 0, int _position = 0, int _turnoverPosition = 0);

//-------------------------setters-------------------------
	void setPosition(int _position);
	void setWiring(const std::string& newWiring);

//-------------------------getters-------------------------
	std::string getName() const;
	int getPosition() const;
	int getTurnoverPosition() const;
	std::string getWiringString() const;
	int getRingSetting() const;

//-------------------------rotation_logic_management-------------------------
	/*
	atNotch:
		check the position with respect to ringSetting is at notch or not (for rotating next rotor)
	*/
	bool atNotch() const;

	/*
	rotate:
		move the position forward
	*/
	void rotate();

//-------------------------cryptographic_method-------------------------
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

};

#endif