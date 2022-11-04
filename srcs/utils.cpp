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

/* Doesn't work with large uploads, I don't know why... */

// std::string readFd(int fd) {
// 	std::string     str;
// 	char            buf[BUFSIZE + 1];
// 	ssize_t         count;

// 	count = read(fd, buf, BUFSIZE);
// 	while (count) {
// 		std::cout << GREEN << "count = " << count << GREEN << NL;
// 		if (count < 0)
// 			throw std::runtime_error("readFd (recv) error");
// 		buf[count] = 0;
// 		str.insert(str.size(), buf, count);
// 		if (count == BUFSIZE) {
// 			count = read(fd, buf, BUFSIZE);
// 		} else {
// 			break ;
// 		}
// 	}
// 	return (str);
// }

std::string readFd(int fd) {
	std::string     str;
	char            buf[BUFSIZE + 1];
	ssize_t         count;

	count = read(fd, buf, BUFSIZE);
	if (count < 0)
		throw std::runtime_error("readFd (read) error");
	buf[count] = 0;
	str.insert(str.size(), buf, count);
	return (str);
}

void	writeFd(int fd, const char* buf, const ssize_t len) {
	ssize_t		written_char = 0;
	ssize_t		ret = 0;

	/* TMP */
	ret = write(fd, buf, len);
	if (ret < 0)
		throw std::runtime_error("writeFd (write) error");
	// std::cerr << RED << " **Wanted write: " << len << NL
	// 	<< " **Actual written: " << ret << RESET<< NL;
	return ;

	/* Actual function */
	DEBUG("Writing len expected:" + convertNbToString(len));
	while (written_char < len) {
		ret = write(fd, buf + written_char, len - written_char);
		std::cout << RED << buf + written_char << RESET << NL;
		DEBUG("  written char: " + convertNbToString(ret));
		if (ret < 0) {
			//DEBUG("ERROR: ret is " + convertNbToString(ret)
			//	+ " (left: " + convertNbToString(len-written_char)
			//	+ ")");
			DEBUG("ERROR: left char = "
				+ convertNbToString(len-written_char) + ")");
			throw std::runtime_error("writeFd (write) error");
		}
		written_char += ret;
	}
	DEBUG("Actual len:" + convertNbToString(written_char));
}

// ssize_t	writeFd(int fd, const char* buf, const ssize_t len) {
// 	ssize_t		ret = 0;

// 	/* TMP */
// 	ret = write(fd, buf, len);
// 	if (ret < 0)
// 		throw std::runtime_error("writeFd (write) error");
// 	// std::cerr << RED << " **Wanted write: " << len << NL
// 	// 	<< " **Actual written: " << ret << RESET<< NL;
// 	return (ret);
// }

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

std::string	generateCopyFilename(const std::string& dir, const std::string& file) {
	const size_t			ext_pos = file.rfind('.');
	const std::string		extension = file.substr(ext_pos);
	std::string				filename = file.substr(0, ext_pos);
	if (filename.empty())
		filename = extension;
	int						last_copy = 1;
	// std::string				full_filepath_copy = dir + "/" + filename + " (1)" + extension;
	if (*(dir.rbegin()) != '/' && *(filename.begin()) != '/')
		filename.insert(0, "/");
	std::string full_filepath_copy = dir + filename + " (1)" + extension;
	while (pathIsAccessible(full_filepath_copy)) {
		const std::string	num = convertNbToString(++last_copy);
		full_filepath_copy = dir + filename + " (" + num + ")" + extension;
	}
	return (full_filepath_copy);
}

/******************************************************************************/
/*                                  DISPLAY                                   */
/******************************************************************************/

void	displayMsg(const std::string& msg, const char* colorCode)
{
	std::cout << colorCode << msg << RESET << std::endl;
}

std::string		extractStatusLine(const std::string& str)
{
	std::string	statusLine;

	statusLine = str.substr(0, str.find("\r\n"));
	return(statusLine);
}

/******************************************************************************/
/*                                     TIME                                   */
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
