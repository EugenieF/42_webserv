#include "mainTests.hpp"

/**************************************************************/
/*                      RUN WEBSERV TEST                      */
/**************************************************************/

TEST_F(parsingSimpleTest, simpleConfig)
{
	std::cout << std::endl << BLUE_B;
	std::cout << "                                      ";
	std::cout << "           [ PARSING ] " << RESET << std::endl;

	checkServer(webservSimple, expectedServer1);
	checkLocation(webservSimple, expectedLocation1);
}

TEST_F(parsingErrorTest, errorFile)
{
	std::vector<t_invalidFile *>::const_iterator	ite;

	for (ite = invalidFile.begin(); ite != invalidFile.end(); ite++)
		checkErrorException(**ite);

}
