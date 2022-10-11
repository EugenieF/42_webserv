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