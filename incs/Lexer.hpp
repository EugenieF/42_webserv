#ifndef LEXER_HPP
# define LEXER_HPP

# include "Token.hpp"

bool	openFile(char *configFile);

class	Lexer
{
	public:
		typedef std::vector<Token>						listOfTokens;
		typedef std::map<std::string, Token::tokenType>	listOfTokenTypes;

		Lexer();
		Lexer(std::string configFile);
		~Lexer();
		const listOfTokens&		getTokens() const;
		void					printTokens();
		void					printType(Token::tokenType type);

	private :
		std::ifstream							_file;
		listOfTokens							_tokens;
		listOfTokenTypes						_tokenTypes;

		void	openFile(std::string configFile);
		bool	checkFile(std::string configFile);
		void	readFile();
		char	getNextCharacter();
		bool	reachedEndOfFile();
		void	getToken(char character);
		void	getValue(const std::string &token);
		bool	tokenIsNumber(const std::string &token);
		bool	tokenIsSize(const std::string &token);
		bool	tokenIsPath(const std::string &token);
		bool	tokenIsAddress(const std::string &token);
		void	ignoreComments(char character);
		void	getDelimiter();
		bool	isDelimiter(char character);
		void	closeFile();
		void	buildTokenTypeArray();

		class CannotOpenFile: public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

};

#endif
