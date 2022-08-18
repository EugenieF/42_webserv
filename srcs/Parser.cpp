#include "Parser.hpp"

Parser::Parser(std::string configFile):
	_configFile(configFile),
	_lexer(configFile),
	_currentToken(_lexer.getTokens().begin()),
	_context(SERVER)
{
	initArrayParsingFunctions();
	printTokens();
}

Parser::~Parser()
{
	deleteServers();
}

/******************************************************************************/
/*                                   PARSER                                   */
/******************************************************************************/

void	Parser::parseRule()
{
	Parser::listOfParsingFunctions::const_iterator	parseFunctIte;
	Token::tokenType	tokenType;

	tokenType = _currentToken->getType();
	parseFunctIte = _parsingFunct.find(tokenType);
	if (parseFunctIte == _parsingFunct.end())
		throwErrorParsing("invalid rule");
	(this->*parseFunctIte->second)();
	if (tokenType != KEYWORD_LOCATION)
		checkDelimiter(SEMICOLON, "invalid delimiter");
}

void	Parser::parseBlock(t_context blockContext)
{
	checkDelimiter(BLOCK_START, "server opening bracket");
	while (!reachedEndOfTokens() && _currentToken->getType() != BLOCK_END)
	{
		_context = blockContext;
		parseRule();
	}
	checkDelimiter(BLOCK_END, "server closing bracket");
}

void	Parser::parseLocation()
{
	std::cout << std::endl << BLUE << "  >>> New Location <<<" << RESET << std::endl;

	getNextToken();	
	if (_context != SERVER || _currentToken->getType() != PATH)
		throwErrorParsing("location rule");
	getNextToken();
	parseBlock(LOCATION);
	std::cout << BLUE << "  >>> End Location <<<" << RESET << std::endl << std::endl;
}

void	Parser::parseServer()
{
	parseBlock(SERVER);
}

void	Parser::parseTokens()
{
	while (!reachedEndOfTokens() && _currentToken->getType() == KEYWORD_SERVER)
	{
		std::cout << std::endl << GREEN << "  *** New Server ***" << RESET << std::endl;
		_currentServer = _servers.insert(_servers.end(), new Server());
		getNextToken();
		parseServer();
	}
	if (!reachedEndOfTokens())
		throwErrorParsing("server rule");
	std::cout << std::endl << "----  END PARSING  ----" << std::endl;
}


/******************************************************************************/
/*                                PARSING RULES                               */
/******************************************************************************/

void	Parser::parseServerNameRule()
{
	std::cout << "Parse Server Name" << std::endl;

	_directive = "server name";
	while (getNextToken() && _currentToken->getType() != SEMICOLON)
	{
		if (_currentToken->getType() != VALUE)
			throwErrorParsing("index rule");
		(*_currentServer)->setServerName(_currentToken->getValue());
	}
}

void	Parser::parseListenRule()
{
	std::cout << "Parse Listen" << std::endl;

	int	port;
	
	_directive = "listen";
	if (!getNextToken())
		throwErrorParsing("listen rule");
	if (_currentToken->getType() == IP_ADDRESS || _currentToken->getType() == VALUE)
	{
		(*_currentServer)->setHost(_currentToken->getValue());
		if (getNextToken() && _currentToken->getType() == SEMICOLON)
			return ;	
		expectToken(COLON, "listen rule");
		expectNextToken(NUMBER, "listen rule");
	}
	else if (_currentToken->getType() == COLON || _currentToken->getType() == NUMBER)
	{
		if (_currentToken->getType() == COLON)
			expectNextToken(NUMBER, "listen rule");
		port = atoi(_currentToken->getValue().c_str());
		(*_currentServer)->setPort(port);
	}
	else
		throwErrorParsing("listen rule");
	expectNextToken(SEMICOLON, "listen rule");
}

