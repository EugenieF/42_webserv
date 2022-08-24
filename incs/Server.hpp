#ifndef SERVER_HPP
# define SERVER_HPP

class Server
{
	private:
		int		_fd;

	public:
		Server();
		~Server();
		int	getFd() const;
};

#endif