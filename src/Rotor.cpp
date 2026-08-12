#include "../include/Rotor.h"
#include <algorithm>
#include <stdexcept>

using namespace std;

const string Rotor::ALPHABET = "abcdefghijklmnopqrstuvwxyz";

//-------------------------constructor-------------------------
Rotor::Rotor(
    string _name,
    vector<char> _wiring,
    int _ringSetting,
    int _position,
    int _turnoverPosition
)
    : name(_name),
      wiring(_wiring),
      ringSetting(_ringSetting),
      position(_position),
      turnoverPosition(_turnoverPosition)
{
    //wiring validation
    if (wiring.size() != 26)
    {
        throw invalid_argument("Rotor wiring must contain exactly 26 characters.");
    }

    string wiringString(wiring.begin(), wiring.end());

    string sorted = wiringString;
    sort(sorted.begin(), sorted.end());

    if (sorted != ALPHABET)
    {
        throw invalid_argument(
            "Rotor wiring must contain every letter a-z exactly once."
        );
    }

    //ring  setting validation
    if (ringSetting < 0 || ringSetting >= 26)
    {
        throw invalid_argument("Ring setting must be between 0 and 25.");
    }

    //position validation
    if (position < 0 || position >= 26)
    {
        throw invalid_argument("Position must be between 0 and 25.");
    }

    //turnover validation
    if (turnoverPosition < 0 || turnoverPosition >= 26)
    {
        throw invalid_argument("Turnover position must be between 0 and 25.");
    }

    //set inverseWiring with respect to Wiring
    inverseWiring.resize(26);

    for (int i = 0; i < 26; ++i)
    {
        inverseWiring[wiring[i] - 'a'] = 'a' + i;
    }
}

//-------------------------setters-------------------------
void Rotor::setPosition(int _position)
{
    //validation
    if (_position < 0 || _position >= 26)
    {
        throw invalid_argument("Position must be between 0 and 25.");
    }

    position = _position;
}


void Rotor::setWiring(const string& newWiring)
{
    //validation(it must be 26 letter)
    if (newWiring.length() != 26)
    {
        throw invalid_argument(
            "Wiring must be exactly 26 characters."
        );
    }

    string sorted = newWiring;
    sort(sorted.begin(), sorted.end());

    //validation(it must be a permutation of all english letter)
    if (sorted != ALPHABET)
    {
        throw invalid_argument(
            "Wiring must contain every letter a-z exactly once."
        );
    }

    wiring.assign(newWiring.begin(), newWiring.end());

    // Rebuild inverse wiring
    inverseWiring.resize(26);

    for (int i = 0; i < 26; ++i)
    {
        inverseWiring[wiring[i] - 'a'] = 'a' + i;
    }
}

//-------------------------getters-------------------------
string Rotor::getName() const
{
    return name;
}


int Rotor::getPosition() const
{
    return position;
}


int Rotor::getTurnoverPosition() const
{
    return turnoverPosition;
}


string Rotor::getWiringString() const
{
    return string(wiring.begin(), wiring.end());
}


int Rotor::getRingSetting() const
{
    return ringSetting;
}

//-------------------------rotation_logic_management-------------------------

bool Rotor::atNotch() const
{
    // Ring setting moves the notch relative to the window
    return ((position + ringSetting) % 26) == turnoverPosition;
}


void Rotor::rotate()
{
    //rotate the rotor once
    position = (position + 1) % 26;
}

//-------------------------cryptographic_method-------------------------
char Rotor::transform(char takenChar, bool isBackward) const
{
    size_t index = ALPHABET.find(takenChar);

    if (index == string::npos)
    {
        return takenChar;
    }

    int inputIndex = static_cast<int>(index);

    if (!isBackward) // rotor[2] -> rotor[1] -> rotor[0] -> reflector
    {
        // Window/ring offset
        int shiftedIndex =
            (inputIndex + position - ringSetting + 26) % 26;

        // Pass through rotor wiring
        int wiredIndex =
            wiring[shiftedIndex] - 'a';

        // Undo window/ring offset
        int outputIndex =
            (wiredIndex - position + ringSetting + 26) % 26;

        return ALPHABET[outputIndex];
    }
    else // rotor[2] <- rotor[1] <- rotor[0] <- reflector
    {
        // Apply window/ring offset before inverse mapping
        int shiftedInput = (inputIndex + position - ringSetting + 26) % 26;

        // Use inverse wiring on the shifted input
        int invIndex = inverseWiring[shiftedInput] - 'a'; //in backward use inverseWiring

        // Undo the offset to get final output
        int outputIndex = (invIndex - position + ringSetting + 26) % 26;

        return ALPHABET[outputIndex];
    }
}
