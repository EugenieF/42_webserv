#include "../mainTests.hpp"

/**************************************************************/
/*                 	   	    HTTP TEST                         */
/**************************************************************/

std::string		connectToServer(Webserv& webserv, ClientTest& client)
{
	std::string	response;

	pid_t pid = fork();

	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		webserv.run();
		return ;
	}
    client.sendRequest(client.request);
	response = client.readResponse();
	return (response);
}

void	checkErrorRequest(Webserv& webserv, ClientTest& client)
{
	try
	{
		connectToServer(webserv, client);
		FAIL() << "Error " << client.request << std::endl;
	}
	catch(const std::exception& e)
	{
		EXPECT_STREQ(client.expectedResponse.c_str(), e.what());
	}
}

void	checkRequest(Webserv& webserv, ClientTest& client)
{
	std::string response = connectToServer(webserv, client);
	EXPECT_EQ(response, client.expectedResponse);
}
