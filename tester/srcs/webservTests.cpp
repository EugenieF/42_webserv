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
	checkErrorException(absentFile);
	checkErrorException(errorRoot);
	checkErrorException(nestedLocation);
	checkErrorException(duplicateLocation);
	checkErrorException(nestedServer);
	checkErrorException(endBracket);
	checkErrorException(onlyLocation);
	checkErrorException(noBlock);
	checkErrorException(locationMaj);
	checkErrorException(incorrectRoot);
	checkErrorException(missingSemicolonRoot);
	checkErrorException(missingSemicolonListen);
}
