#include "../include/EnigmaConfigurator.h"
#include "../include/Defines.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <cctype>

using namespace std;

//-------------------------constructor-------------------------
EnigmaConfigurator::EnigmaConfigurator(): 
	reflector(nullptr), 
    plugboard(nullptr)
{
    for (int i = 0; i < 3; i++) 
	{
		rotors[i] = nullptr;
	}
}

//-------------------------destructor-------------------------
EnigmaConfigurator::~EnigmaConfigurator() 
{
    clearAll();
}

void EnigmaConfigurator::clearAll() 
{
    for (int i = 0; i < 3; i++) 
	{ 
		delete rotors[i]; 
		rotors[i] = nullptr; 
	}

    delete reflector;
    reflector = nullptr;

    delete plugboard;
    plugboard = nullptr;
}

//-------------------------validation_method-------------------------
bool EnigmaConfigurator::isValidWiring(const string& wiring) const 
{
    if (wiring.length() != 26) 
	{
		return false;
	}

    string sorted = wiring;
    sort(sorted.begin(), sorted.end());
    return unique(sorted.begin(), sorted.end()) == sorted.end();
}

//-------------------------random_configurating_methods-------------------------
string EnigmaConfigurator::generateRandomWiring() const 
{
    string alphabet = Rotor::ALPHABET;
    
    //is all rotor nullptr
    bool allNull = true;
    for (int i = 0; i < 3; i++) 
    {
        if (rotors[i] != nullptr) 
        {
            allNull = false;
            break;
        }
    }
    
    //if all rotor nullptr
    if (allNull) 
    {
        random_device rd;
        mt19937 g(rd());
        shuffle(alphabet.begin(), alphabet.end(), g); //shuffle the alphabet
        return alphabet;
    }
    
    //if one or more rotor setted
    while (true)
    {
        random_device rd;
        mt19937 g(rd());
        shuffle(alphabet.begin(), alphabet.end(), g); //shuffle the alphabet
        
        //check wiring is not similar to another
        bool duplicateFound = false;
        for (int i = 0; i < 3; i++)
        {
            if (rotors[i] != nullptr && rotors[i]->getWiringString() == alphabet)
            {
                duplicateFound = true;
                break;
            }
        }
        
        if (!duplicateFound)
        {
            return alphabet;
        }
    }
}

void EnigmaConfigurator::generateRandomAll() 
{
    cout << BOLD << CYAN << "\nGenerating random wiring for all rotors...\n" ;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, 25);
    for (int i = 0; i < 3; i++) 
    {
        string wiring = generateRandomWiring(); // randomize wiring
        int randomNotch = dist(gen); //randomize notch

        cout << "Rotor " << RESET << BOLD << MAGENTA << i + 1 << RESET << BOLD << CYAN << " wiring: " << wiring << "  notch: " << randomNotch << "\n";

        vector<char> wiringVec(wiring.begin(), wiring.end());
        
        delete rotors[i];
        rotors[i] = new Rotor("Random", wiringVec, 0, 0, randomNotch);
    }

    cout << RESET << endl;
}


//-------------------------getter-------------------------
int EnigmaConfigurator::getValidInt(const string& prompt, int min, int max) const 
{
    int value;
    while (true) 
	{
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < min || value > max) 
		{
            cin.clear();
            cin.ignore(1000, '\n');
            cerr << BOLD << RED << "Invalid input. Enter a number between " << min << " and " << max << ".\n" << RESET;
        } 
		else 
		{
            cin.ignore(1000, '\n');
            return value;
        }
    }
}

string EnigmaConfigurator::getValidString(const string& prompt, bool allowEmpty) const 
{
    string value;
    while (true) 
	{
        cout << prompt;
        getline(cin, value);
        if (value.empty() && !allowEmpty) 
		{
            cerr << BOLD << RED << "Input cannot be empty. Try again.\n" << RESET;
        } 
		else 
		{
            return value;
        }
    }
}

