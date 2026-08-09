#ifndef ENIGMAMACHINE_H
#define ENIGMAMACHINE_H

#include "Rotor.h"
#include "Reflector.h"

class EnigmaMachine
{
private:
	Rotor* rotors[3];
	Reflector* reflector;

public:
//-------------------------constructor_and_destructor-------------------------
	EnigmaMachine(Rotor* rotor[3]);
	~EnigmaMachine();

//-------------------------file_management-------------------------
	/*
	loadConfigs:
		read and set the initial settings of rotors and reflector
	*/
	void loadConfigs(const std::string& filename = "enigma_config.txt");

//-------------------------setters-------------------------
	/*
	setRotor:
		set one of the rotors
	*/
	void setRotor(int index, Rotor* rotor);
	void setReflector(Reflector* _reflector);

//-------------------------cryptographic_methods-------------------------
	/*
	transform:

	*/
	char trasform(char takenChar);

	std::string totalTransform(std::string takenString);
};

#endif