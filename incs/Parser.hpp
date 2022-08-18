#ifndef PARSER_HPP
# define PARSER_HPP

# include "Lexer.hpp"
# include "Server.hpp"
# include "webserv.hpp"

typedef enum e_context
{
	SERVER		= 0,
	LOCATION	= 1,
}	t_context;

class Parser
{
	public:
		typedef std::vector<Server *>						listOfServers;
		typedef void (Parser::*parsingFunction)();
		typedef std::map<Token::tokenType, parsingFunction>	listOfParsingFunctions;
	
		Parser(std::string configFile);
		~Parser();
		void								parseTokens();

		/**************************    VARIABLES    ****************************/

	private:
		std::string							_configFile;
		Lexer								_lexer;
		Lexer::listOfTokens::const_iterator	_currentToken;
		listOfServers						_servers;
		listOfServers::const_iterator		_currentServer;
		listOfParsingFunctions				_parsingFunct;
		t_context							_context;
		std::string							_directive;
		
		/**************************     PARSER     *****************************/

		void								parseServer();
		void								parseLocation();
		void								parseBlock(t_context blockContext);
		void								parseRule();


		/**************************  PARSING RULES  ***************************/

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

		/****************************     UTILS     ***************************/

		bool								reachedEndOfTokens();
		bool								getNextToken();
		void								checkDelimiter(Token::tokenType tokenType, std::string errorMsg);
		void								expectNextToken(Token::tokenType expectedType, std::string errorMsg);
		void								expectToken(Token::tokenType expectedType, std::string errorMsg);
		void								initArrayParsingFunctions();
		void								deleteServers();
		std::string							getConfigFile() const;
		std::string							getDirective() const;

		/****************************     ERROR     ***************************/

		void								throwErrorParsing(std::string errorMsg);

		/****************************     PRINT     ***************************/

		void								printCurrentToken();
		void								printTokens();
		void								printArrayParsingFunctions();
};

#endif
