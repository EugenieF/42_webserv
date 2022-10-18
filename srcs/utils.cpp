# include "utils.hpp"

StatusCode		g_statusCode;
MimeType		g_mimeType;
HttpMethod		g_httpMethod;

/******************************************************************************/
/*                                 DEBUG                                      */
/******************************************************************************/

void	DEBUG(const std::string& str)
{
	(void)str;
	#ifdef DISPLAY
		PRINT(str);
	#endif
}

/******************************************************************************/
/*                              STRING RELATED                                */
/******************************************************************************/

std::string	convertSizeToString(size_t size)
{
	std::stringstream	ss;

	ss << size;
	return (ss.str());
}

std::string	convertNbToString(int nb)
{
	std::stringstream	ss;

	ss << nb;
	return (ss.str());
}

std::string	convertCharPtrToString(char* ptr)
{
	std::stringstream	ss;

	ss << ptr;
	return (ss.str());
}

bool	convertPort(const std::string& str, int* port)
{
	size_t	found;

	found = str.find_first_not_of("0123456789");
	if (found != std::string::npos)
		return (false);
	*port = atoi(str.c_str());
	return (*port >= 0 && *port <= 65535);
}

bool	convertHttpCode(const std::string& str, int* code)
{
	size_t	found;

	found = str.find_first_not_of("0123456789");
	if (found != std::string::npos)
		return (false);
	*code = atoi(str.c_str());
	return (*code >= 0 && *code <= 505);
}

std::string	trimSpacesStr(std::string *str, const char *toTrim)
{
	str->erase(0, str->find_first_not_of(toTrim));
	str->erase(str->find_last_not_of(toTrim) + 1);
	return (*str);
}

std::string	trimSpacesEndStr(std::string *str, const char *toTrim)
{
	str->erase(str->find_last_not_of(toTrim) + 1);
	return (*str);
}

/* Strdup... C++ style */
char* clone_str(const std::string& str) {
	if (str.empty())
		return (0);

	char*   copy = new char[str.size() + 1];

	strcpy(copy, str.c_str());
	return (copy);
}

std::string readFd(int fd) {
	std::string     str;
	char            buf[BUFSIZE + 1];
	ssize_t         count;

	memset(buf, 0, BUFSIZE + 1);
	count = read(fd, buf, BUFSIZE + 1);
	while (count) {
		if (count < 0)
			throw std::runtime_error("readFd (read) error");
		str += buf;
		if (count == BUFSIZE) {
			memset(buf, 0, BUFSIZE + 1);
			count = read(fd, buf, BUFSIZE + 1);
		} else {
			break ;
		}
	}
	return (str);
}

/******************************************************************************/
/*                               FILE RELATED                                 */
/******************************************************************************/

bool	pathIsFile(const std::string& path)
{
	struct stat s;

	return (stat(path.c_str(), &s) == 0 && (s.st_mode & S_IFREG));
}

bool	pathIsDirectory(const std::string& path)
{
	struct stat s;

	return (stat(path.c_str(), &s) == 0 && (s.st_mode & S_IFDIR));
}

bool	pathIsAccessible(const std::string& path)
{
	int	ret;

	ret = access(path.c_str(), F_OK); 
	return (!ret);
}

void	displayMsg(const std::string& msg, const char* colorCode)
{
	std::cout << colorCode << msg << RESET << std::endl;
}

/******************************************************************************/
/*                               TIME RELATED                                 */
/******************************************************************************/

std::string	getFormattedDate()
{
	std::time_t	time;
    char		date[100];
	int			ret;

	time = std::time(NULL);
    ret = std::strftime(date, sizeof(date), "%a, %d %b %Y %X GMT", std::localtime(&time));
	if (!ret)
	{
		/* An error occured */
		std::cerr << "Webserv error: strftime() function failed" << std::endl;
		throw (INTERNAL_SERVER_ERROR);
	}
	return(std::string(date));	
}

std::string	getFormattedDate(std::time_t time)
{
    char		date[100];
	int			ret;

    ret = std::strftime(date, sizeof(date), "%a, %d %b %Y %X GMT", std::localtime(&time));
	if (!ret)
	{
		/* An error occured */
		std::cerr << "Webserv error: strftime() function failed" << std::endl;
		throw (INTERNAL_SERVER_ERROR);
	}
	return(std::string(date));	
}
