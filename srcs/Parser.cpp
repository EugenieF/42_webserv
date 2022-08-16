#include "Parser.hpp"

Parser::Parser(std::string configFile): _lexer(configFile)
{
	initArrayParsingFunctions();
	printArrayParsingFunctions();
	printTokens();
}

Parser::~Parser()
{
	listOfServers::const_iterator		ite;

	for (ite = _servers.begin(); ite != _servers.end(); ite++)
		delete (*ite); // Not sure about this
}

void	Parser::parseTokens()
{
	Lexer::listOfTokens::const_iterator				ite;
	Parser::listOfParsingFunctions::iterator		searchFunct;

	
	for (ite = _lexer.getTokens().begin(); ite != _lexer.getTokens().end(); ite++)
	{
		_currentToken = *ite;
		if (_currentToken.getType() == KEYWORD_SERVER)
		{
			std::cout << "*** Server ***" << std::endl;
			_servers.insert(_servers.end(), new Server());
		}
		searchFunct = _parsingFunct.find(_currentToken.getType());
		if (searchFunct != _parsingFunct.end())
		{
			// std::cout << std::endl;
			// _lexer.printType(searchFunct->first);
			(this->*searchFunct->second)();
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

void	Parser::parseAutoindexRule()
{
	std::cout << "Parse Autoindex" << std::endl;
}

void	Parser::parseMaxBodySizeRule()
{
	std::cout << "Parse MaxBodySize" << std::endl;
}

void	Parser::parseCgiRule()
{
	std::cout << "Parse Cgi" << std::endl;
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
