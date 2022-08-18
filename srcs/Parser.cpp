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
		throwErrorParsing("invalid value");
	if (!(*_currentServer)->insertLocation(_currentToken->getValue()))
		throwErrorParsing("duplicate location");
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
		(*_currentServer)->setName(_currentToken->getValue());
	}
	expectToken(SEMICOLON, "invalid number of arguments in");
}

void	Parser::parseListenRule()
{
	std::cout << "Parse Listen" << std::endl;

	int	port;
	
	_directive = "listen";
	if (!getNextToken())
		throwErrorParsing("invalid value");
	if (_currentToken->getType() == IP_ADDRESS || _currentToken->getType() == VALUE)
	{
		(*_currentServer)->setHost(_currentToken->getValue());
		if (getNextToken() && _currentToken->getType() == SEMICOLON)
			return ;	
		expectToken(COLON, "invalid value");
		expectNextToken(NUMBER, "invalid value");
	}
	else if (_currentToken->getType() == COLON || _currentToken->getType() == NUMBER)
	{
		if (_currentToken->getType() == COLON)
			expectNextToken(NUMBER, "invalid value");
		port = atoi(_currentToken->getValue().c_str());
		(*_currentServer)->setPort(port);
	}
	else
		throwErrorParsing("invalid value");
	expectNextToken(SEMICOLON, "invalid number of arguments in");
}

void	Parser::parseRootRule()
{
	std::cout << "Parse Root" << std::endl;

	_directive = "root";
	expectNextToken(PATH, "invalid value");
	(*_currentServer)->setRoot(_currentToken->getValue());
	expectNextToken(SEMICOLON, "invalid number of arguments in");
}

void	Parser::parseIndexRule()
{
	std::cout << "Parse Index" << std::endl;

	_directive = "index";
	while (getNextToken() && _currentToken->getType() != SEMICOLON)
	{
		if (_currentToken->getType() != VALUE)
			throwErrorParsing("invalid value");
		(*_currentServer)->setIndex(_currentToken->getValue());
	}
	expectToken(SEMICOLON, "invalid number of arguments in");
}

void	Parser::parseAutoindexRule()
{
	std::cout << "Parse Autoindex" << std::endl;

	_directive = "autoindex";
	expectNextToken(VALUE, "invalid value");
	if (_currentToken->getValue() == "on")
		(*_currentServer)->setAutoindex(true);
	else if (_currentToken->getValue() == "off")
		(*_currentServer)->setAutoindex(false);
	else
		throwErrorParsing("invalid value");
	expectNextToken(SEMICOLON, "invalid number of arguments in");
}

void	Parser::parseErrorPageRule()
{
	std::cout << "Parse ErrorPage" << std::endl;

	int code;

	_directive = "error_page";
	expectNextToken(NUMBER, "invalid value");
	code = atoi(_currentToken->getValue().c_str());
	expectNextToken(PATH, "invalid value");
	if (!_lexer.checkFile(_currentToken->getValue()))
		throwErrorParsing("invalid value");
	(*_currentServer)->setErrorPage(code, _currentToken->getValue());
	expectNextToken(SEMICOLON, "invalid number of arguments in");
}

void	Parser::parseRedirectRule()
{
	std::cout << "Parse Redirect" << std::endl;

	int code;

	_directive = "redirect";
	expectNextToken(NUMBER, "invalid value");
	code = atoi(_currentToken->getValue().c_str());
	expectNextToken(PATH, "invalid value");
	(*_currentServer)->setRedirection(code, _currentToken->getValue());
	expectNextToken(SEMICOLON, "invalid number of arguments in");
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

	std::string	extension;

	_directive = "cgi";
	expectNextToken(VALUE, "invalid value");
	extension = _currentToken->getValue();
	expectNextToken(PATH, "invalid value");
	(*_currentServer)->setCgi(extension, _currentToken->getValue());
	expectNextToken(SEMICOLON, "invalid number of arguments in");
}

void	Parser::parseAllowedMethodRule()
{
	std::cout << "Parse AllowedMethod" << std::endl;

	_directive = "allowed_method";
	while (getNextToken() && _currentToken->getType() != SEMICOLON)
	{
		if (!(*_currentServer)->isAllowedMethod(_currentToken->getValue()))
			throwErrorParsing("invalid value");
	}
	expectToken(SEMICOLON, "invalid number of arguments in");
}

void	Parser::parseUploadPathRule()
{
	std::cout << "Parse Upload Path" << std::endl;

	_directive = "upload_path";
	expectNextToken(PATH, "invalid value");
	(*_currentServer)->setUploadPath(_currentToken->getValue());
	expectNextToken(SEMICOLON, "invalid number of arguments in");
}

		
/******************************************************************************/
/*                                   UTILS                                    */
/******************************************************************************/


// void	expectedTokens()
// {
// 	while (!reachedEndOfTokens() && _currentToken->getType() != SEMICOLON)
// 	{
// 		if (!getNextToken() || _currentToken->getType() != expectedType)
// 			throwErrorParsing("invalid argument");
// 	}
// }

void	Parser::expectNextToken(Token::tokenType expectedType, std::string errorMsg)
{
	(void)errorMsg;
	if (!getNextToken() || _currentToken->getType() != expectedType)
		throwErrorParsing("invalid value");
}

void	Parser::expectToken(Token::tokenType expectedType, std::string errorMsg)
{
	(void)errorMsg;
	if (reachedEndOfTokens() || _currentToken->getType() != expectedType)
		throwErrorParsing("invalid value");
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

std::string		Parser::getDirective() const
{
	return (_directive);
}

/******************************************************************************/
/*                                   ERROR                                    */
/******************************************************************************/

// Should we display the first error only or all errors occured ???

/* unknown directive "Root" in nginx.conf:8 */
/* invalid number of arguments in "root" directive in nginx.conf:8 */
/* invalid number of arguments in "error_page" directive in nginx.conf:12 */
/* "server" directive is not allowed here in nginx.conf:10 */
/* invalid port in "0.0.0.0:" of the "listen" directive in nginx.conf:4 */
/* invalid port in "8000000" of the "listen" directive in nginx.conf:4 */
/* "client_max_body_size" directive invalid value in nginx.conf:10 */

void	Parser::throwErrorParsing(std::string errorMsg)
{
	std::string message;

	message = "Webserv error: \"" + getDirective() + "\" directive " + errorMsg 
		+ " in "+ getConfigFile() + ":" + _currentToken->getLineStr();
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
