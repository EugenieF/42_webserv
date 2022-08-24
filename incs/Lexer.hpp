#ifndef LEXER_HPP
# define LEXER_HPP

# include <unistd.h>

# include "Token.hpp"
# include "Colors.hpp"

bool	openFile(char *configFile);

class	Lexer
{
	public:
		typedef std::vector<Token>						listOfTokens;
		typedef std::map<std::string, Token::tokenType>	listOfTokenTypes;

		Lexer();
		Lexer(std::string configFile);
		~Lexer();

	private :
		std::ifstream						_file;
		listOfTokens						_tokens;
		listOfTokenTypes					_tokenTypes;
		unsigned int						_lineCount;

		/****************************     LEXER      ***************************/

		void								readFile();
		void								getToken(char character);
		void								getValue(std::string &token);
		void								getDelimiter();

		/****************************   TOKEN TYPE   ***************************/

		bool								tokenIsNumber(const std::string &token);
		bool								tokenIsSize(const std::string &token);
		bool								tokenIsPath(const std::string &token);
		bool								tokenIsAddress(const std::string &token);
		std::string							handleSize(std::string &token);

		/****************************     FILE      ***************************/

		void								openFile(std::string configFile);
		void								closeFile();
	public :
		bool								checkFile(std::string configFile);

		/****************************     UTILS     ***************************/

	private :
		char								getNextCharacter();
		bool								reachedEndOfFile();
		void								ignoreComments(char character);
		bool								isDelimiter(char character);
		void								buildTokenTypeArray();

	public :
		const listOfTokens&					getTokens() const;
		listOfTokenTypes					getTokenTypes() const;

		/****************************     PRINT     ***************************/

		void								printTokens();
		std::string							printType(Token::tokenType type);
};

#endif
