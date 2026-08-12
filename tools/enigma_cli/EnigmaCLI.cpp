#include "../../include/EnigmaMachine.h"
#include "../../include/Defines.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void printBanner() 
{
    cout << BOLD << CYAN;
    cout << "+-------------------------------------------------------+\n";
    cout << "|                   ENIGMA MACHINE                      |\n";
    cout << "+-------------------------------------------------------+\n";
    cout << "| Type your messages below. ( " << RESET << BOLD << MAGENTA << ":h" << RESET << BOLD << CYAN << " for commands)          |\n";
    cout << "+-------------------------------------------------------+\n";
    cout << RESET;
}

/*
printHelp:
    print all commands and their effects
*/
void printHelp() 
{
    cout << BOLD << YELLOW;
    cout << "\n  Available commands:\n";
    cout << "   [" << RESET << BOLD << MAGENTA << ":q" << RESET << BOLD << YELLOW << "] Quit the program\n";
    cout << "   [" << RESET << BOLD << MAGENTA << ":r" << RESET << BOLD << YELLOW << "] Reset machine to initial config\n";
    cout << "   [" << RESET << BOLD << MAGENTA << ":c" << RESET << BOLD << YELLOW << "] Clear the screen\n";
    cout << "   [" << RESET << BOLD << MAGENTA << ":s" << RESET << BOLD << YELLOW << "] Show the current status\n";
    cout << "   [" << RESET << BOLD << MAGENTA << ":h" << RESET << BOLD << YELLOW << "] Show this help\n";
    cout << RESET;
}

int main(int argc, char* argv[])
{
	// for colerful windows terminal
	enableVirtualTerminal();

	string configFile = "enigma_config.txt";
    if (argc > 1) 
    {
        configFile = argv[1];
    }

	// Create machine instance with empty rotors
    Rotor* emptyRotors[3] = {nullptr, nullptr, nullptr};
    EnigmaMachine enigma(emptyRotors);
    
    // Load configuration
    enigma.loadConfigs(configFile);
	printBanner();

    string input;
    while (true) 
	{
        cout <<"> ";
        getline(cin, input);

        size_t start = input.find_first_not_of(" \t");
        if (start != string::npos) input = input.substr(start);
        size_t end = input.find_last_not_of(" \t");
        if (end != string::npos) input = input.substr(0, end + 1);

        if (input.empty()) 
		{
			continue;
		}

        //=================commands=================

        if (input == ":q") //quit
		{
            cout << "Goodbye.\n";
            break;
        }

        if (input == ":r") //reset
		{
            enigma.loadConfigs(configFile);
            cout << BOLD << GREEN << "  Machine reset to initial configuration.\n" << RESET;
            continue;
        }

        if (input == ":c") //clear
		{
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif
            printBanner();
            continue;
        }

        if (input == ":s") //status
        {
            cout << BOLD << ORANGE;
            cout << "  Rotor 1: pos=" << enigma.getRotorPosition(0) 
                << ", ring=" << enigma.getRotorRing(0) << "\n";
            cout << "  Rotor 2: pos=" << enigma.getRotorPosition(1) 
                << ", ring=" << enigma.getRotorRing(1) << "\n";
            cout << "  Rotor 3: pos=" << enigma.getRotorPosition(2) 
                << ", ring=" << enigma.getRotorRing(2) << "\n";
            cout << RESET;
            continue;
        }

        if (input == ":h") //help
		{
            printHelp();
            continue;
        }

        //=================plain_text=================

        string output = enigma.totalTransform(input);
        cout << BOLD << YELLOW << "  " << output << RESET << "\n";
    }

    return 0;
}