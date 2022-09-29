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
/*                                INDEX PAGE                                  */
/******************************************************************************/

void    Autoindex::_generateHtmlHeader()
{
    _indexPage = "<!DOCTYPE html>\n\
    <html>\n\
        <head>\n\
            <title>Autoindex page</title>\n\
        </head>\n\
        <body>\n\
        <h1>INDEX</h1>\n\
            <p>\n";
}

void    Autoindex::_generateHtmlFooter()
{
    _indexPage += "</p>\n\
    </body>\n\
    </html>\n";
}

void    Autoindex::_generateIndexPage()
{
	DIR*            directory;
	struct dirent*  directoryEntity;

	directory = opendir(_directoryPath.c_str());
	if (directory) /* error case */
		return ;
    _generateHtmlHeader();
	while ((directoryEntity = readdir(directory)))
    {

    }
    _generateHtmlFooter();
    closedir(directory);
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
