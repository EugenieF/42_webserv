#include "Token.hpp"

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/

Token::Token():
	_type(FILE_START),
	_value(""),
	_line(0) {}

Token::Token(tokenType type, tokenValue value, unsigned int line):
	_type(type),
	_value(value),
	_line(line)	{}

Token::Token(Token const &other):
	_type(other.getType()),
	_value(other.getValue()),
	_line(other.getLineNbr())
{
	*this = other;
}

Token::~Token() {}

Token		&Token::operator=(const Token &other)
{
	if (this != &other)
	{
		_type = other.getType();
		_value = other.getValue();
		_line = other.getLineNbr();
	}
	return (*this);
}

/******************************************************************************/
/*                                  GETTER                                    */
/******************************************************************************/

Token::tokenType	Token::getType() const
{
	return (this->_type);
}

Token::tokenValue	Token::getValue() const
{
	return (this->_value);
}

size_t	Token::getLineNbr() const
{
	return (this->_line);
}

std::string		Token::getLineStr() const
{
	std::stringstream	ss;

	ss << _line;
	return (ss.str());
}

std::string		Token::getLineStr(int nb) const
{
	std::stringstream	ss;

	ss << _line + nb;
	return (ss.str());
}

