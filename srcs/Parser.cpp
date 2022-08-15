#include "Parser.hpp"

Parser::Parser(std::string configFile): _lexer(configFile)
{
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
	Lexer::listOfTokens::const_iterator		ite;
	
	for (ite = _lexer.getTokens().begin(); ite != _lexer.getTokens().end(); ite++)
	{
		_currentToken = *ite;
		if (_currentToken.getType() == KEYWORD_SERVER)
		{
			std::cout << "*** Server ***" << std::endl;
			_servers.insert(_servers.end(), new Server());
		}
	}
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
