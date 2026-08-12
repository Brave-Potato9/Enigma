#include "Reflector.h"

#include <algorithm>
#include <stdexcept>

using namespace std;

const string Reflector::ALPHABET = "abcdefghijklmnopqrstuvwxyz";

//-------------------------constructor-------------------------
Reflector::Reflector(const vector<char>& _wiring)
    : wiring(_wiring)
{
	//validation
    if (wiring.size() != 26)
	{
        throw invalid_argument(
            "Reflector wiring must contain exactly 26 characters."
        );
	}

    string wiringString(wiring.begin(), wiring.end());

    string sorted = wiringString;
    sort(sorted.begin(), sorted.end());

	//validation
    if (sorted != ALPHABET)
	{
        throw invalid_argument(
            "Reflector wiring must contain every letter a-z exactly once."
        );
	}

    // A real Enigma reflector must be an involution: R(R(x)) = x
    for (int i = 0; i < 26; ++i)
    {
        int j = wiring[i] - 'a';

        if (j == i)
		{
            throw invalid_argument(
                "Enigma reflector cannot contain fixed points."
            );
		}

        if (wiring[j] != 'a' + i)
		{
            throw invalid_argument(
                "Reflector wiring must be symmetric."
            );
		}
    }
}

//-------------------------getter-------------------------
string Reflector::getWiringString() const
{
    return string(wiring.begin(), wiring.end());
}

//-------------------------cryptographic_method-------------------------
char Reflector::reflect(char takenChar) const
{
    size_t index = ALPHABET.find(takenChar);

    if (index == string::npos) //if one character is not in alphabet return itself
	{
        return takenChar;
	}

    return wiring[index];
}
