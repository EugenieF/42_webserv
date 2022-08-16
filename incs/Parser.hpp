#ifndef PARSER_HPP
# define PARSER_HPP

# include "Lexer.hpp"
# include "Server.hpp"

class Parser
{
	public:
		typedef std::vector<Server *>					listOfServers;
		typedef std::map<Token::tokenType, void (Parser::*)()>	listOfParsingFunctions;
	
		Parser(std::string configFile);
		~Parser();
		void								parseTokens();
		void								printTokens();

	private:
		Token								_currentToken;
		Lexer								_lexer;
		listOfServers						_servers;
		listOfParsingFunctions				_parsingFunct;
		
		void								initArrayParsingFunctions();
		void								parseServerNameRule();
		void								parseListenRule();
		void								parseRootRule();
		void								parseIndexRule();
		void								parseAutoindexRule();
		void								parseMaxBodySizeRule();
		void								parseCgiRule();
};

#endif
