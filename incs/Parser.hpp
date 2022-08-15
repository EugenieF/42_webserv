#ifndef PARSER_HPP
# define PARSER_HPP

# include "Lexer.hpp"
# include "Server.hpp"

class Parser
{
	public:
		typedef std::vector<Server *>		listOfServers;
	
		Parser(std::string configFile);
		~Parser();
		void					parseTokens();
		void					printTokens();

	private:
		Token					_currentToken;
		Lexer					_lexer;
		listOfServers			_servers;

};

#endif
