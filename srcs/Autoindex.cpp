#include "Autoindex.hpp"

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/

Autoindex::Autoindex(const std::string& path):
    _directoryPath(""),
    _indexPage("")
{
    if (path[0] != '/')
        _directoryPath += "/"; 
    _directoryPath += path;
    _generateIndexPage();
}

Autoindex::Autoindex(const Autoindex& other):
    _directoryPath(other.getDirectoryPath()),
    _indexPage(other.getIndexPage())
{
    *this = other;
}

Autoindex::~Autoindex() {}

Autoindex&  Autoindex::operator=(const Autoindex& other)
{
    if (this != &other)
    {
        _directoryPath = other.getDirectoryPath();
        _indexPage = other.getIndexPage();
    }
    return (*this);
}

/******************************************************************************/
/*                              GENERATE HTML CODE                            */
/******************************************************************************/

std::string	    Autoindex::_generateHtmlHeader()
{
	std::string	header;

    header = "<!DOCTYPE html>\n\
    <html>\n\
        <head>\n\
            <title>Autoindex page</title>\n\
        </head>\n\
        <body>\n\
        <h1>INDEX</h1>\n\
            <p>\n";
	return (header);
}

std::string    Autoindex::_generateHtmlFooter()
{
	std::string	footer;

	footer = "</p>\n\
    </body>\n\
    </html>\n";
	return (footer);
}

std::string		Autoindex::_getFileInfos(const std::string& filePath)
{
	struct stat		s;
	std::string		fileInfos;
	std::string		time;
	std::string		size;

	if (stat(filePath.c_str(), &s) != 0)
		return ;
	time = _getFileModificationTime(s);
	size = _getFileSize(s);
	fileInfos = _formatCell(&time) + _formatCell(&size);
	return (fileInfos);
}

std::string		Autoindex::_getFileLink(const unsigned char fileType, std::string fileName)
{
	std::string	fileLink;

	fileLink = "<a href=\"" + fileName;
	if (fileType == DT_DIR) /* file is directory */
		fileLink += "/";
	fileLink += "\">" + filename + "</a>";
	_formatCell(&fileLink);
	return (fileLink);
}

// file link + file info
std::string		Autoindex::_generateHtmlLink(const unsigned char fileType, const char* name)
{
	std::string	link;
	std::string	filename(name);

	link = "<tr>\n";
	link += _getFileLink(fileType, filename);
	link += _getFileInfos(_path + fileName);
	link += "</tr>\n";
	return (link);
}

void    Autoindex::_generateIndexPage()
{
	DIR*            directory;
	struct dirent*	file;

	directory = opendir(_directoryPath.c_str());
	if (directory)
	{
		/* error case */
		std::cerr << RED << "Webserv error: opendir() failed" << RESET << std::endl;
		return ;
	}
    _indexPage += _generateHtmlHeader();
	while ((file = readdir(directory)))
	{
		/* We generate a link for all files in the directory */
		_indexPage += _generateHtmlLink(file->d_type, file->d_name);
	}
	_indexPage += _generateHtmlFooter();
    closedir(directory);
}

/******************************************************************************/
/*                                   UTILS                                    */
/******************************************************************************/

void	Autoindex::_formatCell(std::string* data)
{
	*data = "<td>" + *data + "</td>\n";
}

std::string		Autoindex::_getFileSize(struct stat	s)
{
	std::stringstream	fileSize;

	if (s.st_mode & S_IFDIR)
	{
		/* Is directory */
		return ("-");
	}
	fileSize << s.st_size;
	return (fileSize.str());
}

std::string		Autoindex::_getFileModificationTime(struct stat	s)
{
    char				date[100];
	std::stringstream	modificationTime;

	if (!std::strftime(date, sizeof(date)), "%d-%b-%Y %H:%M", std::localtime(&s.st_mtime))
	{
		/* Error */
		std::cerr << RED << "Webserv error: strftime() failed" << RESET << std::endl;
		return ("");
	}
	modificationTime << date;
	return(modificationTime.str());
}

/******************************************************************************/
/*                                  GETTER                                    */
/******************************************************************************/

std::string     Autoindex::getDirectoryPath() const
{
    return (_directoryPath);
}

std::string     Autoindex::getIndexPage() const
{
    return (_indexPage);
}
