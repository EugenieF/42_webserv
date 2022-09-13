#include "Parser.hpp"

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/

Parser::Parser():
	_lexer(),
	_currentBlock(NULL),
	_context(NONE)
{
	_initArrayParsingFunctions();
}

Parser::Parser(std::string configFile):
	_configFile(configFile),
	_lexer(configFile),
	_currentToken(_lexer.getTokens().begin()),
	_currentBlock(NULL),
	_context(NONE)
{
	_initArrayParsingFunctions();
	// printTokens();
	parseTokens();
}

Parser::Parser(const Parser& other):
	_configFile(other.getConfigFile()),
	_lexer(other.getLexer()),
	_currentToken(other.getCurrentToken()),
	_currentServer(other.getCurrentServer()),
	_currentBlock(other.getCurrentBlock()),
	_parsingFunct(other.getParsingFunct()),
	_context(other.getContext()),
	_directive(other.getDirective())
{
	*this = other;
}

Parser::~Parser()
{
	_deleteServers();
}

Parser&		Parser::operator=(const Parser& other)
{
	if (this != &other)
	{
		_configFile = other.getConfigFile();
		_lexer = other.getLexer();
		_currentToken = other.getCurrentToken();
		_currentServer = other.getCurrentServer();
		_currentBlock = other.getCurrentBlock();
		_parsingFunct = other.getParsingFunct();
		_context = other.getContext();
		_directive = other.getDirective();
	}
	return (*this);
}

/******************************************************************************/
/*                                   PARSER                                   */
/******************************************************************************/

void	Parser::parseFile(std::string configFile)
{
	_configFile = configFile;
	_lexer.openFile(configFile);
	_currentToken = _lexer.getTokens().begin();
	parseTokens();
}

void	Parser::_parseRule()
{
	Parser::listOfParsingFunctions::const_iterator	parseFunctIte;
	Token::tokenType								tokenType;

	tokenType = _currentToken->getType();
	if (tokenType == KEYWORD_SERVER)
		_throwErrorParsing("nested server");
	if (_tokenIsDelimiter(tokenType))
		_throwErrorParsing(_unexpectedValueMsg(_currentToken));
	parseFunctIte = _parsingFunct.find(tokenType);
	if (parseFunctIte == _parsingFunct.end())
		_throwErrorParsing(_unknownDirectiveMsg(_currentToken));
	_setDirective();
	(this->*parseFunctIte->second)();
	if (tokenType != KEYWORD_LOCATION && tokenType != KEYWORD_LISTEN)
		_expectNextToken(SEMICOLON, _invalidNbOfArgumentsMsg());
}

void	Parser::_updateContext(t_context currentContext, blockPtr currentBlock)
{
	_context = currentContext;
	_currentBlock = currentBlock;
	if (currentBlock)
		_currentBlock->setContext(currentContext);
}

void	Parser::_createNewLocation()
{
	blockPtr		newLocation;
	std::string		locationPath;
	blockPtr		serverBlockTmp;
	std::string		lineNbrLocation;

	_directive = "location";
	if (!_currentBlockIsServer())
		_throwErrorParsing("nested location");
	lineNbrLocation	= _currentToken->getLineStr();
	serverBlockTmp = _currentBlock;

	// _expectNextToken(PATH, _invalidValueMsg(_currentToken + 1));
	_getNextToken();
	if (_currentToken->getType() == BLOCK_START)
		_throwErrorParsing(_invalidNbOfArgumentsMsg());
	if (_tokenIsDelimiter(_currentToken->getType()))
		_throwErrorParsing(_unexpectedValueMsg(_currentToken));

	locationPath = _currentToken->getValue();
	newLocation = new Block;
	_updateContext(LOCATION, newLocation);
	_parseBlock();
	_directive = "";
	if (!(serverBlockTmp->insertLocation(locationPath, newLocation)))
		_throwErrorParsing("duplicate location '" + locationPath + "'", lineNbrLocation);
	_updateContext(SERVER, serverBlockTmp);
}

Parser::blockPtr	Parser::_createNewServer()
{
	blockPtr	newServer;

	if (_context != NONE)
		_throwErrorParsing("nested server");
	newServer = new Block;
	_updateContext(SERVER, newServer);
	return (newServer);
}