//-------------------------configuration_methods-------------------------
void EnigmaConfigurator::configureRotor(int index) 
{
    cout << BOLD << CYAN;
    cout << "\n+-------------------------------------------------------+\n";
    cout << "|                  Configuring Rotor " << RESET << BOLD << MAGENTA << index + 1 << RESET << BOLD << CYAN << "                  |\n";
    cout << "+-------------------------------------------------------+\n";
    cout << "| " << RESET << BOLD << MAGENTA << '1' << RESET << BOLD << CYAN << ". I                                                  |\n";
    cout << "| " << RESET << BOLD << MAGENTA << '2' << RESET << BOLD << CYAN << ". II                                                 |\n";
    cout << "| " << RESET << BOLD << MAGENTA << '3' << RESET << BOLD << CYAN << ". III                                                |\n";
    cout << "| " << RESET << BOLD << MAGENTA << '4' << RESET << BOLD << CYAN << ". IV                                                 |\n";
    cout << "| " << RESET << BOLD << MAGENTA << '5' << RESET << BOLD << CYAN << ". V                                                  |\n";
    cout << "| " << RESET << BOLD << MAGENTA << '6' << RESET << BOLD << CYAN << ". Custom                                             |\n";
    cout << "+-------------------------------------------------------+\n";
    cout << RESET;
    
    int choice = getValidInt("Choice: ", 1, 6);
    
    string name, wiringStr;
    int turnover = 0;
    
    //standard rotors
    vector<string> rotorNames = {"I", "II", "III", "IV", "V"};
    vector<string> rotorWirings = {
        "ekmflgdqvzntowyhxuspaibrcj",
        "ajdksiruxblhwtmcqgznpyfvoe",
        "bdfhjlcprtxvznyeiwgakmusqo",
        "esovpzjayquirhxlnftgkcdmwb",
        "vzbrgityupsdnhlxawmjqofeck"
    };

    vector<int> turnovers = {16, 4, 21, 9, 25};
    
    if (choice == 6) //manualy customize the rotor
	{
        cout << "Enter Name: ";
        cin >> name;

        cout << "Enter wiring (26 unique letters): ";
        cin >> wiringStr;
        while (!isValidWiring(wiringStr)) 
		{
            cerr << BOLD << RED << "Invalid. Try again: " << RESET;
            cin >> wiringStr;
        }

        turnover = getValidInt("Turnover position (0-25): ", 0, 25);
    } 
	else //choose from standard rotors
	{
        for(auto rotor: rotors)
        {
            if(rotor != nullptr && rotor->getWiringString() == rotorWirings[choice - 1])
            {
                cerr << BOLD << RED << "This rotor is chosen before. try again\n" << RESET;
                return;
            }
        }

        name = rotorNames[choice - 1];
        wiringStr = rotorWirings[choice - 1];
        turnover = turnovers[choice - 1];
        cout << "Using " << name << " (turnover: " << turnover << ")\n";
    }
    
    int ring = getValidInt("Ring Setting (0-25): ", 0, 25);
    int pos = getValidInt("Initial Position (0-25): ", 0, 25);
    
    vector<char> wiringVec(wiringStr.begin(), wiringStr.end());
    delete rotors[index];
    rotors[index] = new Rotor(name, wiringVec, ring, pos, turnover);
    
    cout << BOLD << GREEN << "Rotor " << index + 1 << " configured.\n" << RESET;
}

