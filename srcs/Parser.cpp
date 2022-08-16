#include "Parser.hpp"

Parser::Parser(std::string configFile):
	_lexer(configFile),
	_currentToken(_lexer.getTokens().begin())
{
	initArrayParsingFunctions();
	// printArrayParsingFunctions();
	printTokens();
}

Parser::~Parser()
{
	deleteServers();
}

void	Parser::deleteServers()
{
	for (_currentServer = _servers.begin(); _currentServer != _servers.end(); _currentServer++)
		delete (*_currentServer); // Not sure about this
}

void	Parser::parseServer()
{
	Parser::listOfParsingFunctions::iterator	parseFunctIte;

	_currentToken++;
	while (!reachedEndOfTokens() && _currentToken->getType() != KEYWORD_SERVER)
	{
		parseFunctIte = _parsingFunct.find(_currentToken->getType());
		if (parseFunctIte != _parsingFunct.end())
		{
			(this->*parseFunctIte->second)();
		}
		_currentToken++;
	}
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

void	Parser::parseTokens()
{
	for (; !reachedEndOfTokens(); _currentToken++)
	{
		if (_currentToken->getType() == KEYWORD_SERVER)
		{
			std::cout << "*** New Server ***" << std::endl;
			_currentServer = _servers.insert(_servers.end(), new Server());
			parseServer();
			if (reachedEndOfTokens())
				break ;
		}
	}
}

void	Parser::parseServerNameRule()
{
	std::cout << "Parse Server Name" << std::endl;
}

void	Parser::parseListenRule()
{
	std::cout << "Parse Listen" << std::endl;
}

void	Parser::parseRootRule()
{
	std::cout << "Parse Root" << std::endl;
}

void	Parser::parseIndexRule()
{
	std::cout << "Parse Index" << std::endl;
}

void	Parser::checkSemicolon(std::string errorMsg)
{
	if (!getNextToken() || _currentToken->getType() != SEMICOLON)
		throw (std::runtime_error(errorMsg));
}

void	Parser::parseAutoindexRule()
{
	
	std::cout << "Parse Autoindex" << std::endl;
	if (getNextToken() && _currentToken->getType() == VALUE)
	{
		if (_currentToken->getValue() == "on")
			(*_currentServer)->setAutoindex(true);
		else if (_currentToken->getValue() == "off")
			(*_currentServer)->setAutoindex(false);
		else
			throw (std::runtime_error("Error rule autoindex"));
		checkSemicolon("Error rule autoindex");
	}
	else
		throw (std::runtime_error("Error rule autoindex"));
}

void	Parser::parseMaxBodySizeRule()
{
	std::cout << "Parse MaxBodySize" << std::endl;
}

void	Parser::parseCgiRule()
{
	std::cout << "Parse Cgi" << std::endl;
}

void	Parser::parseErrorPageRule()
{
	std::cout << "Parse ErrorPage" << std::endl;
}

void	Parser::parseRedirectRule()
{
	std::cout << "Parse Redirect" << std::endl;
}

void	Parser::parseAllowedMethodRule()
{
	std::cout << "Parse AllowedMethod" << std::endl;
}

void	Parser::parseUploadPathRule()
{
	std::cout << "Parse Upload Path" << std::endl;
}

void	Parser::printArrayParsingFunctions()
{
	Parser::listOfParsingFunctions::iterator	ite;

	std::cout << "Print array Parsing functions :" << std::endl;
	for (ite = _parsingFunct.begin(); ite != _parsingFunct.end(); ite++)
		(this->*ite->second)();
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
}

/****************         PRINT        *****************/

void	Parser::printTokens()
{
	Lexer::listOfTokens::const_iterator		ite;

	std::cout << std::endl << "Size : " << _lexer.getTokens().size();
	std::cout << std::endl  << "  >>> PARSER | List of tokens <<< " << std::endl;
	for (ite = _lexer.getTokens().begin(); ite != _lexer.getTokens().end(); ite++)
	{
		std::cout << "[";
		_lexer.printType(ite->getType());
		std::cout << " : " << ite->getValue() << "]" << std::endl;
	}
}
