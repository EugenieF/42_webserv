#include "../mainTests.hpp"

/**************************************************************/
/*                     CLASS PARSING TEST                     */
/**************************************************************/

class parsingTest: public ::testing::Test
{
	protected:
		std::string simpleConfig = "../testFiles/basic.conf";
		Webserv	webservSimple(std::string simpleConfig);

};
