#ifndef PARSER_HPP
# define PARSER_HPP

# include "Lexer.hpp"
# include "Server.hpp"

class Parser
{
	public:
		typedef std::vector<Server *>						listOfServers;
		typedef void (Parser::*parsingFunction)();
		typedef std::map<Token::tokenType, parsingFunction>	listOfParsingFunctions;
	
		Parser(std::string configFile);
		~Parser();
		void								parseTokens();
		void								printTokens();
		void								printArrayParsingFunctions();

	private:
		Lexer								_lexer;
		Lexer::listOfTokens::const_iterator	_currentToken;
		listOfServers						_servers;
		listOfServers::const_iterator		_currentServer;
		listOfParsingFunctions				_parsingFunct;
		
		void								parseServer();
		void								deleteServers();
		bool								reachedEndOfTokens();
		bool								getNextToken();
		void								checkSemicolon(std::string errorMsg);

		void								initArrayParsingFunctions();
		void								parseServerNameRule();
		void								parseListenRule();
		void								parseRootRule();
		void								parseIndexRule();
		void								parseAutoindexRule();
		void								parseMaxBodySizeRule();
		void								parseCgiRule();
		void								parseErrorPageRule();
		void								parseRedirectRule();
		void								parseAllowedMethodRule();
		void								parseUploadPathRule();
};

#endif
