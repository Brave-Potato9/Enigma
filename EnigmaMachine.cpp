#include "EnigmaMachine.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

EnigmaMachine::EnigmaMachine(Rotor *rotor[3]):
    reflector(nullptr)
{
    for (int i = 0; i < 3; i++)
    {
        setRotor(i, rotor[i]);
    }
}

EnigmaMachine::~EnigmaMachine()
{
    for (int i = 0; i < 3; i++)
    {
        delete rotors[i];
    }
    delete reflector;
}

void EnigmaMachine::loadConfigs(const std::string& filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Could not open config file: " << filename << endl;
        return;
    }

    for (int i = 0; i < 3; i++)
    {
        delete rotors[i];
        rotors[i] = nullptr;
    }
    delete reflector;
    reflector = nullptr;

    string line;
    while (getline(file, line))
    {
        line.erase(0, line.find_first_not_of(" \t"));
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        istringstream iss(line);
        string key, value;
        if (!getline(iss, key, ':'))
        {
            continue;
        }

        if (!getline(iss, value))
        {
            continue;
        }

        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        if (key == "rotor1" || key == "rotor2" || key == "rotor3")
        {
            string name, wiring;
            int ring, pos, turnover;
            char comma;
            istringstream valStream(value);
            if (valStream >> name >> comma >> ring >> comma >> pos >> comma >> turnover >> comma >> wiring)
            {
                int index = (key == "rotor1") ? 0 : (key == "rotor2") ? 1 : 2;
                vector<char> wiringVec(wiring.begin(), wiring.end());
                rotors[index] = new Rotor(name, wiringVec, ring, pos, turnover);
            }
        }
        else if (key == "reflector")
        {
            vector<char> wiringVec(value.begin(), value.end());
            reflector = new Reflector(wiringVec);
        }
    }
    file.close();
}

void EnigmaMachine::setRotor(int index, Rotor *rotor)
{
    if (index >= 0 && index < 3)
    {
        rotors[index] = rotor;
    }
}

void EnigmaMachine::setReflector(Reflector *_reflector)
{
    reflector = _reflector;
}

char EnigmaMachine::trasform(char takenChar)
{
    if ((*rotors[0])++)
    {
        if ((*rotors[1])++)
        {
            (*rotors[2])++;
        }
    }

    char cipherChar = takenChar;

    for (int i = 0; i < 3; i++)
    {
        cipherChar = rotors[i]->transform(cipherChar, false);
    }

    cipherChar = reflector->reflect(cipherChar);

    for (int i = 0; i < 3; i++)
    {
        cipherChar = rotors[2 - i]->transform(cipherChar, true);
    }

    return cipherChar;
}

string EnigmaMachine::totalTransform(string takenString)
{
    string cipherString = "";
    for (char c : takenString)
    {
        if (isalpha(c))
        {
            cipherString.push_back(trasform(tolower(c)));
        }
        else
        {
            cipherString.push_back(c);
        }
    }

    return cipherString;
}