void EnigmaConfigurator::configureReflector() 
{
    cout << BOLD << CYAN;
    cout << "\n+-------------------------------------------------------+\n";
    cout << "|                 Configuring Reflector                 |\n";
    cout << "+-------------------------------------------------------+\n";
    cout << "| " << RESET << BOLD << MAGENTA << '1' << RESET << BOLD << CYAN << ". B                                                  |\n";
    cout << "| " << RESET << BOLD << MAGENTA << '2' << RESET << BOLD << CYAN << ". C                                                  |\n";
    cout << "| " << RESET << BOLD << MAGENTA << '3' << RESET << BOLD << CYAN << ". Custom                                             |\n";
    cout << "+-------------------------------------------------------+\n";
    cout << RESET;

    int choice = getValidInt("Choice: ", 1, 3);
    
    string wiringStr;
    if (choice == 3) //manualy customize the reflector
	{
        cout << "Enter wiring (26 unique letters): ";
        cin >> wiringStr;
        while (!isValidWiring(wiringStr)) 
		{
            cerr << BOLD << RED << "Invalid. Try again: " << RED;
            cin >> wiringStr;
        }
    } 
	else //choose from standard reflector
	{
        //standard reflectors
        vector<string> wirings = {
            "yruhqsldpxngokmiebfzcwvjat",
            "fvpjiaoyedrzxwgctkuqsbnmhl"
        };

        wiringStr = wirings[choice - 1];
        cout << "Using reflector " << (char)('A' + choice - 1) << "\n";
    }
    
    vector<char> wiringVec(wiringStr.begin(), wiringStr.end());

    delete reflector;
    reflector = new Reflector(wiringVec);
    cout << BOLD << GREEN << "Reflector configured.\n" << RESET;
}

void EnigmaConfigurator::configurePlugboard()
{
    cout << BOLD << CYAN;
    cout << "\n+-------------------------------------------------------+\n";
    cout << "|                 Configuring Plugboard                 |\n";
    cout << "+-------------------------------------------------------+\n";
    cout << "| " << RESET << BOLD << MAGENTA << '1' << RESET << BOLD << CYAN << ". Set manualy                                        |\n";
    cout << "| " << RESET << BOLD << MAGENTA << '2' << RESET << BOLD << CYAN << ". Set as defaulf                                     |\n";
    cout << "+-------------------------------------------------------+\n";
    cout << RESET;

    int choice = getValidInt("Choice: ", 1, 2);

    delete plugboard;
    plugboard = new Plugboard();

    if(choice == 1)
    {
        char input1, input2;

        while (true)
        {
            cout << "Enter the chracters space seprated('q q' for stop): ";
            cin >> input1 >> input2;

            if(!isalpha(input1) || !isalpha(input2))
            {
                cerr << BOLD << RED << "please enter only two character space seprated" << RESET;
                continue;
            }
            
            if(input1 == 'q' && input2 == 'q')
            {
                break;
            }
            
            try
            {
                plugboard->addPair(input1, input2);
            }
            catch(const std::exception& e)
            {
                std::cerr << BOLD << RED << e.what() << RESET << '\n';
                continue;
            }
            
        }
        
    }
    
    cout << BOLD << GREEN << "Plugboard configured.\n" << RESET;
}

//-------------------------file_management-------------------------
bool EnigmaConfigurator::saveConfig(const string& filename) const 
{
    ofstream file(filename);
    if (!file.is_open()) 
	{
        cerr << BOLD << RED << "Error: Cannot write to " << filename << endl << RESET;
        return false;
    }
    
    file << "# Enigma Machine Configuration\n";
    file << "# Format: name, ringSetting, position, turnover, wiring\n\n";
    
    for (int i = 0; i < 3; i++) 
	{
        if (rotors[i]) 
		{
            file << "rotor" << i + 1 << ": " 
                 << rotors[i]->getName() << ", "
                 << rotors[i]->getRingSetting() << ", "
                 << rotors[i]->getPosition() << ", "
                 << rotors[i]->getTurnoverPosition() << ", "
                 << rotors[i]->getWiringString() << "\n";
        }
    }
    
    if (reflector) 
	{
        file << "reflector: " << reflector->getWiringString() << "\n";
    }

    if (plugboard) 
    {
        file << "plugboard: " << plugboard->getMappingString() << "\n";
    }
    
    file.close();
    cout << BOLD << GREEN << "Configuration saved to " << filename << "\n" << RESET;
    return true;
}

