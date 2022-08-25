#include "mainTests.hpp"

/**************************************************************/
/*                      RUN WEBSERV TEST                      */
/**************************************************************/

TEST_F(parsingTest, simpleConfig)
{
	std::cout << std::endl << BLUE_B;
	std::cout << "                                      ";
	std::cout << "           [ PARSING ] " << RESET << std::endl;

	simpleConfig(webservSimple);
}
