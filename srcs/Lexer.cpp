#include "webserv.hpp"
#include "Lexer.hpp"

Lexer::Lexer() {}

Lexer::Lexer(std::string configFile): _lineCount(1)
{
	buildTokenTypeArray();
	_tokens.insert(_tokens.end(), Token(FILE_START, "", 0));
	openFile(configFile);
}

Lexer::~Lexer()
{
	this->closeFile();
}

/******************************************************************************/
/*                                   LEXER                                    */
/******************************************************************************/

void	Lexer::readFile()
{
	char	character;

	while (!_file.eof())
	{
		character = _file.peek();
		if (character == '#')
			ignoreComments(character);
		else if (isspace(character))
			getNextCharacter();
		else if (isDelimiter(character))
			getDelimiter();
		else
			getToken(character);
	}
	std::cout << YELLOW << "Line count = " << _lineCount << RESET << std::endl;
}

void	Lexer::getToken(char character)
{
	std::string					token;
	listOfTokenTypes::iterator	ite;

	token = "";
	while (!reachedEndOfFile() && !isspace(character) && !isDelimiter(character) && character != '#')
	{
		token += getNextCharacter();
		character = _file.peek();
	}
	if (token != "")
	{
		ite = _tokenTypes.find(token);
		if (ite != _tokenTypes.end())
			_tokens.insert(_tokens.end(), Token(_tokenTypes[token], token, _lineCount));
		else
			getValue(token);
	}
}

void	Lexer::getValue(std::string &token)
{
	t_tokenType	type;

	type = VALUE;
	if (tokenIsNumber(token))
		type = NUMBER;
	else if (tokenIsSize(token))
	{
		token = handleSize(token);
		type = NUMBER;
	}
	else if (tokenIsPath(token))
		type = PATH;
	_tokens.insert(_tokens.end(), Token(type, token, _lineCount));
}

void	Lexer::getDelimiter()
{
	std::string		token;

	token = "";
	token += getNextCharacter();
	_tokens.insert(_tokens.end(), Token(_tokenTypes[token], token, _lineCount));
}

/******************************************************************************/
/*                                TOKEN TYPE                                  */
/******************************************************************************/

bool	Lexer::isDelimiter(char character)
{
	return (character == ';' || character == '{' || character == '}' || character == ':');
}

bool	Lexer::tokenIsNumber(const std::string &token)
{
	return (token.find_first_not_of("0123456789") == std::string::npos);
}

std::string		Lexer::handleSize(std::string &token)
{
	char			lastCharacter;
	std::string		size;

	lastCharacter = token[token.size() - 1];
	size = token.substr(0, token.size() - 1);
	if (lastCharacter == 'k' || lastCharacter == 'K')
		size += "000";
	if (lastCharacter == 'm' || lastCharacter == 'M')
		size += "000000";
	if (lastCharacter == 'g' || lastCharacter == 'G')
		size += "000000000";
	std::cout << "token = " << token << " | size = " << size << std::endl;
	return (size);
}

bool	Lexer::tokenIsSize(const std::string &token)
{
	size_t	found;
	char	lastCharacter;

	found = token.find_first_not_of("0123456789");
	lastCharacter = token[token.size() - 1];
	if (found != std::string::npos && found == token.size() - 1)
		return (lastCharacter == 'k' || lastCharacter == 'K'
			|| lastCharacter == 'm' ||  lastCharacter == 'M'
			|| lastCharacter == 'g' || lastCharacter == 'G');
	return (false);
}

bool	Lexer::tokenIsPath(const std::string &token)
{
	return (token.find("/") != std::string::npos);
}

// bool	Lexer::tokenIsAddress(const std::string &token)
// {
// 	size_t	found;
// 	size_t	pos;

// 	pos = 0;
// 	for (int i = 0; i < 3; i++)
// 	{
// 		found = token.find_first_not_of("0123456789", pos);
// 		if (found == std::string::npos || token[found] != '.')
// 			return (false);
// 		pos = found + 1;
// 	}
// 	return (token.find_first_not_of("0123456789", found + 1) == std::string::npos);
// }

/******************************************************************************/
/*                                    FILE                                    */
/******************************************************************************/

bool	Lexer::checkFile(std::string configFile)
{
	struct stat	statStruct;

	if (stat(configFile.c_str(), &statStruct) != 0)
		return (false); // not found
	if (statStruct.st_mode & S_IFDIR)
		return (false); // is directory
	if (access(configFile.c_str(), F_OK) < 0)
		return (false); // no rights
	return (true);
}

