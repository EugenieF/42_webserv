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
		t_invalidFile	incorrectExt;
		t_invalidFile	noPermission;
		t_invalidFile	directory;
	
		t_invalidFile	nestedServer;
		t_invalidFile	serverMaj;
		t_invalidFile	noBlock;
	
		t_invalidFile	nestedLocation;
		t_invalidFile	duplicateLocation;
		t_invalidFile	onlyLocation;
		t_invalidFile	locationMaj;
		t_invalidFile	missingLocationPath;

		t_invalidFile	endBracket;
		t_invalidFile	inverseBracket;
		t_invalidFile	doubleBrackets;
		t_invalidFile	unexpectedOpeningBracket;
		t_invalidFile	unexpectedClosingBracket;
		t_invalidFile	multipleClosing;

		t_invalidFile	unexpectedSemicolon;
		t_invalidFile	unexpectedEndSemicolon;
		t_invalidFile	missingSemicolonRoot;
		t_invalidFile	missingSemicolonListen;

		t_invalidFile	unknownMethod;
		t_invalidFile	unknownMethod2;
		t_invalidFile	unknownMethod3;
		t_invalidFile	unknownMethod4;
		t_invalidFile	unknownMethod5;

		t_invalidFile	autoindexArgs;
		t_invalidFile	autoindexNoArgs;
		t_invalidFile	incorrectAutoindexServer;
		t_invalidFile	incorrectAutoindexLocation;

		t_invalidFile	errorRoot;
		t_invalidFile	emptyRoot;
		t_invalidFile	incorrectRoot;

		t_invalidFile	incorrectSize;
		t_invalidFile	incorrectSize2;
		t_invalidFile	incorrectSizeLocation;
		t_invalidFile	multipleSize;
		t_invalidFile	noSize;

		t_invalidFile	invalidErrorPage;
		t_invalidFile	invalidErrorPage2;
		t_invalidFile	invalidErrorPage3;
		t_invalidFile	invalidErrorPage4;
		t_invalidFile	noErrorPage;

		t_invalidFile	cgiArgs;
		t_invalidFile	cgiArgs2;
		t_invalidFile	cgiNoArg;
	
		t_invalidFile	indexNoArgServer;
		t_invalidFile	indexNoArgLocation;

		t_invalidFile	emptyListen;
		t_invalidFile	listenInLocation;
		t_invalidFile	invalidListen;
		t_invalidFile	invalidListen2;
		t_invalidFile	invalidListen3;

		t_invalidFile	emptyRedirect;
		t_invalidFile	invalidRedirect;
		t_invalidFile	invalidRedirect2;
		t_invalidFile	invalidRedirect3;
		t_invalidFile	invalidRedirect4;

		t_invalidFile	emptyServerName;
		t_invalidFile	serverNameInLocation;

		t_invalidFile	emptyUploadPath;
		t_invalidFile	invalidUploadPath;

		std::vector<t_invalidFile *> invalidFile = {&absentFile, &incorrectExt, &noPermission, &directory,
		&nestedServer, &serverMaj, &noBlock,
		&nestedLocation, &duplicateLocation, &onlyLocation, &locationMaj, &missingLocationPath,
		&endBracket, &inverseBracket, &doubleBrackets, &unexpectedOpeningBracket, &unexpectedClosingBracket, &multipleClosing,
		&unexpectedSemicolon, &unexpectedEndSemicolon, &missingSemicolonListen, &missingSemicolonRoot,
		&unknownMethod, &unknownMethod2, &unknownMethod3, &unknownMethod4, &unknownMethod5,
		&autoindexArgs, &autoindexNoArgs, &incorrectAutoindexServer, &incorrectAutoindexLocation,
		&errorRoot, &emptyRoot, &incorrectRoot,
		&incorrectSize, &incorrectSize2, &incorrectSizeLocation, &multipleSize, &noSize,
		&invalidErrorPage, &invalidErrorPage2, &invalidErrorPage3, &invalidErrorPage4, &noErrorPage,
		&cgiArgs, &cgiArgs2, &cgiNoArg,
		&indexNoArgServer, &indexNoArgLocation,
		&emptyListen, &listenInLocation, &invalidListen, &invalidListen2, &invalidListen3,
		&emptyRedirect, &invalidRedirect, &invalidRedirect2, &invalidRedirect3, &invalidRedirect4,
		&emptyServerName, &serverNameInLocation,
		&emptyUploadPath, &invalidUploadPath};

	void	SetUp() override
	{
/* file */
		absentFile.filename = "absent.conf";
		absentFile.msg = "Webserv error: file '" + absentFile.filename + "' not found";

		incorrectExt.filename = "testFiles/invalid/incorrectExt.con";
		incorrectExt.msg = "Webserv error: invalid extension, the configuration file must end with '.conf'";

		noPermission.filename = "testFiles/invalid/noPermission.conf";
		noPermission.msg = "Webserv error: cannot open file '" + noPermission.filename + "'";

		directory.filename = "testFiles/invalid/directory.conf";
		directory.msg = "Webserv error: file '" + directory.filename + "' is a directory";

/* brackets */
		endBracket.filename = "testFiles/invalid/brackets/endBracket.conf";
		endBracket.msg = "Webserv error: unexpected end of file, expecting '}' in " + endBracket.filename + ":9";

		inverseBracket.filename = "testFiles/invalid/brackets/inverseBracket.conf";
		inverseBracket.msg = "Webserv error: unexpected '}' in " + inverseBracket.filename + ":1";

		doubleBrackets.filename = "testFiles/invalid/brackets/doubleBrackets.conf";
		doubleBrackets.msg = "Webserv error: unexpected '{' in " + doubleBrackets.filename + ":1";

		unexpectedOpeningBracket.filename = "testFiles/invalid/brackets/unexpectedOpeningBracket.conf";
		unexpectedOpeningBracket.msg = "Webserv error: unexpected '{' in " + unexpectedOpeningBracket.filename + ":1";
		
		unexpectedClosingBracket.filename = "testFiles/invalid/brackets/unexpectedClosingBracket.conf";
		unexpectedClosingBracket.msg = "Webserv error: unexpected '}' in " + unexpectedClosingBracket.filename + ":1";

		multipleClosing.filename = "testFiles/invalid/brackets/multipleClosing.conf";
		multipleClosing.msg = "Webserv error: unexpected '}' in " + multipleClosing.filename + ":10";

/* semicolon */
		unexpectedSemicolon.filename = "testFiles/invalid/semicolon/unexpectedSemicolon.conf";
		unexpectedSemicolon.msg = "Webserv error: unexpected ';' in " + unexpectedSemicolon.filename + ":1";

		unexpectedEndSemicolon.filename = "testFiles/invalid/semicolon/unexpectedEndSemicolon.conf";
		unexpectedEndSemicolon.msg = "Webserv error: unexpected ';' in " + unexpectedEndSemicolon.filename + ":9";

		missingSemicolonRoot.filename = "testFiles/invalid/semicolon/missingSemicolonRoot.conf";
		missingSemicolonRoot.msg = "Webserv error: invalid number of arguments in 'root' directive in " + missingSemicolonRoot.filename + ":3";

		missingSemicolonListen.filename = "testFiles/invalid/semicolon/missingSemicolonListen.conf";
		missingSemicolonListen.msg = "Webserv error: invalid parameter 'server_name' in " + missingSemicolonListen.filename + ":4";

/* server */
		nestedServer.filename = "testFiles/invalid/server/nestedServer.conf";
		nestedServer.msg = "Webserv error: nested server in " + nestedServer.filename + ":8";

		noBlock.filename = "testFiles/invalid/server/noBlock.conf";
		noBlock.msg = "Webserv error: 'root' directive is not allowed here in " + noBlock.filename + ":1";

		serverMaj.filename = "testFiles/invalid/server/serverMaj.conf";
		serverMaj.msg = "Webserv error: unknown directive 'Server' in " + serverMaj.filename + ":1";

/* location */
		onlyLocation.filename = "testFiles/invalid/location/onlyLocation.conf";
		onlyLocation.msg = "Webserv error: 'location' directive is not allowed here in " + onlyLocation.filename + ":1";

		nestedLocation.filename = "testFiles/invalid/location/nestedLocation.conf";
		nestedLocation.msg = "Webserv error: nested location in " + nestedLocation.filename + ":14";

		duplicateLocation.filename = "testFiles/invalid/location/duplicateLocation.conf";
		duplicateLocation.msg = "Webserv error: duplicate location '/www/' in " + duplicateLocation.filename + ":17";

		locationMaj.filename = "testFiles/invalid/location/locationMaj.conf";
		locationMaj.msg = "Webserv error: unknown directive 'Location' in " + locationMaj.filename + ":10";

		missingLocationPath.filename = "testFiles/invalid/location/missingLocationPath.conf";
		missingLocationPath.msg = "Webserv error: invalid number of arguments in 'location' directive in " + missingLocationPath.filename + ":9";

/* allowed_method */
		unknownMethod.filename = "testFiles/invalid/allowed_method/unknownMethod.conf";
		unknownMethod.msg = "Webserv error: unknown method 'GOT' in 'allowed_method' directive in " + unknownMethod.filename + ":12";
		
		unknownMethod2.filename = "testFiles/invalid/allowed_method/unknownMethod2.conf";
		unknownMethod2.msg = "Webserv error: unknown method 'GE' in 'allowed_method' directive in " + unknownMethod2.filename + ":12";

		unknownMethod3.filename = "testFiles/invalid/allowed_method/unknownMethod3.conf";
		unknownMethod3.msg = "Webserv error: unknown method 'GETT' in 'allowed_method' directive in " + unknownMethod3.filename + ":12";

		unknownMethod4.filename = "testFiles/invalid/allowed_method/unknownMethod4.conf";
		unknownMethod4.msg = "Webserv error: unknown method 'HEAD' in 'allowed_method' directive in " + unknownMethod4.filename + ":12";

		unknownMethod5.filename = "testFiles/invalid/allowed_method/unknownMethod5.conf";
		unknownMethod5.msg = "Webserv error: unknown method 'index' in 'allowed_method' directive in " + unknownMethod5.filename + ":12";

/* root */
		errorRoot.filename = "testFiles/invalid/root/rootArgs.conf";
		errorRoot.msg = "Webserv error: invalid number of arguments in 'root' directive in " + errorRoot.filename + ":3";

		emptyRoot.filename = "testFiles/invalid/root/emptyRoot.conf";
		emptyRoot.msg = "Webserv error: invalid number of arguments in 'root' directive in " + emptyRoot.filename + ":3";

		incorrectRoot.filename = "testFiles/invalid/root/incorrectRoot.conf";
		incorrectRoot.msg = "Webserv error: unknown directive 'rooot' in " + incorrectRoot.filename + ":5";

/* autoindex */
		autoindexArgs.filename = "testFiles/invalid/autoindex/args.conf";
		autoindexArgs.msg = "Webserv error: invalid number of arguments in 'autoindex' directive in " + autoindexArgs.filename + ":8";

		autoindexNoArgs.filename = "testFiles/invalid/autoindex/noArgs.conf";
		autoindexNoArgs.msg = "Webserv error: invalid number of arguments in 'autoindex' directive in " + autoindexNoArgs.filename + ":8";

		incorrectAutoindexServer.filename = "testFiles/invalid/autoindex/incorrectInServer.conf";
		incorrectAutoindexServer.msg = "Webserv error: invalid value 'offf' in 'autoindex' directive in " + incorrectAutoindexServer.filename + ":7";

		incorrectAutoindexLocation.filename = "testFiles/invalid/autoindex/incorrectInLocation.conf";
		incorrectAutoindexLocation.msg = "Webserv error: invalid value 'offf' in 'autoindex' directive in " + incorrectAutoindexLocation.filename + ":12";

/* client_max_body_size */
		incorrectSize.filename = "testFiles/invalid/client_max_body_size/incorrectSize.conf";
		incorrectSize.msg = "Webserv error: invalid value '-600' in 'client_max_body_size' directive in " + incorrectSize.filename + ":9";

		incorrectSize2.filename = "testFiles/invalid/client_max_body_size/incorrectSize2.conf";
		incorrectSize2.msg = "Webserv error: invalid value 'index' in 'client_max_body_size' directive in " + incorrectSize2.filename + ":9";

		incorrectSizeLocation.filename = "testFiles/invalid/client_max_body_size/incorrectSizeLocation.conf";
		incorrectSizeLocation.msg = "Webserv error: invalid value '00001a' in 'client_max_body_size' directive in " + incorrectSizeLocation.filename + ":10";

		multipleSize.filename = "testFiles/invalid/client_max_body_size/multipleSize.conf";
		multipleSize.msg = "Webserv error: invalid number of arguments in 'client_max_body_size' directive in " + multipleSize.filename + ":9";

		noSize.filename = "testFiles/invalid/client_max_body_size/noSize.conf";
		noSize.msg = "Webserv error: invalid number of arguments in 'client_max_body_size' directive in " + noSize.filename + ":10";

/* error_page */
		invalidErrorPage.filename = "testFiles/invalid/error_page/invalidErrorPage.conf";
		invalidErrorPage.msg = "Webserv error: invalid value 'index' in 'error_page' directive in " + invalidErrorPage.filename + ":6";

		invalidErrorPage2.filename = "testFiles/invalid/error_page/invalidErrorPage2.conf";
		invalidErrorPage2.msg = "Webserv error: invalid value '-404' in 'error_page' directive in " + invalidErrorPage2.filename + ":8";

		invalidErrorPage3.filename = "testFiles/invalid/error_page/invalidErrorPage3.conf";
		invalidErrorPage3.msg = "Webserv error: invalid number of arguments in 'error_page' directive in " + invalidErrorPage3.filename + ":18";

		invalidErrorPage4.filename = "testFiles/invalid/error_page/invalidErrorPage4.conf";
		invalidErrorPage4.msg = "Webserv error: invalid value '404' in 'error_page' directive in " + invalidErrorPage4.filename + ":13";

		noErrorPage.filename = "testFiles/invalid/error_page/noErrorPage.conf";
		noErrorPage.msg = "Webserv error: invalid number of arguments in 'error_page' directive in " + noErrorPage.filename + ":13";

/* cgi */
		cgiArgs.filename = "testFiles/invalid/cgi/cgiArgs.conf";
		cgiArgs.msg = "Webserv error: invalid number of arguments in 'cgi' directive in " + cgiArgs.filename + ":14";

		cgiArgs2.filename = "testFiles/invalid/cgi/cgiArgs2.conf";
		cgiArgs2.msg = "Webserv error: invalid number of arguments in 'cgi' directive in " + cgiArgs2.filename + ":13";

		cgiNoArg.filename = "testFiles/invalid/cgi/cgiNoArg.conf";
		cgiNoArg.msg = "Webserv error: invalid number of arguments in 'cgi' directive in " + cgiNoArg.filename + ":10";

/* index */
		indexNoArgServer.filename = "testFiles/invalid/index/indexNoArgServer.conf";
		indexNoArgServer.msg = "Webserv error: invalid number of arguments in 'index' directive in " + indexNoArgServer.filename + ":5";

		indexNoArgLocation.filename = "testFiles/invalid/index/indexNoArgLocation.conf";
		indexNoArgLocation.msg = "Webserv error: invalid number of arguments in 'index' directive in " + indexNoArgLocation.filename + ":11";

/* listen */
		emptyListen.filename = "testFiles/invalid/listen/emptyListen.conf";
		emptyListen.msg = "Webserv error: invalid number of arguments in 'listen' directive in " + emptyListen.filename + ":3";

		listenInLocation.filename = "testFiles/invalid/listen/listenInLocation.conf";
		listenInLocation.msg = "Webserv error: 'listen' directive is not allowed here in " + listenInLocation.filename + ":7";

		invalidListen.filename = "testFiles/invalid/listen/invalidListen.conf";
		invalidListen.msg = "Webserv error: invalid port of the 'listen' directive in " + invalidListen.filename + ":3";

		invalidListen2.filename = "testFiles/invalid/listen/invalidListen2.conf";
		invalidListen2.msg = "Webserv error: invalid parameter '8000' in " + invalidListen2.filename + ":3";

		invalidListen3.filename = "testFiles/invalid/listen/invalidListen3.conf";
		invalidListen3.msg = "Webserv error: invalid parameter ':' in " + invalidListen3.filename + ":3";

/* redirect */
		emptyRedirect.filename = "testFiles/invalid/redirect/emptyRedirect.conf";
		emptyRedirect.msg = "Webserv error: invalid number of arguments in 'redirect' directive in " + emptyRedirect.filename + ":16";

		invalidRedirect.filename = "testFiles/invalid/redirect/invalidRedirect.conf";
		invalidRedirect.msg = "Webserv error: invalid value '-301' in 'redirect' directive in " + invalidRedirect.filename + ":7";

		invalidRedirect2.filename = "testFiles/invalid/redirect/invalidRedirect2.conf";
		invalidRedirect2.msg = "Webserv error: invalid value '/www/redirection.html' in 'redirect' directive in " + invalidRedirect2.filename + ":16";

		invalidRedirect3.filename = "testFiles/invalid/redirect/invalidRedirect3.conf";
		invalidRedirect3.msg = "Webserv error: invalid number of arguments in 'redirect' directive in " + invalidRedirect3.filename + ":16";

		invalidRedirect4.filename = "testFiles/invalid/redirect/invalidRedirect4.conf";
		invalidRedirect4.msg = "Webserv error: invalid number of arguments in 'redirect' directive in " + invalidRedirect4.filename + ":7";

/* server_name */
		emptyServerName.filename = "testFiles/invalid/server_name/emptyServerName.conf";
		emptyServerName.msg = "Webserv error: invalid number of arguments in 'server_name' directive in " + emptyServerName.filename + ":6";

		serverNameInLocation.filename = "testFiles/invalid/server_name/serverNameInLocation.conf";
		serverNameInLocation.msg = "Webserv error: 'server_name' directive is not allowed here in " + serverNameInLocation.filename + ":14";

/* upload_path */
		emptyUploadPath.filename = "testFiles/invalid/upload_path/emptyUploadPath.conf";
		emptyUploadPath.msg = "Webserv error: invalid number of arguments in 'upload_path' directive in " + emptyUploadPath.filename + ":14";

		invalidUploadPath.filename = "testFiles/invalid/upload_path/invalidUploadPath.conf";
		invalidUploadPath.msg = "Webserv error: invalid number of arguments in 'upload_path' directive in " + invalidUploadPath.filename + ":14";

	}
	
};
