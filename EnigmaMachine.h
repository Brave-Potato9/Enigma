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
	EnigmaMachine(Rotor* rotor[3]);
	~EnigmaMachine();

	void loadConfigs(const std::string& filename = "enigma_config.txt");

	void setRotor(int index, Rotor* rotor);
	void setReflector(Reflector* _reflector);

	char trasform(char takenChar);

	std::string totalTransform(std::string takenString);
};

#endif