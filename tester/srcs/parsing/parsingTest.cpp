#include "../mainTests.hpp"

/**************************************************************/
/*                 	   	  PARSING TEST                        */
/**************************************************************/

void	checkServer(Webserv& webserv, expectedBlock& expectedServer)
{
	(void)expectedServer;
	Block*	server;

	EXPECT_EQ(webserv.getNbOfServers(), 1);
	server = webserv.getConfigServers()[0];
	EXPECT_EQ(server->getNbOfLocations(), expectedServer.nbOfLocations);
	EXPECT_EQ(server->getServerNames(), expectedServer.serverNames);
	EXPECT_EQ(server->getHost(), expectedServer.host);
	EXPECT_EQ(server->getPort(), expectedServer.port);
	EXPECT_EQ(server->getRoot(), expectedServer.root);
	EXPECT_EQ(server->getIndexes(), expectedServer.indexes);
	EXPECT_EQ(server->getAutoindex(), expectedServer.autoindex);
	EXPECT_EQ(server->getClientBodyLimit(), expectedServer.clientBodyLimit);
	EXPECT_EQ(server->getCgiExt(), expectedServer.cgiExt);
	EXPECT_EQ(server->getCgiPath(), expectedServer.cgiPath);
	EXPECT_EQ(server->getErrorPages(), expectedServer.errorPages);
}

void	checkLocation(Webserv& webserv, expectedBlock& expectedLocation)
{
	Block*	location;

	location = webserv.getConfigServers()[0]->getLocations()[expectedLocation.locationPath];

	EXPECT_EQ(location->getRoot(), expectedLocation.root);
	EXPECT_EQ(location->getIndexes(), expectedLocation.indexes);
	EXPECT_EQ(location->getAutoindex(), expectedLocation.autoindex);
	EXPECT_EQ(location->getClientBodyLimit(), expectedLocation.clientBodyLimit);
	EXPECT_EQ(location->getCgiExt(), expectedLocation.cgiExt);
	EXPECT_EQ(location->getCgiPath(), expectedLocation.cgiPath);
	EXPECT_EQ(location->getErrorPages(), expectedLocation.errorPages);
	EXPECT_EQ(location->getRedirectCode(), expectedLocation.redirectCode);
	EXPECT_EQ(location->getRedirectUri(), expectedLocation.redirectUri);
	EXPECT_EQ(location->isAllowedMethod(GET), expectedLocation.methods[GET]);
	EXPECT_EQ(location->isAllowedMethod(POST), expectedLocation.methods[POST]);
	EXPECT_EQ(location->isAllowedMethod(DELETE), expectedLocation.methods[DELETE]);
	EXPECT_EQ(location->getUploadPath(), expectedLocation.uploadPath);
}

void	checkErrorException(t_invalidFile invalidFile)
{
	try
	{
		invalidFile.webserv.parse(invalidFile.filename);
		FAIL() << "Error " << invalidFile.filename;
	}
	catch(const std::exception& e)
	{
		EXPECT_STREQ(invalidFile.msg.c_str(), e.what());
	}
}

void	checkErrorCase(std::vector<t_invalidFile *> invalidFile)
{
	std::vector<t_invalidFile *>::const_iterator	ite;

	for (ite = invalidFile.begin(); ite != invalidFile.end(); ite++)
		checkErrorException(**ite);
}
