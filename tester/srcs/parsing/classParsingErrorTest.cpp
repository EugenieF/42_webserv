#include "../mainTests.hpp"

/**************************************************************/
/*                     CLASS PARSING TEST                     */
/**************************************************************/

typedef struct s_invalidFile
{
	Webserv			webserv;
	std::string		filename;
	std::string		msg;
}	t_invalidFile;

class parsingErrorTest: public ::testing::Test
{
	protected:
/* file */
		t_invalidFile	absentFile;
		t_invalidFile	incorrectExt;
		t_invalidFile	noPermission;
		t_invalidFile	directory;
		std::vector<t_invalidFile *> invalidFile = {&absentFile, &incorrectExt, &noPermission, &directory};

/* brackets */
		t_invalidFile	endBracket;
		t_invalidFile	inverseBracket;
		t_invalidFile	doubleBrackets;
		t_invalidFile	unexpectedOpeningBracket;
		t_invalidFile	unexpectedClosingBracket;
		t_invalidFile	multipleClosing;
		std::vector<t_invalidFile *> invalidBracket = {&endBracket, &inverseBracket, &doubleBrackets,
		&unexpectedOpeningBracket, &unexpectedClosingBracket, &multipleClosing};
		
/* semicolon */
		t_invalidFile	unexpectedSemicolon;
		t_invalidFile	unexpectedEndSemicolon;
		t_invalidFile	missingSemicolonRoot;
		t_invalidFile	missingSemicolonListen;
		std::vector<t_invalidFile *> invalidSemicolon = {&unexpectedSemicolon, &unexpectedEndSemicolon, &missingSemicolonListen, &missingSemicolonRoot};
	
/* server block */
		t_invalidFile	nestedServer;
		t_invalidFile	serverMaj;
		t_invalidFile	noBlock;
		std::vector<t_invalidFile *> invalidServerBlock = {&nestedServer, &serverMaj, &noBlock};
	
/* location block */
		t_invalidFile	nestedLocation;
		t_invalidFile	duplicateLocation;
		t_invalidFile	onlyLocation;
		t_invalidFile	locationMaj;
		t_invalidFile	missingLocationPath;
		std::vector<t_invalidFile *> invalidLocationBlock = {&nestedLocation, &duplicateLocation, &onlyLocation,
		&locationMaj, &missingLocationPath};

/* allowed_method */
		t_invalidFile	noMethod;
		t_invalidFile	method;
		t_invalidFile	method2;
		t_invalidFile	method3;
		t_invalidFile	method4;
		t_invalidFile	method5;
		t_invalidFile	method6;
		std::vector<t_invalidFile *> invalidMethod = {&noMethod, &method, &method2, &method3, &method4, &method5, &method6};

/* autoindex */
		t_invalidFile	noAutoindex;
		t_invalidFile	autoindex;
		t_invalidFile	autoindex2;
		t_invalidFile	autoindex3;
		t_invalidFile	autoindex4;
		t_invalidFile	autoindexServer;
		t_invalidFile	autoindexServer2;
		t_invalidFile	autoindexLocation;
		std::vector<t_invalidFile *> invalidAutoindex = {&noAutoindex, &autoindex, &autoindex2, &autoindex3,
		&autoindex4, &autoindexServer, &autoindexServer2, &autoindexLocation};

/* root */
		t_invalidFile	emptyRoot;
		t_invalidFile	root;
		t_invalidFile	root2;
		t_invalidFile	root3;
		std::vector<t_invalidFile *> invalidRoot = {&emptyRoot, &root, &root3};

/* client_max_body_size */
		t_invalidFile	maxBodySize;
		t_invalidFile	maxBodySize2;
		t_invalidFile	maxBodySize3;
		t_invalidFile	maxBodySizeLocation;
		t_invalidFile	multipleSize;
		t_invalidFile	noSize;
		std::vector<t_invalidFile *> invalidClientMaxBodySize = {&maxBodySize, &maxBodySize2, &maxBodySize3,
		&maxBodySizeLocation, &multipleSize, &noSize};

/* error_page */
		t_invalidFile	errorPage;
		t_invalidFile	errorPage2;
		t_invalidFile	errorPage3;
		t_invalidFile	errorPage4;
		t_invalidFile	errorPage5;
		t_invalidFile	noErrorPage;
		std::vector<t_invalidFile *> invalidErrorPage = {&errorPage, &errorPage2, &errorPage3,
		&errorPage4, &errorPage5, &noErrorPage};

/* cgi */
		t_invalidFile	cgi;
		t_invalidFile	cgi2;
		t_invalidFile	cgi3;
		t_invalidFile	noCgi;
		std::vector<t_invalidFile *> invalidCgi = {&cgi, &cgi2, &cgi3, &noCgi};
	
/* index */
		t_invalidFile	noIndexServer;
		t_invalidFile	noIndexLocation;
		t_invalidFile	index;
		t_invalidFile	index2;
		std::vector<t_invalidFile *> invalidIndex = {&noIndexServer, &noIndexLocation, &index, &index2};

/* listen */
		t_invalidFile	emptyListen;
		t_invalidFile	listenInLocation;
		t_invalidFile	listen;
		t_invalidFile	listen2;
		t_invalidFile	listen3;
		t_invalidFile	listen4;
		t_invalidFile	listen5;
		t_invalidFile	listen6;
		t_invalidFile	listen7;
		std::vector<t_invalidFile *> invalidListen = {&emptyListen, &listenInLocation, &listen, &listen2, &listen3,
		&listen4, &listen5, &listen6, &listen7};

/* redirect */
		t_invalidFile	emptyRedirect;
		t_invalidFile	redirect;
		t_invalidFile	redirect2;
		t_invalidFile	redirect3;
		t_invalidFile	redirect4;
		std::vector<t_invalidFile *> invalidRedirect = {&emptyRedirect, &redirect, &redirect2, &redirect3, &redirect4};

/* server_name */
		t_invalidFile	emptyServerName;
		t_invalidFile	serverName;
		t_invalidFile	serverName2;
		t_invalidFile	serverName3;
		t_invalidFile	serverNameInLocation;
		std::vector<t_invalidFile *> invalidServerName = {&emptyServerName, &serverName, &serverName2,
		&serverName3, &serverNameInLocation};

/* upload_path */
		t_invalidFile	emptyUploadPath;
		t_invalidFile	uploadPath;
		t_invalidFile	uploadPath2;
		std::vector<t_invalidFile *> invalidUploadPath = {&emptyUploadPath, &uploadPath, &uploadPath2};


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
		endBracket.msg = "Webserv error: unexpected end of file in " + endBracket.filename + ":8";

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

/* server block */
		nestedServer.filename = "testFiles/invalid/server/nestedServer.conf";
		nestedServer.msg = "Webserv error: nested server in " + nestedServer.filename + ":8";