void	Parser::_parseBlock()
{
	_expectNextToken(BLOCK_START, _unexpectedValueMsg(_currentToken + 1));
	while (!_reachedEndOfBlock())
	{
		_getNextToken();
		_parseRule();
	}
	_expectNextToken(BLOCK_END, _unexpectedValueMsg(_currentToken + 1));
}

void	Parser::parseTokens()
{
	blockPtr	newServer;

	while (!_reachedEndOfTokens())
	{
		_expectNextToken(KEYWORD_SERVER, _keywordServerError());
		newServer = _createNewServer();
		_parseBlock();
		_currentServer = _servers.insert(_servers.end(), newServer);
		_updateContext(NONE, NULL);
	}
}

/******************************************************************************/
/*                                PARSING RULES                               */
/******************************************************************************/

/* Context: Server */
void	Parser::_parseServerNameRule()
{
	if (!_currentBlockIsServer())
		_throwErrorParsing(_directiveNotAllowedHereMsg());
	_expectMinimumNbOfArguments(1);
	while (!_reachedEndOfDirective())
	{
		_expectNextToken(VALUE, _invalidValueMsg(_currentToken + 1));
		_currentBlock->setName(_currentToken->getValue());
	}
}

/* Context: Server */
void	Parser::_parseListenRule()
{
	int	port;

	if (!_currentBlockIsServer())
		_throwErrorParsing(_directiveNotAllowedHereMsg());
	_getNextToken();
	switch (_currentToken->getType())
	{
		case VALUE:
			_checkHost(_currentToken->getValue());
			_currentBlock->setHost(_currentToken->getValue());
			if (_reachedEndOfDirective())
				break;	
			_expectNextToken(COLON, _invalidParameterMsg());
		case COLON:
			_expectNextToken(NUMBER, _invalidPortMsg());
		case NUMBER:
			port = atoi(_currentToken->getValue().c_str());
			_currentBlock->setPort(port);
			break;
		case SEMICOLON:
			_throwErrorParsing(_invalidNbOfArgumentsMsg());
		default:
			_throwErrorParsing(_invalidValueMsg(_currentToken + 1));
	}
	_expectNextToken(SEMICOLON, _invalidParameterMsg());
}

/* Context: Server, Location */
void	Parser::_parseIndexRule()
{
	_expectMinimumNbOfArguments(1);
	while (!_reachedEndOfDirective())
	{
		_expectNextToken(VALUE, _invalidValueMsg(_currentToken + 1));
		_currentBlock->setIndex(_currentToken->getValue());
	}
}

/* Context: Server, Location */
void	Parser::_parseRootRule()
{
	std::string	path;

	_expectNbOfArguments(1);
	_expectNextToken(VALUE, _invalidValueMsg(_currentToken + 1));
	path = _currentToken->getValue();
	_currentBlock->setRoot(path);
}

/* Context: Server, Location */
void	Parser::_parseAutoindexRule()
{
	_expectNbOfArguments(1);
	_getNextToken();
	if (_currentToken->getValue() == "on")
		return (_currentBlock->setAutoindex(true));
	if (_currentToken->getValue() == "off")
		return (_currentBlock->setAutoindex(false));
	_throwErrorParsing(_invalidValueMsg(_currentToken));
}

/* Context: Server, Location */
void	Parser::_parseErrorPageRule()
{
	int				code;
	std::string		page;

	_expectNbOfArguments(2);
	_expectNextToken(NUMBER, _invalidValueMsg(_currentToken + 1));
	code = atoi(_currentToken->getValue().c_str());
	if (code < 300 || code > 527)
		_throwErrorParsing(_invalidErrorCodeMsg(_currentToken));
	_expectNextToken(VALUE, _invalidValueMsg(_currentToken + 1));
	page = _currentToken->getValue();
	_currentBlock->setErrorPage(code, page);
}

/* Context: Server, Location */
void	Parser::_parseMaxBodySizeRule()
{
	size_t	maxSize;

	_expectNbOfArguments(1);
	_expectNextToken(NUMBER, _invalidValueMsg(_currentToken + 1));
	maxSize = atol(_currentToken->getValue().c_str());
	_currentBlock->setClientBodyLimit(maxSize);
}

