#include "../mainTests.hpp"

/**************************************************************/
/*                 	   	  PARSING TEST                        */
/**************************************************************/

void	simpleConfig(Webserv& webservSimple)
{
	Parser::listOfServers::const_iterator ite;

	EXPECT_EQ(webservSimple.getNbOfServers(), 3);

	ite = webservSimple.getServers().begin();
	EXPECT_EQ((*ite)->getNbOfLocations(), 2);
	EXPECT_EQ((*ite)->getRoot(), "./www");
	EXPECT_EQ((*ite)->getHost(), "127.0.0.0");
	EXPECT_EQ((*ite)->getPort(), 9000);
}