		noBlock.filename = "testFiles/invalid/server/noBlock.conf";
		noBlock.msg = "Webserv error: 'root' directive is not allowed here in " + noBlock.filename + ":1";

		serverMaj.filename = "testFiles/invalid/server/serverMaj.conf";
		serverMaj.msg = "Webserv error: unknown directive 'Server' in " + serverMaj.filename + ":1";

/* location block */
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
		noMethod.filename = "testFiles/invalid/allowed_method/noMethod.conf";
		noMethod.msg = "Webserv error: invalid number of arguments in 'allowed_method' directive in " + noMethod.filename + ":12";

		method.filename = "testFiles/invalid/allowed_method/method.conf";
		method.msg = "Webserv error: unknown method 'GOT' in 'allowed_method' directive in " + method.filename + ":12";
		
		method2.filename = "testFiles/invalid/allowed_method/method2.conf";
		method2.msg = "Webserv error: unknown method 'GE' in 'allowed_method' directive in " + method2.filename + ":12";

		method3.filename = "testFiles/invalid/allowed_method/method3.conf";
		method3.msg = "Webserv error: unknown method 'GETT' in 'allowed_method' directive in " + method3.filename + ":12";

		method4.filename = "testFiles/invalid/allowed_method/method4.conf";
		method4.msg = "Webserv error: unknown method 'HEAD' in 'allowed_method' directive in " + method4.filename + ":12";

