#include "Reflector.h"

using namespace std;

const string Reflector::ALPHABET = "abcdefghijklmnopqrstuvwxyz";

Reflector::Reflector(const vector<char>& _wiring):
	wiring(_wiring)
{}

char Reflector::reflect(char takenChar) const
{
    return wiring[ALPHABET.find(takenChar)];
}
	
string Reflector::getWiringString() const
{
	return string(wiring.begin(), wiring.end());
}