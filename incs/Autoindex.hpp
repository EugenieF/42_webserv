#ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP

# include <dirent.h>

# include "utils.hpp"

class Autoindex
{
    private:
        std::string     _directoryPath;
        std::string     _indexPage;

    public:
        Autoindex(const std::string& path);
        Autoindex(const Autoindex& other);
        ~Autoindex();
        Autoindex&      operator=(const Autoindex& other);

        std::string     getIndexPage() const;
        std::string     getDirectoryPath() const;

    private:
        void            _generateIndexPage();
        void            _generateHtmlHeader();
        void            _generateHtmlFooter();
};

#endif
