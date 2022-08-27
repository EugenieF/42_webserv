#include "../mainTests.hpp"

/**************************************************************/
/*                     CLASS PARSING TEST                     */
/**************************************************************/

typedef enum e_directive
{
	AUTOINDEX,
	CGI,
	ERROR_PAGE,
	REDIRECT,
	BODY_LIMIT,
	ALLOWED_METHOD,
	UPLOAD_PATH,
}	t_directive;

struct	expectedBlock
{
	/*** SERVER ONLY ***/
	int							nbOfLocations;
	Block::listOfStrings		serverNames;
	std::string					host;
	int							port;

	/*** BOTH ***/
	std::string					root;
	Block::listOfStrings		indexes;
	bool						autoindex;
	size_t						clientBodyLimit;
	std::string					cgiExt;
	std::string					cgiPath;
	Block::listOfErrorPages		errorPages;

	/*** LOCATION ONLY ***/
	std::string					locationPath;
	int							redirectCode;
	std::string					redirectUri;
	bool						methods[ALLOWED_METHODS_COUNT];
	std::string					uploadPath;
};

void	initServer(expectedBlock* server)
{
	server->nbOfLocations = 0;
	server->port = 8000;
	server->host = "0.0.0.0";
	server->root = DEFAULT_ROOT;
	server->autoindex = false;
	server->clientBodyLimit = DEFAULT_CLIENT_BODY_LIMIT;
	server->cgiExt = "";
	server->cgiPath = "";
}

void	initLocation(expectedBlock* location)
{
	location->root = DEFAULT_ROOT;
	location->autoindex = false;
	location->clientBodyLimit = DEFAULT_CLIENT_BODY_LIMIT;
	location->cgiExt = "";
	location->cgiPath = "";
	location->redirectCode = 0;
	location->redirectUri = "";
	location->uploadPath = "";
	location->methods[GET] = false;
	location->methods[POST] = false;
	location->methods[DELETE] = false;
}

class parsingSimpleTest: public ::testing::Test
{
	protected:
		Webserv					webservSimple;
		expectedBlock			expectedServer1;
		expectedBlock			expectedLocation1;

	void	SetUp() override
	{
		/*******  BASIC  *******/
		webservSimple.parse("testFiles/valid/basic.conf");

		initServer(&expectedServer1);
		expectedServer1.nbOfLocations = 1;
		expectedServer1.serverNames = {"localhost", "efrancon"};
		expectedServer1.host = "127.0.0.0";
		expectedServer1.port = 9000;
		expectedServer1.root = "./www";
		expectedServer1.indexes = {"index.html", "index.htm", "index.php"};
		expectedServer1.autoindex = true;
		expectedServer1.clientBodyLimit = (size_t)600000000;
		expectedServer1.cgiExt = "php";
		expectedServer1.cgiPath = "/usr/bin/php-cgi";
		expectedServer1.errorPages.insert(std::make_pair(404, "./www/404.html"));

		initLocation(&expectedLocation1);
		expectedLocation1.locationPath = "/www/";
		expectedLocation1.indexes = {"index.html", "index.php"};
		expectedLocation1.uploadPath = "./www";
		expectedLocation1.cgiExt = ".py";
		expectedLocation1.cgiPath = "/usr/bin/python3";
		expectedLocation1.redirectCode = 301;
		expectedLocation1.redirectUri = "/www/redirection.html";
		expectedLocation1.methods[POST] = true;
		expectedLocation1.methods[DELETE] = true;
	}
};


class parsingErrorTest: public ::testing::Test
{
	protected:
		Webserv			absentFile;
		std::string		absentFileFilename;
		std::string		absentFileMsg;

		Webserv			errorRoot;
		std::string		errorRootFilename;
		std::string		errorRootMsg;

		Webserv			nestedLocation;
		std::string		nestedLocationFilename;
		std::string		nestedLocationMsg;

		Webserv			duplicateLocation;
		std::string		duplicateLocationFilename;
		std::string		duplicateLocationMsg;

	void	SetUp() override
	{
		absentFileFilename = "absent.conf";
		absentFileMsg = "Webserv error: file '" + absentFileFilename + "' not found";

		errorRootFilename = "testFiles/invalid/rootArgs.conf";
		errorRootMsg = "Webserv error: invalid number of arguments in 'root' directive in " + errorRootFilename + ":3";

		nestedLocationFilename = "testFiles/invalid/nestedLocation.conf";
		nestedLocationMsg = "Webserv error: nested location in " + nestedLocationFilename + ":14";

		duplicateLocationFilename = "testFiles/invalid/duplicateLocation.conf";
		duplicateLocationMsg = "Webserv error: duplicate location '/www/' in " + duplicateLocationFilename + ":17";
	}
};