/* Context: Server, Location */
void	Parser::_parseCgiRule()
{
	_expectNbOfArguments(2);
	_expectNextToken(VALUE, _invalidValueMsg(_currentToken + 1));
	_currentBlock->setCgiExt(_currentToken->getValue());
	_expectNextToken(VALUE, _invalidValueMsg(_currentToken + 1));
	_currentBlock->setCgiPath(_currentToken->getValue());
}

/* Context: Location */
void	Parser::_parseRedirectRule()
{
	int				code;
	std::string		uri;

	if (!_currentBlockIsLocation())
		_throwErrorParsing(_directiveNotAllowedHereMsg());
	_expectNbOfArguments(2);
	_expectNextToken(NUMBER, _invalidValueMsg(_currentToken + 1));
	code = atoi(_currentToken->getValue().c_str());
	_expectNextToken(VALUE, _invalidValueMsg(_currentToken + 1));
	uri = _currentToken->getValue();
	_currentBlock->setRedirection(code, uri);
}

/* Context: Location */
void	Parser::_parseAllowedMethodRule()
{
	if (!_currentBlockIsLocation())
		_throwErrorParsing(_directiveNotAllowedHereMsg());
	while (!_reachedEndOfDirective())
	{
		_getNextToken();
		if (!_currentBlock->isAllowedMethod(_currentToken->getValue()))
			_throwErrorParsing(_unknownMethodMsg(_currentToken));
		_currentBlock->setMethod(_currentToken->getValue());
	}
}

/* Context: Location */
void	Parser::_parseUploadPathRule()
{
	if (!_currentBlockIsLocation())
		_throwErrorParsing(_directiveNotAllowedHereMsg());
	_expectNbOfArguments(1);
	_expectNextToken(VALUE, _invalidValueMsg(_currentToken + 1));
	// _getNextToken();
	// if (_tokenIsDelimiter(_currentToken->getType()))
	// 	_throwErrorParsing(_unexpectedValueMsg(_currentToken));
	_currentBlock->setUploadPath(_currentToken->getValue());
}

/******************************************************************************/
/*                                   UTILS                                    */
/******************************************************************************/

bool	Parser::_currentBlockIsServer()
{
	return (_context == SERVER);
}

bool	Parser::_currentBlockIsLocation()
{
	return (_context == LOCATION);
}

void	Parser::_expectNbOfArguments(int expectedNb)
{
	int									count;
	Lexer::listOfTokens::const_iterator	ite;

	count = 0;
	for (ite = _currentToken + 1; ite->getType() != SEMICOLON && ite != _lexer.getTokens().end(); ite++)
	{
		if (_tokenIsDelimiter(ite->getType()))
			_throwErrorParsing(_unexpectedValueMsg(ite), ite->getLineStr());
		count++;
	}
	if (count != expectedNb)
	{
		if (ite == _lexer.getTokens().end())
			ite--;
		_throwErrorParsing(_invalidNbOfArgumentsMsg(), ite->getLineStr());	
	}
}

void	Parser::_expectMinimumNbOfArguments(int expectedNb)
{
	int									count;
	Lexer::listOfTokens::const_iterator	ite;

	count = 0;
	for (ite = _currentToken + 1; ite->getType() != SEMICOLON && ite != _lexer.getTokens().end(); ite++)
	{
		if (_tokenIsDelimiter(ite->getType()))
			_throwErrorParsing(_unexpectedValueMsg(ite), ite->getLineStr());
		count++;
	}
	if (count < expectedNb)
		_throwErrorParsing(_invalidNbOfArgumentsMsg());	
}

void	Parser::_expectNextToken(Token::tokenType expectedType, std::string errorMsg)
{
	if (!_getNextToken() || _currentToken->getType() != expectedType)
		_throwErrorParsing(errorMsg);
}

bool	Parser::_reachedEndOfDirective()
{
	Lexer::listOfTokens::const_iterator	ite;

	ite = _currentToken + 1;
	return (!_reachedEndOfTokens() && ite != _lexer.getTokens().end() && ite->getType() == SEMICOLON);
}

bool	Parser::_reachedEndOfBlock()
{
	Lexer::listOfTokens::const_iterator	ite;

	ite = _currentToken + 1;
	if (_reachedEndOfTokens())
		_throwErrorParsing(_unexpectedValueMsg(_currentToken + 1), _currentToken->getLineStr(1));
	return (ite != _lexer.getTokens().end() && ite->getType() == BLOCK_END);
}

