#include "Parser.hpp"

Parser::Parser():
	_configFile(""),
	_lexer(),
	_currentToken(_lexer.getTokens().begin()),
	_context(NONE)
{}

Parser::Parser(std::string configFile):
	_configFile(configFile),
	_lexer(configFile),
	_currentToken(_lexer.getTokens().begin()),
	_context(NONE)
{
	initArrayParsingFunctions();
	parseTokens();
}

Parser::~Parser()
{
	deleteServers();
}

/******************************************************************************/
/*                                   PARSER                                   */
/******************************************************************************/

void	Parser::parseFile(std::string configFile)
{
	_configFile = configFile;
	_lexer.openFile(configFile);
}

void	Parser::parseRule()
{
	Parser::listOfParsingFunctions::const_iterator	parseFunctIte;
	Token::tokenType								tokenType;

	tokenType = _currentToken->getType();
	parseFunctIte = _parsingFunct.find(tokenType);
	if (parseFunctIte == _parsingFunct.end())
		throwErrorParsing(invalidValueMsg());
	setDirective();
	std::cout << "Parse " << _lexer.printType(_currentToken->getType()) << std::endl;
	(this->*parseFunctIte->second)();
	if (tokenType != KEYWORD_LOCATION)
		expectNextToken(SEMICOLON, invalidNbOfArgumentsMsg());
}

void	Parser::updateContext(t_context currentContext, blockPtr currentBlock)
{
	_context = currentContext;
	_currentBlock = currentBlock;
	if (currentBlock)
		_currentBlock->setContext(currentContext);
}

void	Parser::createNewLocation()
{
	std::cout << std::endl << BLUE << "  >>> New Location <<<" << RESET << std::endl;
	
	blockPtr		newLocation;
	std::string		locationPath;
	blockPtr		serverBlockTmp;

	_directive = "";
	if (!currentBlockIsServer())
		throwErrorParsing("nested Location");
	serverBlockTmp = _currentBlock;
	expectNextToken(PATH, invalidValueMsg());	
	locationPath = _currentToken->getValue();
	newLocation = new Block;
	updateContext(LOCATION, newLocation);
	parseBlock();
	_directive = "";
	if (!(serverBlockTmp->insertLocation(locationPath, newLocation)))
		throwErrorParsing("duplicate location \"" + locationPath + "\"");
	std::cout << std::endl << GREEN << "  *** Suite Server ***" << RESET << std::endl;
	updateContext(SERVER, serverBlockTmp);
}

Parser::blockPtr	Parser::createNewServer()
{
	blockPtr	newServer;

	if (_context != NONE)
		throwErrorParsing("nested Server");
	newServer = new Block;
	updateContext(SERVER, newServer);
	return (newServer);
}

void	Parser::parseBlock()
{
	expectNextToken(BLOCK_START, "expecting '{'");
	while (!reachedEndOfBlock())
	{
		getNextToken();
		parseRule();
	}
	expectNextToken(BLOCK_END, "expecting '}'");
}

void	Parser::parseTokens()
{
	blockPtr	newServer;

	while (!reachedEndOfTokens())
	{
		std::cout << std::endl << GREEN << "  *** New Server ***" << RESET << std::endl;
		expectNextToken(KEYWORD_SERVER, "is not allowed here");
		newServer = createNewServer();
		parseBlock();
		_currentServer = _servers.insert(_servers.end(), newServer);
		updateContext(NONE, NULL);
	}
}


/******************************************************************************/
/*                                PARSING RULES                               */
/******************************************************************************/

/* Context: Server */
void	Parser::parseServerNameRule()
{
	if (!currentBlockIsServer())
		throwErrorParsing(directiveNotAllowedHere());
	while (!reachedEndOfDirective())
	{
		expectNextToken(VALUE, invalidValueMsg());
		_currentBlock->setName(_currentToken->getValue());
	}
}

/* Context: Server, Location */
void	Parser::parseIndexRule()
{
	while (!reachedEndOfDirective())
	{
		expectNextToken(VALUE, invalidValueMsg());
		_currentBlock->setIndex(_currentToken->getValue());
	}
}