//-------------------------UI_methods-------------------------
void EnigmaConfigurator::showStatus() const 
{
    cout << BOLD << CYAN;
    cout << "\n+-------------------------------------------------------+\n";
    cout << "|                 Current Configuration                 |\n";
    cout << "+-------------------------------------------------------+\n";
    cout << RESET;

    for (int i = 0; i < 3; i++)  //show each rotor's status
	{
        if (rotors[i]) 
		{
            cout << GREEN << "Rotor " << i + 1 << ": " << rotors[i]->getName()
                 << "  Ring: " << rotors[i]->getRingSetting()
                 << "  Pos: " << rotors[i]->getPosition()
                 << "  Turnover: " << rotors[i]->getTurnoverPosition()
                 << "  Wiring: " << rotors[i]->getWiringString() << "\n" << RESET;
        } 
		else 
		{
            cout << RED << "Rotor " << i + 1 << ": Not set\n" << RESET;
        }
    }
    
	if (reflector) //show reflector's status
	{
        cout << GREEN << "Reflector: " << reflector->getWiringString() << "\n" << RESET;
    } 
	else 
	{
        cout << RED << "Reflector: Not set\n" << RESET;
    }

    if (plugboard) 
    {
        cout << GREEN << "Plugboard: ";
        bool hasPairs = false;

        for (int i = 0; i < 26; ++i) 
        {
            char from = static_cast<char>('a' + i);
            char to = plugboard->getMapping()[i];
            if (from != to) 
            {
                cout << from << "<->" << to << " ";
                hasPairs = true;
            }
        }

        if (!hasPairs) 
        {
            cout << ORANGE << "(empty)" << RESET;
        }
        cout << "\n" << RESET;
    } 
    else 
    {
        cout << ORANGE << "Plugboard: Not set\n" << RESET;
    }

    cout << BOLD << CYAN << "+-------------------------------------------------------+\n" << RESET;
}

//-------------------------main_method-------------------------
void EnigmaConfigurator::run() 
{
    int option;
    while (true) 
	{
        showStatus();
        cout << BOLD << CYAN;
        cout << "\n+-------------------------------------------------------+\n";
        cout << "|                  Configuring Rotor                    |\n";
        cout << "+-------------------------------------------------------+\n";
        cout << "| " << RESET << BOLD << MAGENTA << '1' << RESET << BOLD << CYAN << ". Configure Rotor 1                                  |\n";
        cout << "| " << RESET << BOLD << MAGENTA << '2' << RESET << BOLD << CYAN << ". Configure Rotor 2                                  |\n";
        cout << "| " << RESET << BOLD << MAGENTA << '3' << RESET << BOLD << CYAN << ". Configure Rotor 3                                  |\n";
        cout << "| " << RESET << BOLD << MAGENTA << '4' << RESET << BOLD << CYAN << ". Configure Reflector                                |\n";
        cout << "| " << RESET << BOLD << MAGENTA << '5' << RESET << BOLD << CYAN << ". Configure Plugboard                                |\n";
        cout << "| " << RESET << BOLD << MAGENTA << '6' << RESET << BOLD << CYAN << ". Generate random wiring                             |\n";
        cout << "| " << RESET << BOLD << MAGENTA << '7' << RESET << BOLD << CYAN << ". Save configuration                                 |\n";
        cout << "| " << RESET << BOLD << MAGENTA << '8' << RESET << BOLD << CYAN << ". Exit                                               |\n";
        cout << "+-------------------------------------------------------+\n";
        cout << RESET;

        option = getValidInt("Choice: ", 1, 8);
        
        switch (option) 
		{
            case 1: //Configure Rotor 1
				configureRotor(0); 
				break;

            case 2: //Configure Rotor 2
				configureRotor(1); 
				break;

            case 3: //Configure Rotor 3
				configureRotor(2); 
				break;

            case 4: //Configure Reflector
				configureReflector(); 
				break;

            case 5: //Configure Plugboard
				configurePlugboard(); 
				break;

            case 6: //Generate random wiring
				generateRandomAll(); 
				break;

            case 7: //Save configuration
				if(reflector && rotors[0] && rotors[1] && rotors[2])
				{
					saveConfig(); 
				}
				else
				{
					cerr << BOLD << RED << "Please set all rotors an reflector first\n" << RESET;
				}
				break;

            case 8: //Exit 
                return;

            default: 
                cerr << BOLD << RED << "Not valid option! try again\n" << RESET;
				break;
        }
    }
}