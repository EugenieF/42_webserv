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
		Lexer(const Lexer& other);
		~Lexer();
		Lexer&		operator=(const Lexer& other);

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

		void								closeFile();
	public :
		void								openFile(std::string configFile);
		void								checkFile(std::string configFile);

		/****************************     UTILS     ***************************/

	private :
		char								getNextCharacter();
		bool								reachedEndOfFile();
		void								ignoreComments(char character);
		bool								isDelimiter(char character);
		void								buildTokenTypeArray();

		/****************************     UTILS     ***************************/

		void								throwErrorLexer(std::string errorMsg);

		/****************************     GETTER    ***************************/

	public :
		const std::ifstream&				getFile() const;
		const listOfTokens&					getTokens() const;
		listOfTokenTypes					getTokenTypes() const;
		size_t								getLineCount() const;

		/****************************     PRINT     ***************************/

		void								printTokens();
		std::string							printType(Token::tokenType type);
};

#endif
