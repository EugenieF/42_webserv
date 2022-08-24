#include "../mainTests.hpp"

/**************************************************************/
/*                     CLASS WEBSERV TEST                     */
/**************************************************************/

class ftStackTest: public ::testing::Test, public globalVarTest
{
	protected:
		Webserv	webserv();

		void	SetUp() override
		{

		}	
};
