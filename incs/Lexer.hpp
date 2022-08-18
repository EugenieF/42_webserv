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

	private :
		std::ifstream						_file;
		listOfTokens						_tokens;
		listOfTokenTypes					_tokenTypes;
		unsigned int						_lineCount;

		/****************************     LEXER      ***************************/

		void								readFile();
		void								getToken(char character);
		void								getValue(const std::string &token);
		void								getDelimiter();

		/****************************   TOKEN TYPE   ***************************/

		bool								tokenIsNumber(const std::string &token);
		bool								tokenIsSize(const std::string &token);
		bool								tokenIsPath(const std::string &token);
		bool								tokenIsAddress(const std::string &token);

		/****************************     FILE      ***************************/

		bool								checkFile(std::string configFile);
		void								openFile(std::string configFile);
		void								closeFile();

		/****************************     UTILS     ***************************/

		char								getNextCharacter();
		bool								reachedEndOfFile();
		void								ignoreComments(char character);
		bool								isDelimiter(char character);
		void								buildTokenTypeArray();

	public :
		const listOfTokens&					getTokens() const;

		/****************************     PRINT     ***************************/

		void								printTokens();
		std::string							printType(Token::tokenType type);
};

#endif
