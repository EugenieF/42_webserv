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

#include <sys/types.h>
#include <sys/socket.h>
// # include <vector>

// std::string readFd(int fd) {
// 	std::string     str;
// 	ssize_t         count;

// 	std::vector<char>	vecBuf(BUFSIZE, '\0');
// 	std::vector<char>	vecStr;

// 	count = read(fd, &vecBuf[0], vecBuf.size());
// 	while (count) {
// 		if (count < 0)
// 			throw std::runtime_error("readFd (read) error");
// 		vecStr.insert(vecStr.end(), vecBuf.begin(), vecBuf.end());
// 		if (count == BUFSIZE) {
// 			count = read(fd, &vecBuf[0], vecBuf.size());
// 		} else {
// 			break ;
// 		}
// 	}
// 	for (std::vector<char>::iterator ite = vecStr.begin(); ite != vecStr.end(); ite++)
// 	{
// 		std::cout << YELLOW << *ite;
// 		str += *ite;
// 	}
// 	std::cout << RESET << NL;
// 	return (str);
// }

std::string readFd(int fd) {
	std::string     	str;
	char            buf[BUFSIZE + 1];
	ssize_t         count;

	count = recv(fd, buf, BUFSIZE, 0);
	while (count) {
		if (count < 0)
			throw std::runtime_error("readFd (read) error");
		buf[count] = 0;
		str.insert(str.size(), buf, count);
		std::cout << RED << "count = " << count << RESET << NL;
		if (count == BUFSIZE) {
			count = recv(fd, buf, BUFSIZE, 0);
		} else {
			break ;
		}
	}
	return (str);
}

std::string		generateRandomString(size_t length)
{
	std::string		charset;
	int				pos;
	size_t			maxIndex;
	std::string		randomStr;

	charset = "0123456789abcdefghijklmnopqrstuvwxyz";
	maxIndex = charset.size() - 1;
	while(randomStr.size() != length)
	{
		pos = ((rand() % maxIndex));
		randomStr += charset.substr(pos, 1);
	}
	return (randomStr);
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
