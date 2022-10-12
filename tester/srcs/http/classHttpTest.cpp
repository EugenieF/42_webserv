#include "../mainTests.hpp"
#include "ClientTest.hpp"

/**************************************************************/
/*                       CLASS HTTP TEST                      */
/**************************************************************/


std::string	getDate()
{
	std::time_t	time;
	char		date[100];

	time = std::time(NULL);
	if (!std::strftime(date, sizeof(date), "%a, %d %b %Y %X GMT", std::localtime(&time)))
		throw std::runtime_error("strftime() function failed");
	return(std::string(date));
}

class clientTest: public ::testing::Test
{
	protected:
		Webserv			webserv;
		ClientTest		client;
		std::string		expectedResponse;
		std::string		expectedResponse2;
		std::string		expectedResponse3;
		std::string		expectedResponse4;

	void	SetUp() override
	{
		webserv.parse("testFiles/valid/request.conf");
		client.init("127.0.0.1", 8080);	
	
		expectedResponse = "HTTP/1.1 200 OK\r\n\
Connection: keep-alive\r\n\
Content-Length: 9\r\n\
Content-Type: application/octet-stream\r\n\
Date: " + getDate() + "\r\n\
Server: Cutie Webserv 1.0\r\n\r\n\
server {}\r\n";

		expectedResponse2 = "HTTP/1.1 200 OK\r\n\
Connection: keep-alive\r\n\
Content-Length: 0\r\n\
Content-Type: text/plain\r\n\
Date: " + getDate() + "\r\n\
Server: Cutie Webserv 1.0\r\n\r\n\
server {}\r\n";

		expectedResponse3 = "HTTP/1.1 200 OK\r\n\
Connection: keep-alive\r\n\
Content-Length: 0\r\n\
Content-Type: application/octet-stream\r\n\
Date: " + getDate() + "\r\n\
Server: Cutie Webserv 1.0\r\n\r\n\
server {}\r\n";

		expectedResponse4 = "HTTP/1.1 200 OK\r\n\
Connection: keep-alive\r\n\
Content-Length: 9\r\n\
Content-Type: application/octet-stream\r\n\
Date: " + getDate() + "\r\n\
Server: Cutie Webserv 1.0\r\n\r\n\
server {}\r\n";
	}
};
