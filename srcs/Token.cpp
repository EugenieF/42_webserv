#include "Token.hpp"

Token::Token():
	_type(FILE_DONE),
	_value(""),
	_line(0) {}

Token::Token(tokenType type, tokenValue value, unsigned int line):
	_type(type),
	_value(value),
	_line(line)	{}

Token::Token(Token const &other)
{
	*this = other;
}

Token::~Token() {}

Token		&Token::operator=(const Token &other)
{
	if (this != &other)
	{
		this->_type = other.getType();
		this->_value = other.getValue();
		this->_line = other.getLineNbr();
	}
	return (*this);
}

Token::tokenType	Token::getType() const
{
	return (this->_type);
}

Token::tokenValue	Token::getValue() const
{
	return (this->_value);
}

unsigned int	Token::getLineNbr() const
{
	return (this->_line);
}

std::string		Token::getLineStr() const
{
	std::stringstream	ss;

	ss << _line;
	return (ss.str());
}
