#include "webserv.hpp"
#include "Lexer.hpp"

Lexer::Lexer() {}

Lexer::Lexer(std::string configFile)
{
	buildTokenTypeArray();
	openFile(configFile);
}

Lexer::~Lexer()
{
	this->closeFile();
}

const char	*Lexer::CannotOpenFile::what() const throw()
{
	return ("Cannot open file");
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

bool	Lexer::tokenIsNumber(const std::string &token)
{
	return (token.find_first_not_of("0123456789") == std::string::npos);
}

void	Lexer::getValue(const std::string &token)
{
	t_tokenType	type;

	type = VALUE;
	if (tokenIsNumber(token))
		type = NUMBER;
	_tokens.insert(_tokens.end(), Token(type, token));
}

bool	Lexer::isDelimiter(char character)
{
	return (character == ';' || character == '{' || character == '}' || character == ':');
}

void	Lexer::getToken(char character)
{
	std::string					token;
	listOfTokenTypes::iterator	ite;

	token = "";
	while (!_file.eof() && !isspace(character) && !isDelimiter(character))
	{
		token += getNextCharacter();
		character = _file.peek();
	}
	if (token != "")
	{
		ite = _tokenTypes.find(token);
		if (ite != _tokenTypes.end())
			_tokens.insert(_tokens.end(), Token(_tokenTypes[token], token));
		else
			getValue(token);
	}
}

void	Lexer::getDelimiter()
{
	std::string		token;

	token = "";
	token += getNextCharacter();
	_tokens.insert(_tokens.end(), Token(_tokenTypes[token], token));
}

char	Lexer::getNextCharacter()
{
	return (_file.get());
}

void	Lexer::ignoreComments(char character)
{
	while (!_file.eof() && character != '\n')
		character = getNextCharacter();
}

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
	printTokens();
}

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
	_file.open(configFile);
	if (!_file || !_file.is_open())
		throw(std::runtime_error("Cannot open file"));
	readFile();
}

void	Lexer::closeFile()
{
	if (_file.is_open())
		_file.close();
}


/**********************            PRINT            *********************/

void	Lexer::printTokens()
{
	listOfTokens::iterator	ite;

	std::cout << std::endl  << "  >>> List of tokens <<< " << std::endl;
	for (ite = _tokens.begin(); ite != _tokens.end(); ite++)
	{
		std::cout << "[";
		printType(ite->getType());
		std::cout << " : " << ite->getValue() << "]" << std::endl;
	}
}

void	Lexer::printType(Token::tokenType type)
{
	if (type == BLOCK_START)
		std::cout << "BLOCK_START";
	else if (type == BLOCK_END)
		std::cout << "BLOCK_END";
	else if (type == SEMICOLON)
		std::cout << "SEMICOLON";
	else if (type == COLON)
		std::cout << "COLON";
	else if (type == KEYWORD_SERVER)
		std::cout << "KEYWORD_SERVER";
	else if (type == KEYWORD_LOCATION)
		std::cout << "KEYWORD_LOCATION";
	else if (type == KEYWORD_SERVER_NAME)
		std::cout << "KEYWORD_SERVER_NAME";
	else if (type == KEYWORD_LISTEN)
		std::cout << "KEYWORD_LISTEN";
	else if (type == KEYWORD_ROOT)
		std::cout << "KEYWORD_ROOT";
	else if (type == KEYWORD_INDEX)
		std::cout << "KEYWORD_INDEX";
	else if (type == KEYWORD_AUTOINDEX)
		std::cout << "KEYWORD_AUTOINDEX";
	else if (type == KEYWORD_CGI)
		std::cout << "KEYWORD_CGI";
	else if (type == KEYWORD_ERROR_PAGE)
		std::cout << "KEYWORD_ERROR_PAGE";
	else if (type == KEYWORD_REDIRECT)
		std::cout << "KEYWORD_REDIRECT";
	else if (type == KEYWORD_BODY_LIMIT)
		std::cout << "KEYWORD_BODY_LIMIT";
	else if (type == KEYWORD_ALLOWED_METHOD)
		std::cout << "KEYWORD_ALLOWED_METHOD";
	else if (type == KEYWORD_UPLOAD_PATH)
		std::cout << "KEYWORD_UPLOAD_PATH";
	else if (type == NUMBER)
		std::cout << "NUMBER";
	else if (type == SIZE)
		std::cout << "SIZE";
	else if (type == PATH)
		std::cout << "PATH";
	else
		std::cout << "VALUE";
}
