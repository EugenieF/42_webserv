#include "../mainTests.hpp"

/**************************************************************/
/*                     CLASS PARSING TEST                     */
/**************************************************************/

typedef enum e_directive
{
	AUTOINDEX,
	CGI,
	ERROR_PAGE,
	REDIRECT,
	BODY_LIMIT,
	ALLOWED_METHOD,
	UPLOAD_PATH,
}	t_directive;

class parsingTest: public ::testing::Test
{
	protected:
		Webserv								webservSimple;
		// std::map<t_directive, std::string>	expectedResult;

	void	SetUp() override
	{
		webservSimple.parse("testFiles/basic.conf");

	}
};
