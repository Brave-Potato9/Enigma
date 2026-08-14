#include "../include/EnigmaMachine.h"
#include "../include/Defines.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

//-------------------------constructor_and_destructor-------------------------
EnigmaMachine::EnigmaMachine(Rotor *rotor[3]):
    reflector(nullptr),
    plugboard(nullptr)
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
    delete plugboard;
}

//-------------------------file_management-------------------------
void EnigmaMachine::loadConfigs(const std::string& filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << BOLD << RED << "Error: Could not open config file: " << filename << endl << RESET;
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
        size_t start = line.find_first_not_of(" \t");
        if (start == string::npos) 
        {
            continue;
        }

        line = line.substr(start);
        
        if (!line.empty() && line.back() == '\r') 
        {
            line.pop_back();
        }

        if (line.empty() || line[0] == '#') 
        {
            continue;
        }

        size_t colon = line.find(':');
        if (colon == string::npos) 
        {
            continue;
        }

        string key = line.substr(0, colon);
        string value = line.substr(colon + 1);

        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        if (!value.empty() && value.back() == '\r') 
        {
            value.pop_back();
        }

        if (key == "rotor1" || key == "rotor2" || key == "rotor3")
        {
            vector<string> parts;
            stringstream ss(value);
            string part;
            while (getline(ss, part, ','))
            {
                part.erase(0, part.find_first_not_of(" \t"));
                part.erase(part.find_last_not_of(" \t") + 1);

                if (!part.empty() && part.back() == '\r') 
                {
                    part.pop_back();
                }

                if (!part.empty()) 
                {
                    parts.push_back(part);
                }
            }

            if (parts.size() != 5)
            {
                cerr << BOLD << RED << "Error: Invalid rotor line (expected 5 parts): " << value << endl << RESET;
                continue;
            }

            string name = parts[0];
            
            int ring = 0, pos = 0, turnover = 0;
            try 
            {
                ring = stoi(parts[1]);
                pos = stoi(parts[2]);
                turnover = stoi(parts[3]);
            } 
            catch (const std::exception& e) 
            {
                cerr << BOLD << RED << "Error parsing numbers in rotor line: " << value << endl << RESET;
                continue;
            }

            string wiring = parts[4];

            int index = (key == "rotor1") ? 0 : (key == "rotor2") ? 1 : 2;
            vector<char> wiringVec(wiring.begin(), wiring.end());
            rotors[index] = new Rotor(name, wiringVec, ring, pos, turnover);
            
        }
        else if (key == "reflector")
        {
            if (!value.empty() && value.back() == '\r') 
            {
                value.pop_back();
            }
            vector<char> wiringVec(value.begin(), value.end());
            reflector = new Reflector(wiringVec);
        }
        else if (key == "plugboard") 
        {
            if (value.length() != 26) 
            {
                cerr << BOLD << RED << "Error: Plugboard wiring must be 26 characters.\n" << RESET;
                continue;
            }
            
            vector<pair<char, char>> pairs;
            for (int i = 0; i < 26; ++i) 
            {
                char from = 'a' + i;
                char to = value[i];
                if (from != to) 
                {
                    if (from < to) 
                    {  
                        pairs.push_back({from, to});
                    }
                }
            }

            try 
            {
                Plugboard* pb = new Plugboard(pairs);
                setPlugboard(pb);
            } 
            catch (const exception& e) 
            {
                cerr << BOLD << RED << "Error loading plugboard: " << e.what() << "\n" << RESET;
            }
        }
    }

    file.close();
}

//-------------------------setters-------------------------
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

void EnigmaMachine::setPlugboard(Plugboard* _plugboard) 
{
    delete plugboard;
    plugboard = _plugboard;
}


int EnigmaMachine::getRotorPosition(int index) const 
{
    if (index >= 0 && index < 3 && rotors[index]) 
    {
        return rotors[index]->getPosition();
    }
    return -1;
}

int EnigmaMachine::getRotorRing(int index) const 
{
    if (index >= 0 && index < 3 && rotors[index]) 
    {
        return rotors[index]->getRingSetting();
    }

    return -1;
}

//-------------------------cryptographic_methods-------------------------
char EnigmaMachine::transform(char takenChar)
{

    //validation (ensure all rotors and the reflector are properly set)
    if (!rotors[0] || !rotors[1] || !rotors[2] || !reflector)
    {
        cerr << BOLD << RED << "Error: Machine not properly configured!\n" << RESET;
        return takenChar;
    }

    // Only lowercase letters are processed.
    if (takenChar < 'a' || takenChar > 'z')
    {
        return takenChar;
    }


    //steeping (rotor rotation)
    /*
        Before each character is encrypted, the rotors step as follows:

        1. The right rotor always steps forward by one position.
        2. If the middle rotor is at its notch position:
            - The left rotor steps forward.
            - The middle rotor also steps forward (double step).
        3. If the right rotor is at its notch position:
            - The middle rotor steps forward.
        4. If both middle and right rotors are at their notches, the middle rotor
            steps only once.
    */

    bool middleAtNotch = rotors[1]->atNotch();
    bool rightAtNotch  = rotors[2]->atNotch();

    // middle rotor triggers left rotor and itself.
    if (middleAtNotch)
    {
        rotors[0]->rotate();  // left rotor steps
    }

    // Middle rotor steps if either middle or right is at notch.
    if (middleAtNotch || rightAtNotch)
    {
        rotors[1]->rotate();  // middle rotor steps
    }

    // Right rotor always steps.
    rotors[2]->rotate();

    //apply the plugboard swap
    takenChar = plugboard->swap(takenChar);


    // forward path (Right → Middle → Left)
    /*
        The signal passes through the rotors from right to left
        Each rotor applies its substitution
    */

    char signal = takenChar;

    signal = rotors[2]->transform(signal, false);  // right rotor
    signal = rotors[1]->transform(signal, false);  // middle rotor
    signal = rotors[0]->transform(signal, false);  // left rotor


    // reflector
    /*
        After passing through all three rotors, the signal enters the reflector.
        The reflector is a fixed substitution that maps each letter to another,
        and it is symmetric so that the return path is well-defined.
    */

    signal = reflector->reflect(signal);


    // backward path (Left → Middle → Right)
    /*
        The reflected signal travels back through the rotors, but this time
        from left to right. Each rotor applies the inverse substitution
        (using the inverse wiring) to undo the forward transformation.
    */

    signal = rotors[0]->transform(signal, true);  // left rotor (inverse)
    signal = rotors[1]->transform(signal, true);  // middle rotor (inverse)
    signal = rotors[2]->transform(signal, true);  // right rotor (inverse)

    //apply the plugboard swap
    signal = plugboard->swap(signal);

    //output
    return signal;
}

string EnigmaMachine::totalTransform(string takenString)
{
    string cipherString = "";
    for (char c : takenString)
    {
        if (isalpha(c))
        {
            cipherString.push_back(transform(tolower(c)));
        }
        else
        {
            cipherString.push_back(c);
        }
    }

    return cipherString;
}