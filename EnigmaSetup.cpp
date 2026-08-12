#include "EnigmaConfigurator.h"
#include "Defines.h"

int main()
{
	// for colerful windows terminal
	enableVirtualTerminal();

	EnigmaConfigurator enigmaConfigurator;
	enigmaConfigurator.run();
	return 0;
}