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

typedef struct s_invalidFile
{
	Webserv			webserv;
	std::string		filename;
	std::string		msg;
}	t_invalidFile;

class parsingErrorTest: public ::testing::Test
{
	protected:
		t_invalidFile	absentFile;
		t_invalidFile	errorRoot;
		t_invalidFile	nestedLocation;
		t_invalidFile	duplicateLocation;
		t_invalidFile	nestedServer;
		t_invalidFile	endBracket;
		t_invalidFile	onlyLocation;
		t_invalidFile	noBlock;
		t_invalidFile	locationMaj;
		t_invalidFile	incorrectRoot;
		t_invalidFile	missingSemicolonRoot;

	void	SetUp() override
	{
		absentFile.filename = "absent.conf";
		absentFile.msg = "Webserv error: file '" + absentFile.filename + "' not found";

		errorRoot.filename = "testFiles/invalid/rootArgs.conf";
		errorRoot.msg = "Webserv error: invalid number of arguments in 'root' directive in " + errorRoot.filename + ":3";

		nestedLocation.filename = "testFiles/invalid/nestedLocation.conf";
		nestedLocation.msg = "Webserv error: nested location in " + nestedLocation.filename + ":14";

		duplicateLocation.filename = "testFiles/invalid/duplicateLocation.conf";
		duplicateLocation.msg = "Webserv error: duplicate location '/www/' in " + duplicateLocation.filename + ":17";

		nestedServer.filename = "testFiles/invalid/nestedServer.conf";
		nestedServer.msg = "Webserv error: nested server in " + nestedServer.filename + ":8";

		endBracket.filename = "testFiles/invalid/endBracket.conf";
		endBracket.msg = "Webserv error: unexpected end of file, expecting '}' in " + endBracket.filename + ":9";

		onlyLocation.filename = "testFiles/invalid/onlyLocation.conf";
		onlyLocation.msg = "Webserv error: 'location' directive is not allowed here in " + onlyLocation.filename + ":1";

		noBlock.filename = "testFiles/invalid/noBlock.conf";
		noBlock.msg = "Webserv error: 'root' directive is not allowed here in " + noBlock.filename + ":1";

		locationMaj.filename = "testFiles/invalid/locationMaj.conf";
		locationMaj.msg = "Webserv error: unknown directive 'Location' in " + locationMaj.filename + ":10";

		incorrectRoot.filename = "testFiles/invalid/incorrectRoot.conf";
		incorrectRoot.msg = "Webserv error: unknown directive 'rooot' in " + incorrectRoot.filename + ":5";

		missingSemicolonRoot.filename = "testFiles/invalid/missingSemicolonRoot.conf";
		missingSemicolonRoot.msg = "Webserv error: invalid number of arguments in 'root' directive in " + missingSemicolonRoot.filename + ":3";
	}
};