void	Lexer::openFile(std::string configFile)
{
	if (!checkFile(configFile))
		throw(std::runtime_error("Cannot open file"));
	_file.open(configFile.c_str());
	if (!_file || !_file.is_open())
		throw(std::runtime_error("Cannot open file"));
	readFile();
}

void	Lexer::closeFile()
{
	if (_file.is_open())
		_file.close();
}


/******************************************************************************/
/*                                   UTILS                                    */
/******************************************************************************/

const Lexer::listOfTokens&		Lexer::getTokens() const
{
	return (_tokens);
}

char	Lexer::getNextCharacter()
{
	char nextCharacter;
	
	nextCharacter = _file.get();
	if (nextCharacter == '\n')
		_lineCount++;
	return (nextCharacter);
}

bool	Lexer::reachedEndOfFile()
{
	return (_file.eof());
}

void	Lexer::ignoreComments(char character)
{
	while (!reachedEndOfFile() && character != '\n')
		character = getNextCharacter();
}

void	Lexer::buildTokenTypeArray(void)
{
	_tokenTypes["{"] = BLOCK_START;
	_tokenTypes["}"] = BLOCK_END;
	_tokenTypes[";"] = SEMICOLON;
	_tokenTypes[":"] = COLON;
	_tokenTypes["server"] = KEYWORD_SERVER;
	_tokenTypes["location"] = KEYWORD_LOCATION;
	_tokenTypes["server_name"] = KEYWORD_SERVER_NAME;
	_tokenTypes["listen"] = KEYWORD_LISTEN;
	_tokenTypes["root"] = KEYWORD_ROOT;
	_tokenTypes["index"] = KEYWORD_INDEX;
	_tokenTypes["autoindex"] = KEYWORD_AUTOINDEX;
	_tokenTypes["cgi"] = KEYWORD_CGI;
	_tokenTypes["error_page"] = KEYWORD_ERROR_PAGE;
	_tokenTypes["redirect"] = KEYWORD_REDIRECT;
	_tokenTypes["client_max_body_size"] = KEYWORD_BODY_LIMIT;
	_tokenTypes["allowed_method"] = KEYWORD_ALLOWED_METHOD;
	_tokenTypes["upload_path"] = KEYWORD_UPLOAD_PATH;
}

/******************************************************************************/
/*                                   PRINT                                    */
/******************************************************************************/

void	Lexer::printTokens()
{
	listOfTokens::iterator	ite;

	std::cout << std::endl  << "  >>> LEXER | List of tokens <<< " << std::endl;
	for (ite = _tokens.begin(); ite != _tokens.end(); ite++)
	{
		std::cout << "[";
		printType(ite->getType());
		std::cout << " : " << ite->getValue() << "]" << std::endl;
	}
}

std::string		Lexer::printType(Token::tokenType type)
{
	if (type == FILE_START)
		return ("FILE_START");
	else if (type == BLOCK_START)
		return ("BLOCK_START");
	else if (type == BLOCK_END)
		return ("BLOCK_END");
	else if (type == SEMICOLON)
		return ("SEMICOLON");
	else if (type == COLON)
		return ("COLON");
	else if (type == KEYWORD_SERVER)
		return ("KEYWORD_SERVER");
	else if (type == KEYWORD_LOCATION)
		return ("KEYWORD_LOCATION");
	else if (type == KEYWORD_SERVER_NAME)
		return ("KEYWORD_SERVER_NAME");
	else if (type == KEYWORD_LISTEN)
		return ("KEYWORD_LISTEN");
	else if (type == KEYWORD_ROOT)
		return ("KEYWORD_ROOT");
	else if (type == KEYWORD_INDEX)
		return ("KEYWORD_INDEX");
	else if (type == KEYWORD_AUTOINDEX)
		return ("KEYWORD_AUTOINDEX");
	else if (type == KEYWORD_CGI)
		return ("KEYWORD_CGI");
	else if (type == KEYWORD_ERROR_PAGE)
		return ("KEYWORD_ERROR_PAGE");
	else if (type == KEYWORD_REDIRECT)
		return ("KEYWORD_REDIRECT");
	else if (type == KEYWORD_BODY_LIMIT)
		return ("KEYWORD_BODY_LIMIT");
	else if (type == KEYWORD_ALLOWED_METHOD)
		return ("KEYWORD_ALLOWED_METHOD");
	else if (type == KEYWORD_UPLOAD_PATH)
		return ("KEYWORD_UPLOAD_PATH");
	else if (type == NUMBER)
		return ("NUMBER");
	else if (type == PATH)
		return("PATH");
	return ("VALUE");
}