/* Context: Server */
void	Parser::parseListenRule()
{
	int	port;

	if (!currentBlockIsServer())
		throwErrorParsing(directiveNotAllowedHere());
	getNextToken();
	switch (_currentToken->getType())
	{
		case VALUE:
			_currentBlock->setHost(_currentToken->getValue());
			if (reachedEndOfDirective())
				break;	
			expectNextToken(COLON, invalidValueMsg());
		case COLON:
			expectNextToken(NUMBER, invalidValueMsg());
		case NUMBER:
			port = atoi(_currentToken->getValue().c_str());
			_currentBlock->setPort(port);
			break;
		default:
			throwErrorParsing(invalidValueMsg());
	}
}

/* Context: Server, Location */
void	Parser::parseRootRule()
{
	std::string	path;

	expectNextToken(PATH, invalidValueMsg());
	path = _currentToken->getValue();
	_currentBlock->setRoot(path);
}

/* Context: Server, Location */
void	Parser::parseAutoindexRule()
{
	expectNextToken(VALUE, invalidValueMsg());
	if (_currentToken->getValue() == "on")
		return (_currentBlock->setAutoindex(true));
	if (_currentToken->getValue() == "off")
		return (_currentBlock->setAutoindex(false));
	throwErrorParsing(invalidValueMsg());
}

/* Context: Server, Location */
void	Parser::parseErrorPageRule()
{
	int				code;
	std::string		page;

	expectNbOfArguments(2);
	expectNextToken(NUMBER, invalidValueMsg());
	code = atoi(_currentToken->getValue().c_str());
	if (code < 100 || code > 600)
		throwErrorParsing("invalid error code");
	expectNextToken(PATH, invalidValueMsg());
	page = _currentToken->getValue();

	if (!_lexer.checkFile(_currentToken->getValue()))
		throwErrorParsing(invalidValueMsg());

	_currentBlock->setErrorPage(code, page);
}

/* Context: Server, Location */
void	Parser::parseMaxBodySizeRule()
{
	size_t	maxSize;

	expectNbOfArguments(1);
	expectNextToken(NUMBER, invalidValueMsg());
	maxSize = atol(_currentToken->getValue().c_str());
	_currentBlock->setClientBodyLimit(maxSize);
}


/* Context: Server, Location */
void	Parser::parseCgiRule()
{
	expectNbOfArguments(2);
	expectNextToken(VALUE, invalidValueMsg());
	_currentBlock->setCgiExt(_currentToken->getValue());
	expectNextToken(PATH, invalidValueMsg());
	_currentBlock->setCgiPath(_currentToken->getValue());
}

/* Context: Location, (Server ???) */
void	Parser::parseRedirectRule()
{
	int				code;
	std::string		uri;

	if (!currentBlockIsLocation())
		throwErrorParsing(directiveNotAllowedHere());
	expectNbOfArguments(2);
	expectNextToken(NUMBER, invalidValueMsg());
	code = atoi(_currentToken->getValue().c_str());

	expectNextToken(PATH, invalidValueMsg());
	uri = _currentToken->getValue();

	_currentBlock->setRedirection(code, uri);
}

/* Context: Location */
void	Parser::parseAllowedMethodRule()
{
	if (!currentBlockIsLocation())
		throwErrorParsing(directiveNotAllowedHere());
	while (!reachedEndOfDirective())
	{
		getNextToken();
		if (!_currentBlock->isAllowedMethod(_currentToken->getValue()))
			throwErrorParsing("invalid value \"" + _currentToken->getValue() + "\"");
	}
}

/* Context: Location */
void	Parser::parseUploadPathRule()
{
	if (!currentBlockIsLocation())
		throwErrorParsing(directiveNotAllowedHere());
	expectNextToken(PATH, invalidValueMsg());
	_currentBlock->setUploadPath(_currentToken->getValue());
}


/******************************************************************************/
/*                                   UTILS                                    */
/******************************************************************************/

bool	Parser::currentBlockIsServer()
{
	return (_context == SERVER);
}

