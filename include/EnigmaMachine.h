#ifndef ENIGMAMACHINE_H
#define ENIGMAMACHINE_H

#include "Rotor.h"
#include "Reflector.h"
#include "Plugboard.h"

class EnigmaMachine
{
private:
	Rotor* rotors[3];
	Reflector* reflector;
	Plugboard* plugboard;

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
	void setPlugboard(Plugboard* _plugboard);

//-------------------------getters-------------------------

	int getRotorPosition(int index) const ;
	int getRotorRing(int index) const ;

//-------------------------cryptographic_methods-------------------------
	/*
	transform:
		encrypt one character
	*/
	char transform(char takenChar);

	/*
	totalTransform:
		encrypt a string
	*/
	std::string totalTransform(std::string takenString);
};

#endif