void	Parser::parseRootRule()
{
	std::cout << "Parse Root" << std::endl;

	_directive = "root";
	expectNextToken(PATH, "root rule");
	(*_currentServer)->setRoot(_currentToken->getValue());
	expectNextToken(SEMICOLON, "root rule");
}

void	Parser::parseIndexRule()
{
	std::cout << "Parse Index" << std::endl;

	_directive = "index";
	while (getNextToken() && _currentToken->getType() != SEMICOLON)
	{
		if (_currentToken->getType() != VALUE)
			throwErrorParsing("index rule");
		(*_currentServer)->setIndex(_currentToken->getValue());
	}
}

void	Parser::parseAutoindexRule()
{
	std::cout << "Parse Autoindex" << std::endl;

	_directive = "autoindex";
	expectNextToken(VALUE, "autoindex rule");
	if (_currentToken->getValue() == "on")
		(*_currentServer)->setAutoindex(true);
	else if (_currentToken->getValue() == "off")
		(*_currentServer)->setAutoindex(false);
	else
		throwErrorParsing("autoindex rule");
	expectNextToken(SEMICOLON, "autoindex rule");
}

void	Parser::parseMaxBodySizeRule()
{
	std::cout << "Parse MaxBodySize" << std::endl;

	_directive = "client_max_body_size";
	while (!reachedEndOfTokens() && _currentToken->getType() != SEMICOLON)
		getNextToken();

	// if (!getNextToken() || !(_currentToken->getType() == SIZE || _currentToken->getType() == NUMBER))
	// 	throwErrorParsing("client_max_body_size rule");
	// (*_currentServer)->setClientBodyLimit(_currentToken->getValue());
	// if (!getNextToken() || _currentToken->getType() != SEMICOLON)
	// 	throwErrorParsing("client_max_body_size rule");
}

void	Parser::parseCgiRule()
{
	std::cout << "Parse Cgi" << std::endl;

	_directive = "cgi";
	while (!reachedEndOfTokens() && _currentToken->getType() != SEMICOLON)
		getNextToken();
}

void	Parser::parseErrorPageRule()
{
	std::cout << "Parse ErrorPage" << std::endl;

	int code;

	_directive = "error_page";
	expectNextToken(NUMBER, "error_page rule");
	code = atoi(_currentToken->getValue().c_str());
	expectNextToken(PATH, "error_page rule");
	(*_currentServer)->setErrorPage(code, _currentToken->getValue());
	expectNextToken(SEMICOLON, "error_page rule");
}

void	Parser::parseRedirectRule()
{
	std::cout << "Parse Redirect" << std::endl;

	_directive = "redirect";
	while (!reachedEndOfTokens() && _currentToken->getType() != SEMICOLON)
		getNextToken();
}

void	Parser::parseAllowedMethodRule()
{
	std::cout << "Parse AllowedMethod" << std::endl;

	_directive = "allowed_method";
	while (!reachedEndOfTokens() && _currentToken->getType() != SEMICOLON)
		getNextToken();
}

void	Parser::parseUploadPathRule()
{
	std::cout << "Parse Upload Path" << std::endl;

	_directive = "upload_path";
	while (!reachedEndOfTokens() && _currentToken->getType() != SEMICOLON)
		getNextToken();
}

		
/******************************************************************************/
/*                                   UTILS                                    */
/******************************************************************************/

void	Parser::expectNextToken(Token::tokenType expectedType, std::string errorMsg)
{
	if (!getNextToken() || _currentToken->getType() != expectedType)
		throwErrorParsing(errorMsg);
}

void	Parser::expectToken(Token::tokenType expectedType, std::string errorMsg)
{
	if (reachedEndOfTokens() || _currentToken->getType() != expectedType)
		throwErrorParsing(errorMsg);
}

bool	Parser::getNextToken()
{
	if (!reachedEndOfTokens())
		_currentToken++;
	return (_currentToken != _lexer.getTokens().end());
}

