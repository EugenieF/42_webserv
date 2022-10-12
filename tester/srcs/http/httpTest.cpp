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
		exit(EXIT_SUCCESS);
	}
	else
	{
		client.sendRequest(client.request);
		response = client.readResponse();
	}
	return (response);
}


void	checkResponse(Webserv& webserv, ClientTest& client, const std::string& expectedResponse, const std::string& request)
{
	std::string response;

	client.request = request;
	response = connectToServer(webserv, client);
	// std::cout << BLUE << std::endl << response << std::endl;
	// std::cout << GREEN << std::endl << expectedResponse << RESET << std::endl;
	EXPECT_EQ(response, expectedResponse);
}

void	checkResponseTwoChunks(Webserv& webserv, ClientTest& client, const std::string& expectedResponse,
	const std::string& request1, const std::string& request2)
{
	std::string response;

	client.request = request1;
	response = connectToServer(webserv, client);
	client.request = request2;
	response = connectToServer(webserv, client);
	EXPECT_EQ(response, expectedResponse);
}
