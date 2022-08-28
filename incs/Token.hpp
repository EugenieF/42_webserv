#ifndef TOKEN_HPP
# define TOKEN_HPP

# include <string>
# include <cstdlib>
# include <iostream>
# include <cstdio>
# include <fstream>
# include <sstream>
# include <vector>
# include <map>
# include <utility>

# include <sys/stat.h>

/******************************************************************************/
/*                                  ENUM                                      */
/******************************************************************************/

typedef enum e_tokenType
{
	FILE_START		= 0,
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
	PATH,
	VALUE,
}	t_tokenType;

/******************************************************************************/
/*                                CLASS TOKEN                                 */
/******************************************************************************/

class	Token
{
	public :
	/***********************      MEMBER TYPES      *********************/
		typedef t_tokenType				tokenType;
		typedef std::string				tokenValue;

	private :
	/**********************     MEMBER VARIABLES     ********************/
		tokenType						_type;
		tokenValue						_value;
		size_t							_line;

	public :
	/**********************  PUBLIC MEMBER FUNCTIONS  *******************/
		Token();
		Token(tokenType type, tokenValue value, unsigned int line);
		Token(const Token &other);
		~Token();
		Token 							&operator=(const Token &other);

						/*-------    Getter    ------*/
		tokenType						getType() const;
		tokenValue						getValue() const;
		size_t							getLineNbr() const;
		std::string						getLineStr() const;
};

#endif