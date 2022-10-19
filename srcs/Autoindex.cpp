#include "Autoindex.hpp"

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/

Autoindex::Autoindex(const std::string& path):
    _directoryPath(""),
    _indexPage("")
{
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
        	<h1>Index of " + _directoryPath + "</h1>\n\
            <table>\n\
				<colgroup span=\"20\"></colgroup>\n";
	return (header);
}

std::string    Autoindex::_generateHtmlFooter()
{
	std::string	footer;

	footer = "\t\t\t</table>\n\
		</body>\n\
	</html>\n";
	return (footer);
}

std::string		Autoindex::_getFileLink(const unsigned char fileType, std::string fileName)
{
	std::string	fileLink;

	fileLink = "<a href=\"" + fileName;
	if (fileType == DT_DIR) /* file is directory */
		fileLink += "/";
	fileLink += "\">" + fileName + "</a>";
	_formatCell(&fileLink);
	return (fileLink);
}

// file link + file info
std::string		Autoindex::_generateHtmlLink(const unsigned char fileType, const std::string& fileName)
{
	struct stat		fileInfos;
	std::string		link;
	std::string		filePath(_directoryPath + fileName);

	if (fileName == "." || fileName == ".." || stat(filePath.c_str(), &fileInfos) != 0)
		return ("");
	link = "\t\t\t<tr>\n";
	link += _getFileLink(fileType, fileName);
	link += _getFileModificationTime(fileInfos);
	link += _getFileSize(fileInfos);
	link += "\t\t\t</tr>\n";
	return (link);
}

void    Autoindex::_generateIndexPage()
{
	DIR*            directory;
	struct dirent*	file;

	directory = opendir(_directoryPath.c_str());
	if (!directory)
	{
		/* error case: unable to read directory */
		throw (INTERNAL_SERVER_ERROR);
	}
    _indexPage += _generateHtmlHeader();
	for (file = readdir(directory); file != NULL; file = readdir(directory))
	{
		/* We generate a link for all files in the directory */
		_indexPage += _generateHtmlLink(file->d_type, std::string(file->d_name));
	}
	_indexPage += _generateHtmlFooter();
    closedir(directory);
}

/******************************************************************************/
/*                                   UTILS                                    */
/******************************************************************************/

void	Autoindex::_formatCell(std::string* data)
{
	*data = "\t\t\t\t<td>" + *data + "</td>\n";
}

std::string		Autoindex::_getFileSize(struct stat	fileInfos)
{
	std::string		size;

	if (fileInfos.st_mode & S_IFDIR) /* Is directory */
		size = "-";
	else
		size = convertSizeToString(fileInfos.st_size);
	_formatCell(&size);
	return (size);
}

std::string		Autoindex::_getFileModificationTime(struct stat	fileInfos)
{
	std::string		time;

	time = getFormattedDate(fileInfos.st_mtime);
	_formatCell(&time);
	return(time);
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