		method5.filename = "testFiles/invalid/allowed_method/method5.conf";
		method5.msg = "Webserv error: unknown method 'index' in 'allowed_method' directive in " + method5.filename + ":12";

		method6.filename = "testFiles/invalid/allowed_method/method6.conf";
		method6.msg = "Webserv error: unexpected '}' in " + method6.filename + ":11";

/* root */
		emptyRoot.filename = "testFiles/invalid/root/emptyRoot.conf";
		emptyRoot.msg = "Webserv error: invalid number of arguments in 'root' directive in " + emptyRoot.filename + ":3";

		root.filename = "testFiles/invalid/root/root.conf";
		root.msg = "Webserv error: unknown directive 'rooot' in " + root.filename + ":5";

		root2.filename = "testFiles/invalid/root/root2.conf";
		root2.msg = "Webserv error: invalid value 'not_a_path' in 'root' directive in " + root2.filename + ":5";

		root3.filename = "testFiles/invalid/root/root3.conf";
		root3.msg = "Webserv error: invalid number of arguments in 'root' directive in " + root3.filename + ":3";

/* autoindex */
		noAutoindex.filename = "testFiles/invalid/autoindex/noAutoindex.conf";
		noAutoindex.msg = "Webserv error: invalid number of arguments in 'autoindex' directive in " + noAutoindex.filename + ":8";

		autoindex.filename = "testFiles/invalid/autoindex/autoindex.conf";
		autoindex.msg = "Webserv error: unexpected '}' in " + autoindex.filename + ":10";

		autoindex2.filename = "testFiles/invalid/autoindex/autoindex2.conf";
		autoindex2.msg = "Webserv error: unexpected end of file in " + autoindex2.filename + ":12";

		autoindex3.filename = "testFiles/invalid/autoindex/autoindex3.conf";
		autoindex3.msg = "Webserv error: unexpected end of file in " + autoindex3.filename + ":12";

		autoindex4.filename = "testFiles/invalid/autoindex/autoindex4.conf";
		autoindex4.msg = "Webserv error: invalid number of arguments in 'autoindex' directive in " + autoindex4.filename + ":8";

		autoindexServer.filename = "testFiles/invalid/autoindex/autoindexServer.conf";
		autoindexServer.msg = "Webserv error: invalid value 'offf' in 'autoindex' directive in " + autoindexServer.filename + ":16";

		autoindexServer2.filename = "testFiles/invalid/autoindex/autoindexServer2.conf";
		autoindexServer2.msg = "Webserv error: invalid value '15667' in 'autoindex' directive in " + autoindexServer2.filename + ":16";

		autoindexLocation.filename = "testFiles/invalid/autoindex/autoindexLocation.conf";
		autoindexLocation.msg = "Webserv error: invalid value 'offf' in 'autoindex' directive in " + autoindexLocation.filename + ":12";

/* client_max_body_size */
		maxBodySize.filename = "testFiles/invalid/client_max_body_size/maxBodySize.conf";
		maxBodySize.msg = "Webserv error: invalid value '-600' in 'client_max_body_size' directive in " + maxBodySize.filename + ":9";

		maxBodySize2.filename = "testFiles/invalid/client_max_body_size/maxBodySize2.conf";
		maxBodySize2.msg = "Webserv error: invalid value 'index' in 'client_max_body_size' directive in " + maxBodySize2.filename + ":9";

		maxBodySize3.filename = "testFiles/invalid/client_max_body_size/maxBodySize3.conf";
		maxBodySize3.msg = "Webserv error: invalid value '145dB' in 'client_max_body_size' directive in " + maxBodySize3.filename + ":9";