bool	Parser::currentBlockIsLocation()
{
	return (_context == LOCATION);
}

void	Parser::expectNbOfArguments(int expectedNb)
{
	(void)expectedNb;
	int	count;
	Lexer::listOfTokens::const_iterator	ite;

	count = 0;
	for (ite = _currentToken + 1; ite->getType() != SEMICOLON && ite != _lexer.getTokens().end(); ite++)
		count++;
	if (count != expectedNb)
		throwErrorParsing(invalidNbOfArgumentsMsg());	
}

void	Parser::expectNextToken(Token::tokenType expectedType, std::string errorMsg)
{
	if (!getNextToken() || _currentToken->getType() != expectedType)
	{
		std::cout << RED << "value = " + _currentToken->getValue() << RESET << std::endl;
		throwErrorParsing(errorMsg);
	}
}

bool	Parser::reachedEndOfDirective()
{
	Lexer::listOfTokens::const_iterator	ite;

	ite = _currentToken + 1;
	return (!reachedEndOfTokens() && ite != _lexer.getTokens().end() && ite->getType() == SEMICOLON);
}

bool	Parser::reachedEndOfBlock()
{
	Lexer::listOfTokens::const_iterator	ite;

	ite = _currentToken + 1;
	return (!reachedEndOfTokens() && ite != _lexer.getTokens().end() && ite->getType() == BLOCK_END);
}

bool	Parser::getNextToken()
{
	if (!reachedEndOfTokens())
		_currentToken++;
	return (_currentToken != _lexer.getTokens().end());
}

bool	Parser::reachedEndOfTokens()
{
	return (_currentToken + 1 == _lexer.getTokens().end());
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
	_parsingFunct[KEYWORD_LOCATION] = &Parser::createNewLocation;
}

std::string		Parser::getConfigFile() const
{
	return (_configFile);
}

std::string		Parser::getDirective() const
{
	return (_directive);
}

void	Parser::setDirective()
{
	Lexer::listOfTokenTypes::const_iterator		ite;
	Lexer::listOfTokenTypes						tokenTypes;

	tokenTypes = _lexer.getTokenTypes();
	for (ite = tokenTypes.begin(); ite != tokenTypes.end(); ite++)
	{
		if (ite->second == _currentToken->getType())
		{
			_directive = ite->first;
			return;
		}
	}
}

Parser::listOfServers		Parser::getServers()
{
	return (_servers);
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

std::string Parser::directiveNotAllowedHere()
{
	return ("\"" + getDirective() + "\" directive is not allowed here");
}

std::string	Parser::invalidNbOfArgumentsMsg()
{
	// std::cout << RED << "Directive = " + getDirective() << RESET << std::endl;
	return ("invalid number of arguments in \"" + getDirective() + "\" directive");
}

std::string	Parser::invalidValueMsg()
{
	std::string	incorrectValue;

	incorrectValue = "";
	if ((_currentToken + 1) != _lexer.getTokens().end())
		incorrectValue = (_currentToken + 1)->getValue();
	// std::cout << RED << "value = " +  incorrectValue << RESET << std::endl;
	return ("invalid value \"" + incorrectValue + "\"");
}

void	Parser::throwErrorParsing(std::string errorMsg)
{
	std::string message;

	message = "Webserv error: " + errorMsg;
	message += " in " + getConfigFile() + ":" + _currentToken->getLineStr();
	delete _currentBlock;
	throw (std::runtime_error(message));
}

/******************************************************************************/
/*                                   PRINT                                    */
/******************************************************************************/

void	Parser::printArrayParsingFunctions()
{
	Parser::listOfParsingFunctions::iterator	ite;

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

void	Parser::displayServersParams()
{
	listOfServers::const_iterator	currentServer;
	int								count;

	std::cout << "OPENFILE" << std::endl;
	count = 1;
	std::cout << GREY << std::endl << "................................";
	for (currentServer = _servers.begin(); currentServer != _servers.end(); currentServer++, count++)
		(*currentServer)->displayParams(count);
	std::cout << "................................" << RESET << std::endl;
}