bool	Parser::reachedEndOfTokens()
{
	return (_currentToken == _lexer.getTokens().end());
}

void	Parser::checkDelimiter(Token::tokenType tokenType, std::string errorMsg)
{
	if (reachedEndOfTokens() || _currentToken->getType() != tokenType)
		throwErrorParsing(errorMsg);
	getNextToken();
}

void	Parser::deleteServers()
{
	for (_currentServer = _servers.begin(); _currentServer != _servers.end(); _currentServer++)
	{
		std::cout << RED << " xxx Delete a server xxx" << RESET << std::endl;
		delete (*_currentServer);
	}
}

void	Parser::initArrayParsingFunctions()
{
	_parsingFunct[KEYWORD_SERVER_NAME] = &Parser::parseServerNameRule;
	_parsingFunct[KEYWORD_LISTEN] = &Parser::parseListenRule;
	_parsingFunct[KEYWORD_ROOT] = &Parser::parseRootRule;
	_parsingFunct[KEYWORD_INDEX] = &Parser::parseIndexRule;
	_parsingFunct[KEYWORD_AUTOINDEX] = &Parser::parseAutoindexRule;
	_parsingFunct[KEYWORD_BODY_LIMIT] = &Parser::parseMaxBodySizeRule;
	_parsingFunct[KEYWORD_CGI] = &Parser::parseCgiRule;
	_parsingFunct[KEYWORD_ERROR_PAGE] = &Parser::parseErrorPageRule;
	_parsingFunct[KEYWORD_REDIRECT] = &Parser::parseRedirectRule;
	_parsingFunct[KEYWORD_ALLOWED_METHOD] = &Parser::parseAllowedMethodRule;
	_parsingFunct[KEYWORD_UPLOAD_PATH] = &Parser::parseUploadPathRule;
	_parsingFunct[KEYWORD_LOCATION] = &Parser::parseLocation;
}

std::string		Parser::getConfigFile() const
{
	return (_configFile);
}

/******************************************************************************/
/*                                   ERROR                                    */
/******************************************************************************/

// Should we display the first error only or all errors occured ???

/* unknown directive "Root" in nginx.conf:8 */
/* invalid number of arguments in "root" directive in nginx.conf:8 */
/* "server" directive is not allowed here in nginx.conf:10 */
/* invalid port in "0.0.0.0:" of the "listen" directive in nginx.conf:4 */
/* invalid port in "8000000" of the "listen" directive in nginx.conf:4 */
/* "client_max_body_size" directive invalid value in nginx.conf:10 */

void	Parser::throwErrorParsing(std::string errorMsg)
{
	std::string message;

	message = "Webserv error: " + errorMsg + " in " + getConfigFile() + ":" + _currentToken->getLineStr();
	throw (std::runtime_error(message));
}

/******************************************************************************/
/*                                   PRINT                                    */
/******************************************************************************/

void	Parser::printArrayParsingFunctions()
{
	Parser::listOfParsingFunctions::iterator	ite;

	std::cout << "Print array Parsing functions :" << std::endl;
	for (ite = _parsingFunct.begin(); ite != _parsingFunct.end(); ite++)
		(this->*ite->second)();
}

void	Parser::printCurrentToken()
{
	std::cout << YELLOW << "Current Token : [";
	std::cout << _lexer.printType(_currentToken->getType()) << " : ";
	std::cout << _currentToken->getValue() << "]" << RESET << std::endl;
}

void	Parser::printTokens()
{
	Lexer::listOfTokens::const_iterator		ite;

	std::cout << std::endl  << "  >>> PARSER | List of tokens <<< " << std::endl;
	for (ite = _lexer.getTokens().begin(); ite != _lexer.getTokens().end(); ite++)
	{
		std::cout << "[" << _lexer.printType(ite->getType());
		std::cout << " : " << ite->getValue() << "]";
		std::cout << " : " << ite->getLineStr() << "]" << std::endl;
	}
}