		maxBodySizeLocation.filename = "testFiles/invalid/client_max_body_size/maxBodySizeLocation.conf";
		maxBodySizeLocation.msg = "Webserv error: invalid value '00001a' in 'client_max_body_size' directive in " + maxBodySizeLocation.filename + ":10";

		multipleSize.filename = "testFiles/invalid/client_max_body_size/multipleSize.conf";
		multipleSize.msg = "Webserv error: invalid number of arguments in 'client_max_body_size' directive in " + multipleSize.filename + ":9";

		noSize.filename = "testFiles/invalid/client_max_body_size/noSize.conf";
		noSize.msg = "Webserv error: invalid number of arguments in 'client_max_body_size' directive in " + noSize.filename + ":10";

/* error_page */
		errorPage.filename = "testFiles/invalid/error_page/errorPage.conf";
		errorPage.msg = "Webserv error: invalid value 'index' in 'error_page' directive in " + errorPage.filename + ":6";

		errorPage2.filename = "testFiles/invalid/error_page/errorPage2.conf";
		errorPage2.msg = "Webserv error: invalid value '-404' in 'error_page' directive in " + errorPage2.filename + ":8";

		errorPage3.filename = "testFiles/invalid/error_page/errorPage3.conf";
		errorPage3.msg = "Webserv error: invalid number of arguments in 'error_page' directive in " + errorPage3.filename + ":18";

		errorPage4.filename = "testFiles/invalid/error_page/errorPage4.conf";
		errorPage4.msg = "Webserv error: invalid value '404' in 'error_page' directive in " + errorPage4.filename + ":13";

		errorPage5.filename = "testFiles/invalid/error_page/errorPage5.conf";
		errorPage5.msg = "Webserv error: value '12' must be between 300 and 527 in " + errorPage5.filename + ":13";

		noErrorPage.filename = "testFiles/invalid/error_page/noErrorPage.conf";
		noErrorPage.msg = "Webserv error: invalid number of arguments in 'error_page' directive in " + noErrorPage.filename + ":13";

/* cgi */
		cgi.filename = "testFiles/invalid/cgi/cgi.conf";
		cgi.msg = "Webserv error: invalid number of arguments in 'cgi' directive in " + cgi.filename + ":14";

		cgi2.filename = "testFiles/invalid/cgi/cgi2.conf";
		cgi2.msg = "Webserv error: invalid number of arguments in 'cgi' directive in " + cgi2.filename + ":13";

		cgi3.filename = "testFiles/invalid/cgi/cgi3.conf";
		cgi3.msg = "Webserv error: unexpected '}' in " + cgi3.filename + ":14";

		noCgi.filename = "testFiles/invalid/cgi/noCgi.conf";
		noCgi.msg = "Webserv error: invalid number of arguments in 'cgi' directive in " + noCgi.filename + ":10";

/* index */
		noIndexServer.filename = "testFiles/invalid/index/noIndexServer.conf";
		noIndexServer.msg = "Webserv error: invalid number of arguments in 'index' directive in " + noIndexServer.filename + ":5";

		noIndexLocation.filename = "testFiles/invalid/index/noIndexLocation.conf";
		noIndexLocation.msg = "Webserv error: invalid number of arguments in 'index' directive in " + noIndexLocation.filename + ":10";

		index.filename = "testFiles/invalid/index/index.conf";
		index.msg = "Webserv error: unexpected end of file in " + index.filename + ":6";

		index2.filename = "testFiles/invalid/index/index2.conf";
		index2.msg = "Webserv error: unexpected end of file in " + index2.filename + ":6";

/* listen */
		emptyListen.filename = "testFiles/invalid/listen/emptyListen.conf";
		emptyListen.msg = "Webserv error: invalid number of arguments in 'listen' directive in " + emptyListen.filename + ":3";

		listenInLocation.filename = "testFiles/invalid/listen/listenInLocation.conf";
		listenInLocation.msg = "Webserv error: 'listen' directive is not allowed here in " + listenInLocation.filename + ":7";

