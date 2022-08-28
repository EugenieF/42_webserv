#ifndef PARSER_HPP
# define PARSER_HPP

# include "Lexer.hpp"
# include "Block.hpp"

/******************************************************************************/
/*                               CLASS PARSER                                 */
/******************************************************************************/

class Parser
{
	
	public:
	/***********************      MEMBER TYPES      *********************/
		typedef std::vector<Block *>						listOfServers;
		typedef	Block*										blockPtr;
		typedef void (Parser::*parsingFunction)();
		typedef std::map<Token::tokenType, parsingFunction>	listOfParsingFunctions;


	private:
	/**********************     MEMBER VARIABLES     ********************/
		std::string							_configFile;
		Lexer								_lexer;
		Lexer::listOfTokens::const_iterator	_currentToken;
		listOfServers						_servers;
		listOfServers::const_iterator		_currentServer;
		blockPtr							_currentBlock;
		listOfParsingFunctions				_parsingFunct;
		t_context							_context;
		std::string							_directive;
		
	public:
	/**********************  PUBLIC MEMBER FUNCTIONS  *******************/
		Parser();
		Parser(std::string configFile);
		Parser(const Parser& other);
		~Parser();
		Parser&								operator=(const Parser& other);
		void								parseFile(std::string configFile);
		void								parseTokens();
		void								printTokens();

						/*-------    Getter   ------*/
		std::string							getConfigFile() const;
		Lexer								getLexer() const;
		Lexer::listOfTokens::const_iterator	getCurrentToken() const;
		listOfServers						getServers() const;
		listOfServers::const_iterator		getCurrentServer() const;
		blockPtr							getCurrentBlock() const;
		listOfParsingFunctions				getParsingFunct() const;
		t_context							getContext() const;
		std::string							getDirective() const;

						/*-------    Display   ------*/
		void								displayServersParams();

	private:
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/
		void								_parseBlock();
		blockPtr							_createNewServer();
		void								_createNewLocation();
		void								_parseRule();


						/*-----  Parsing rules -----*/
		void								_parseServerNameRule();
		void								_parseListenRule();
		void								_parseRootRule();
		void								_parseIndexRule();
		void								_parseAutoindexRule();
		void								_parseMaxBodySizeRule();
		void								_parseCgiRule();
		void								_parseErrorPageRule();
		void								_parseRedirectRule();
		void								_parseAllowedMethodRule();
		void								_parseUploadPathRule();

						/*-------    Utils    ------*/
		void								_updateContext(t_context currentContext, blockPtr currentBlock);
		bool								_currentBlockIsServer();
		bool								_currentBlockIsLocation();
		bool								_reachedEndOfTokens();
		bool								_reachedEndOfDirective();
		bool								_reachedEndOfBlock();
		bool								_getNextToken();
		void								_expectNbOfArguments(int expectedNb);
		void								_expectNextToken(Token::tokenType expectedType, std::string errorMsg);
		void								_initArrayParsingFunctions();
		void								_deleteServers();
		void								_setDirective();

						/*-------     Error    ------*/
		std::string							_directiveNotAllowedHere();
		std::string							_invalidValueMsg();
		std::string							_invalidPathMsg();
		std::string							_invalidNbOfArgumentsMsg();
		void								_throwErrorParsing(std::string errorMsg);
		void								_throwErrorParsingWithLine(std::string errorMsg, std::string lineNbr);

						/*-------    Display   ------*/
		void								_printCurrentToken();
		void								_printArrayParsingFunctions();
};

#endif
