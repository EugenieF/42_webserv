#ifndef TOKEN_HPP
# define TOKEN_HPP

# include <string>

typedef enum e_tokenType
{
	FILE_DONE	= 0,
	BLOCK_START,
	BLOCK_END,
	SEMICOLON,
	COLON,
	KEYWORD_SERVER,
	KEYWORD_LOCATION,
	KEYWORD_SERVER_NAME,
	KEYWORD_LISTEN,
	KEYWORD_ROOT,
	KEYWORD_INDEX,
	KEYWORD_AUTOINDEX,
	KEYWORD_CGI,
	KEYWORD_ERROR_PAGE,
	KEYWORD_REDIRECT,
	KEYWORD_BODY_LIMIT,
	KEYWORD_ALLOWED_METHOD,
	KEYWORD_UPLOAD_PATH,
	NUMBER,
	SIZE,
	PATH,
	VALUE,
}	t_tokenType;

class	Token
{
	public :
		typedef t_tokenType		tokenType;
		typedef std::string		tokenValue;

	private :
		tokenType				_type;
		tokenValue				_value;

	public :
		Token();
		Token(tokenType type, tokenValue value);
		Token(const Token &other);
		~Token();
		Token 					&operator=(const Token &other);
		tokenType				getType() const;
		tokenValue				getValue() const;
};

#endif