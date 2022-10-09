#ifndef __MAINTESTS_HPP__
# define __MAINTESTS_HPP__

# define BLUE_B "\033[40;1;38;5;117m"
# define ORANGE_B "\033[40;1;38;5;216m"
# define RESET "\033[0m"


# include "Webserv.hpp"
# include "gtest/gtest.h"

# include "parsing/classParsingTest.cpp"
# include "parsing/classParsingErrorTest.cpp"
# include "parsing/parsingTest.cpp"

# include "http/classHttpTest.cpp"

#endif
