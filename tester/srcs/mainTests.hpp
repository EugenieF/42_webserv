#ifndef __MAINTESTS_HPP__
# define __MAINTESTS_HPP__

# define BLUE_B "\033[40;1;38;5;117m"
# define ORANGE_B "\033[40;1;38;5;216m"
# define RESET "\033[0m"

# include "gtest/gtest.h"
# include "Webserv.hpp"

# include "parsing/classParsingTest.cpp"
# include "parsing/classParsingErrorTest.cpp"
# include "parsing/parsingTest.cpp"

# include "http/ClientTest.hpp"
# include "http/classHttpTest.cpp"
# include "http/httpTest.cpp"

#endif
