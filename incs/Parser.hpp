#ifndef PARSER_HPP
# define PARSER_HPP

# include "Lexer.hpp"
# include "Block.hpp"

class Parser
{
	public:
		typedef std::vector<Block *>						listOfServers;
		typedef	Block*										blockPtr;
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
		blockPtr							_currentBlock;
		listOfParsingFunctions				_parsingFunct;
		t_context							_context;
		std::string							_directive;
		
		/**************************     PARSER     *****************************/

		void								parseBlock();
		blockPtr							createNewServer();
		void								createNewLocation();
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

		void								updateContext(t_context currentContext, blockPtr currentBlock);
		bool								currentBlockIsServer();
		bool								currentBlockIsLocation();
		bool								reachedEndOfTokens();
		bool								reachedEndOfDirective();
		bool								reachedEndOfBlock();
		bool								getNextToken();
		void								expectNbOfArguments(int expectedNb);
		void								expectNextToken(Token::tokenType expectedType, std::string errorMsg);
		void								initArrayParsingFunctions();
		void								deleteServers();
		std::string							getConfigFile() const;
		std::string							getDirective() const;
		void								setDirective();

		/****************************     ERROR     ***************************/

		std::string							directiveNotAllowedHere();
		std::string							invalidValueMsg();
		std::string							invalidNbOfArgumentsMsg();
		void								throwErrorParsing(std::string errorMsg);

		/****************************     PRINT     ***************************/

		void								printCurrentToken();
		void								printTokens();
		void								printArrayParsingFunctions();

	public:
		void								displayServersParams();

};

#endif
