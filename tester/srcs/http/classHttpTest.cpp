#include "../mainTests.hpp"
#include "ClientTest.hpp"

/**************************************************************/
/*                       CLASS HTTP TEST                      */
/**************************************************************/


class clientTest: public ::testing::Test
{
	protected:
		Webserv		webserv;
		ClientTest	client;

	void	SetUp() override
	{
		webserv.parse("testFiles/valid/basic.conf");
		client.init("127.0.0.1", 8080);
	
		pid_t pid = fork();

		if (pid == -1)
		{
			perror("fork");
  			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
			webserv.run();
		else
		{	
    		client.sendRequest("GET /test/infos.txt HTTP/1.1\r\nHOST: www/ \r\nContent-length: 0\r\nContent-TYPE: text/plain \r\n\r\nHello World!\r\n\r\n");
			client.readResponse();
		}
	}
};
