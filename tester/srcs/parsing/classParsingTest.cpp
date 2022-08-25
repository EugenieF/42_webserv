#include "../mainTests.hpp"

/**************************************************************/
/*                     CLASS PARSING TEST                     */
/**************************************************************/

class parsingTest: public ::testing::Test
{
	protected:
		Webserv	webservSimple;

	void	SetUp() override
	{
		webservSimple.parse("testFiles/basic.conf");
	}
};
