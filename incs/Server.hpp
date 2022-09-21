#ifndef SERVER_HPP
# define SERVER_HPP

/******************************************************************************/
/*                                CLASS SERVER                                */
/******************************************************************************/

class Server
{
	private:
	/**********************     MEMBER VARIABLES     ********************/
		int			_fd;

	public:
	/**********************  PUBLIC MEMBER FUNCTIONS  *******************/
		Server();
		~Server();

						/*-------   Getter   ------*/
		int			getFd() const;
};

#endif