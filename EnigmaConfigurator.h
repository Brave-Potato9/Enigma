#ifndef ENIGMACONFIGURATOR_H
#define ENIGMACONFIGURATOR_H

#include "Rotor.h"
#include "Reflector.h"
#include <string>

class EnigmaConfigurator 
{
private:
    Rotor* rotors[3];
    Reflector* reflector;

    void clearAll();

	/*
	isValidWiring:
		check the taken wiring:
			1. is 26 char
			2. each character mentioned once

	*/
    bool isValidWiring(const std::string& wiring) const;

	/*
	generateRandomWiring:
		randomize a valid wiring
	*/
    std::string generateRandomWiring() const;

	/*
	getValidInt:
		get an int from user that is valid and bitween min and max
	*/
    int getValidInt(const std::string& prompt, int min, int max) const;

	/*
	getValidString:
		get an string from user that is valid
	*/
    std::string getValidString(const std::string& prompt, bool allowEmpty = false) const;

	/*
	configureRotor:
		user adjust the rotor manualy or randomly
	*/
    void configureRotor(int index);

	/*
	configureReflector:
		user adjust the reflector manualy or randomly
	*/
    void configureReflector();
    void generateRandomAll();
    bool saveConfig(const std::string& filename = "enigma_config.txt") const;
    void showStatus() const;

public:
    EnigmaConfigurator();
    ~EnigmaConfigurator();

    void run(); 
};

#endif