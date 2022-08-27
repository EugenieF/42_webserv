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
	checkErrorException(absentFile, absentFileFilename, absentFileMsg.c_str());
	checkErrorException(errorRoot, errorRootFilename, errorRootMsg.c_str());
	checkErrorException(nestedLocation, nestedLocationFilename, nestedLocationMsg.c_str());
	// checkErrorException(duplicateLocation, duplicateLocationFilename, duplicateLocationMsg.c_str());
}
