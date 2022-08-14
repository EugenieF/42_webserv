#ifndef LEXER_HPP
# define LEXER_HPP

# include <vector>
# include <map>
# include <utility>
# include <string>

# include <sys/stat.h>

# include "Token.hpp"

bool	openFile(char *configFile);

class	Lexer
{
	public:
		typedef std::vector<Token>						listOfTokens;
		typedef std::map<std::string, Token::tokenType>	listOfTokenTypes;

	private :
			std::ifstream							_file;
			listOfTokens							_tokens;
			listOfTokenTypes						_tokenTypes;

	public :
		Lexer();
		Lexer(std::string configFile);
		~Lexer();
		void	openFile(std::string configFile);
		bool	checkFile(std::string configFile);
		void	readFile();
		void	splitTokens(char character);
		bool	isDelimiter(char character);
		void	closeFile();
		void	printTokens();
		void	printType(Token::tokenType type);
		void	buildTokenTypeArray();

		class CannotOpenFile: public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

};

#endif