bool	Parser::_getNextToken()
{
	if (!_reachedEndOfTokens())
		_currentToken++;
	return (_currentToken != _lexer.getTokens().end());
}

bool	Parser::_reachedEndOfTokens()
{
	return (_currentToken != _lexer.getTokens().end() && _currentToken + 1 == _lexer.getTokens().end());
}

void	Parser::_deleteServers()
{
	for (_currentServer = _servers.begin(); _currentServer != _servers.end(); _currentServer++)
	{
		if (*_currentServer)
			delete (*_currentServer);
	}
}

void	Parser::_initArrayParsingFunctions()
{
	_parsingFunct[KEYWORD_SERVER_NAME] = &Parser::_parseServerNameRule;
	_parsingFunct[KEYWORD_LISTEN] = &Parser::_parseListenRule;
	_parsingFunct[KEYWORD_ROOT] = &Parser::_parseRootRule;
	_parsingFunct[KEYWORD_INDEX] = &Parser::_parseIndexRule;
	_parsingFunct[KEYWORD_AUTOINDEX] = &Parser::_parseAutoindexRule;
	_parsingFunct[KEYWORD_BODY_LIMIT] = &Parser::_parseMaxBodySizeRule;
	_parsingFunct[KEYWORD_CGI] = &Parser::_parseCgiRule;
	_parsingFunct[KEYWORD_ERROR_PAGE] = &Parser::_parseErrorPageRule;
	_parsingFunct[KEYWORD_REDIRECT] = &Parser::_parseRedirectRule;
	_parsingFunct[KEYWORD_ALLOWED_METHOD] = &Parser::_parseAllowedMethodRule;
	_parsingFunct[KEYWORD_UPLOAD_PATH] = &Parser::_parseUploadPathRule;
	_parsingFunct[KEYWORD_LOCATION] = &Parser::_createNewLocation;
}

void	Parser::_setDirective()
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

bool	Parser::_isDirective(Token::tokenType type)
{
	Lexer::listOfTokenTypes::const_iterator		ite;
	Lexer::listOfTokenTypes						tokenTypes;

	tokenTypes = _lexer.getTokenTypes();
	for (ite = tokenTypes.begin(); ite != tokenTypes.end(); ite++)
	{
		_lexer.printType(type);
		if (ite->second == type)
		{
			_directive = ite->first;
			return (true);
		}
	}
	return (false);
}

bool	Parser::_tokenIsDelimiter(Token::tokenType tokenType)
{
	return (tokenType == BLOCK_END || tokenType == BLOCK_START || tokenType == SEMICOLON);
}

void	Parser::_checkHost(const std::string &token)
{
	size_t		found;
	size_t		pos;

	pos = 0;
	if (token == "localhost")
		return ;
	for (int i = 0; i < 4; i++)
	{
		found = token.find_first_not_of("0123456789", pos);
		if ((i < 3 && (found == std::string::npos || token[found] != '.')) 
			|| (i == 3 && found != std::string::npos)
			|| (atoi(token.substr(pos, found - pos).c_str()) > 255))
			_throwErrorParsing("host not found in '" + token + "' of the 'listen' directive");
		pos = found + 1;
	}
}

/******************************************************************************/
/*                                  GETTER                                    */
/******************************************************************************/

std::string		Parser::getConfigFile() const
{
	return (_configFile);
}

Lexer	Parser::getLexer() const
{
	return (_lexer);
}

Lexer::listOfTokens::const_iterator		Parser::getCurrentToken() const
{
	return (_currentToken);
}

Parser::listOfServers	Parser::getServers() const
{
	return (_servers);
}

Parser::listOfServers::const_iterator	Parser::getCurrentServer() const
{
	return (_currentServer);
}

Parser::blockPtr	Parser::getCurrentBlock() const
{
	return (_currentBlock);
}

Parser::listOfParsingFunctions		Parser::getParsingFunct() const
{
	return (_parsingFunct);
}

t_context	Parser::getContext() const
{
	return (_context);
}

std::string		Parser::getDirective() const
{
	return (_directive);
}