		listen.filename = "testFiles/invalid/listen/listen.conf";
		listen.msg = "Webserv error: invalid port of the 'listen' directive in " + listen.filename + ":3";

		listen2.filename = "testFiles/invalid/listen/listen2.conf";
		listen2.msg = "Webserv error: invalid parameter '8000' in " + listen2.filename + ":3";

		listen3.filename = "testFiles/invalid/listen/listen3.conf";
		listen3.msg = "Webserv error: invalid parameter ':' in " + listen3.filename + ":3";

		listen4.filename = "testFiles/invalid/listen/listen4.conf";
		listen4.msg = "Webserv error: invalid port of the 'listen' directive in " + listen4.filename + ":3";

		listen5.filename = "testFiles/invalid/listen/listen5.conf";
		listen5.msg = "Webserv error: invalid parameter ':' in " + listen5.filename + ":3";

		listen6.filename = "testFiles/invalid/listen/listen6.conf";
		listen6.msg = "Webserv error: invalid port of the 'listen' directive in " + listen6.filename + ":3";

		listen7.filename = "testFiles/invalid/listen/listen7.conf";
		listen7.msg = "Webserv error: unexpected '}' in " + listen7.filename + ":4";

/* redirect */
		emptyRedirect.filename = "testFiles/invalid/redirect/emptyRedirect.conf";
		emptyRedirect.msg = "Webserv error: invalid number of arguments in 'redirect' directive in " + emptyRedirect.filename + ":16";

		redirect.filename = "testFiles/invalid/redirect/redirect.conf";
		redirect.msg = "Webserv error: invalid value '-301' in 'redirect' directive in " + redirect.filename + ":10";

		redirect2.filename = "testFiles/invalid/redirect/redirect2.conf";
		redirect2.msg = "Webserv error: invalid value '/www/redirection.html' in 'redirect' directive in " + redirect2.filename + ":16";

		redirect3.filename = "testFiles/invalid/redirect/redirect3.conf";
		redirect3.msg = "Webserv error: invalid number of arguments in 'redirect' directive in " + redirect3.filename + ":16";

		redirect4.filename = "testFiles/invalid/redirect/redirect4.conf";
		redirect4.msg = "Webserv error: 'redirect' directive is not allowed here in " + redirect4.filename + ":7";

/* server_name */
		emptyServerName.filename = "testFiles/invalid/server_name/emptyServerName.conf";
		emptyServerName.msg = "Webserv error: invalid number of arguments in 'server_name' directive in " + emptyServerName.filename + ":6";

		serverName.filename = "testFiles/invalid/server_name/serverName.conf";
		serverName.msg = "Webserv error: unexpected '}' in " + serverName.filename + ":1";

		serverName2.filename = "testFiles/invalid/server_name/serverName2.conf";
		serverName2.msg = "Webserv error: unexpected end of file in " + serverName2.filename + ":3";

		serverName3.filename = "testFiles/invalid/server_name/serverName3.conf";
		serverName3.msg = "Webserv error: unexpected end of file in " + serverName3.filename + ":3";

		serverNameInLocation.filename = "testFiles/invalid/server_name/serverNameInLocation.conf";
		serverNameInLocation.msg = "Webserv error: 'server_name' directive is not allowed here in " + serverNameInLocation.filename + ":14";

/* upload_path */
		emptyUploadPath.filename = "testFiles/invalid/upload_path/emptyUploadPath.conf";
		emptyUploadPath.msg = "Webserv error: invalid number of arguments in 'upload_path' directive in " + emptyUploadPath.filename + ":14";

		uploadPath.filename = "testFiles/invalid/upload_path/uploadPath.conf";
		uploadPath.msg = "Webserv error: invalid number of arguments in 'upload_path' directive in " + uploadPath.filename + ":14";

		uploadPath2.filename = "testFiles/invalid/upload_path/uploadPath2.conf";
		uploadPath2.msg = "Webserv error: 'upload_path' directive is not allowed here in " + uploadPath2.filename + ":17";
	}
	
};
