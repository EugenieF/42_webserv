#include "../mainTests.hpp"

/**************************************************************/
/*                 	   	  PARSING TEST                        */
/**************************************************************/

void	simpleConfig(Webserv webservSimple)
{
	EXPECT_EQ(webservSimple.getNbOfServers(), (3));
}
