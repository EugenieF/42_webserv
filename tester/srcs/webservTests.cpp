#include "mainTests.hpp"

/**************************************************************/
/*                      RUN WEBSERV TEST                      */
/**************************************************************/

/*------------------------  PARSING  ------------------------ */

// TEST_F(parsingSimpleTest, simpleConfig)
// {
// 	std::cout << std::endl << BLUE_B;
// 	std::cout << "                                      ";
// 	std::cout << "           [ PARSING ] " << RESET << std::endl;

// 	checkServer(webservSimple, expectedServer1);
// 	checkLocation(webservSimple, expectedLocation1);
// }

/*---------------------  ERROR PARSING  -------------------- */

// TEST_F(parsingErrorTest, errorFile) //Problem test
// {
// 	checkErrorCase(invalidFile);
// }

// TEST_F(parsingErrorTest, errorBracket)
// {
// 	checkErrorCase(invalidBracket);
// }

// TEST_F(parsingErrorTest, errorSemicolon)
// {
// 	checkErrorCase(invalidSemicolon);
// }

// TEST_F(parsingErrorTest, errorServerBlock)
// {
// 	checkErrorCase(invalidServerBlock);
// }

// TEST_F(parsingErrorTest, errorLocationBlock)
// {
// 	checkErrorCase(invalidLocationBlock);
// }

// TEST_F(parsingErrorTest, errorMethod)
// {
// 	checkErrorCase(invalidMethod);
// }

// TEST_F(parsingErrorTest, errorAutoindex)
// {
// 	checkErrorCase(invalidAutoindex);
// }

// TEST_F(parsingErrorTest, errorRoot)
// {
// 	checkErrorCase(invalidRoot);
// }

// TEST_F(parsingErrorTest, errorCgi)
// {
// 	checkErrorCase(invalidCgi);
// }

// TEST_F(parsingErrorTest, errorErrorPage)
// {
// 	checkErrorCase(invalidErrorPage);
// }

// TEST_F(parsingErrorTest, errorIndex)
// {
// 	checkErrorCase(invalidIndex);
// }

// TEST_F(parsingErrorTest, errorRedirect)
// {
// 	checkErrorCase(invalidRedirect);
// }

// TEST_F(parsingErrorTest, errorListen)
// {
// 	checkErrorCase(invalidListen);
// }

// TEST_F(parsingErrorTest, errorUploadPath)
// {
// 	checkErrorCase(invalidUploadPath);
// }

// TEST_F(parsingErrorTest, errorServerName)
// {
// 	checkErrorCase(invalidServerName);
// }

// TEST_F(parsingErrorTest, errorClientMaxBodySize)
// {
// 	checkErrorCase(invalidClientMaxBodySize);
// }

/*------------------------  REQUEST  ------------------------ */

TEST_F(simpleRequestTest, requestClient)
{
	getRequest(webserv, client, expectedResponse, "GET /valid/onlyServer.conf HTTP/1.1\r\nHOST:   www.example.com \r\nContent-Length: 0  \r\nContent-TYPE: text/plain \r\n\r\n");	
	postRequest(webserv, client, expectedResponse2, "POST /valid/testPost.txt HTTP/1.1\r\nhost: www.example.com \r\ncontent-length: 33\r\ncontent-TYPE: text/plain \r\n\r\nThis is a test for a POST request\r\n");	
	EXPECT_EQ(pathIsAccessible("./testFiles/valid/testPost.txt", true));
	deleteRequest(webserv, client, expectedResponse3, "DELETE /valid/testPost.txt HTTP/1.1\r\nHost:  www.example.com\r\nconTent-lengTh: 0\r\n\r\n");
	EXPECT_EQ(pathIsAccessible("./testFiles/valid/testPost.txt", false));
}

// TEST_F(chunkedRequestTest, requestClient)
// {
// 	chunkedPostRequest(webserv, client, expectedResponse3);	
// }
