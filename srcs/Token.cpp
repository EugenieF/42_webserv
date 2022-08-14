#include "Token.hpp"

Token::Token(): _type(FILE_DONE), _value("") {}

Token::Token(tokenType type, tokenValue value): _type(type), _value(value) {}

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