/******************************************************************************/
/*                                   ERROR                                    */
/******************************************************************************/

/* invalid port in "0.0.0.0:" of the "listen" directive in nginx.conf:4 */
/* invalid port in "8000000" of the "listen" directive in nginx.conf:4 */
/* "client_max_body_size" directive invalid value in nginx.conf:10 */
/* unexpected end of file, expecting ';' or '}' in nginx.conf:2 */

std::string Parser::_invalidErrorCodeMsg(Lexer::listOfTokens::const_iterator token)
{
	return ("value '" + token->getValue() + "' must be between 300 and 527");
}

std::string Parser::_unexpectedValueMsg(Lexer::listOfTokens::const_iterator token)
{
	if (token != _lexer.getTokens().end())
		return ("unexpected '" + token->getValue() + "'");
	return ("unexpected end of file, expecting '}'");
}

std::string Parser::_unknownMethodMsg(Lexer::listOfTokens::const_iterator token)
{
	return ("unknown method '" + token->getValue() + "' in '" + getDirective() + "' directive");
}

std::string Parser::_unknownDirectiveMsg(Lexer::listOfTokens::const_iterator token)
{
	return ("unknown directive '" + token->getValue() + "'");
}

std::string Parser::_keywordServerError()
{
	Lexer::listOfTokens::const_iterator		nextToken;

	nextToken = _currentToken + 1;
	if (_tokenIsDelimiter(nextToken->getType()))
		return (_unexpectedValueMsg(nextToken));
	if (_isDirective(nextToken->getType()))
		return (_directiveNotAllowedHereMsg());
	return (_unknownDirectiveMsg(nextToken));
}

std::string Parser::_directiveNotAllowedHereMsg()
{
	return ("'" + getDirective() + "' directive is not allowed here");
}

std::string	Parser::_invalidNbOfArgumentsMsg()
{
	return ("invalid number of arguments in '" + getDirective() + "' directive");
}

std::string	Parser::_invalidParameterMsg()
{
	std::string	incorrectParam;

	incorrectParam = "";
	if ((_currentToken + 1) != _lexer.getTokens().end())
		incorrectParam = (_currentToken + 1)->getValue();
	return ("invalid parameter '" + incorrectParam + "'");
}

std::string	Parser::_invalidPortMsg()
{
	return ("invalid port of the 'listen' directive");
}

std::string	Parser::_invalidValueMsg(Lexer::listOfTokens::const_iterator token)
{
	std::string	incorrectValue;

	incorrectValue = "";
	if (token != _lexer.getTokens().end())
		incorrectValue = token->getValue();
	return ("invalid value '" + incorrectValue + "' in '" + getDirective() + "' directive");
}

std::string	Parser::_invalidPathMsg()
{
	std::string	incorrectValue;

	incorrectValue = "";
	if ((_currentToken) != _lexer.getTokens().end())
		incorrectValue = (_currentToken)->getValue();
	return ("invalid path '" + incorrectValue + "'");
}

void	Parser::_throwErrorParsing(std::string errorMsg)
{
	std::string message;

	message = "Webserv error: " + errorMsg;
	message += " in " + getConfigFile() + ":" + _currentToken->getLineStr();
	if (_currentBlock)
		delete _currentBlock;
	throw (std::runtime_error(message));
}

void	Parser::_throwErrorParsing(std::string errorMsg, std::string lineNbr)
{
	std::string message;

	message = "Webserv error: " + errorMsg;
	message += " in " + getConfigFile() + ":" + lineNbr;
	if (_currentBlock)
		delete _currentBlock;
	throw (std::runtime_error(message));
}

/******************************************************************************/
/*                                  DISPLAY                                   */
/******************************************************************************/

void	Parser::_printArrayParsingFunctions()
{
	Parser::listOfParsingFunctions::iterator	ite;

	for (ite = _parsingFunct.begin(); ite != _parsingFunct.end(); ite++)
		(this->*ite->second)();
}

void	Parser::_printCurrentToken()
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

	count = 1;
	std::cout << GREY << std::endl << "................................";
	for (currentServer = _servers.begin(); currentServer != _servers.end(); currentServer++, count++)
		(*currentServer)->displayParams(count);
	std::cout << "................................" << RESET << std::endl;
}
