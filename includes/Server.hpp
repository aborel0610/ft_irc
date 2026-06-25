#ifndef SERVER_HPP
# define SERVER_HPP

# include <cstring>
# include <string>
# include <iostream>
# include <map>
# include "Client.hpp"
# include "Channel.hpp"
# include <vector>
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/types.h>
# include <cstdlib>
# include <fcntl.h>
# include <poll.h>
# include <cctype>

typedef	struct s_message
{
	std::string	prefix;
	std::string	cmd;
	std::string	args[15];
}	t_message;

class Server
{
private:
	int								_serverfd;
	int								_port;
	std::string						_password;
	std::map<int, Client>			_clients;
	std::map<std::string, Channel>	_channels;
	std::vector<pollfd>				_fds;
	static bool						signal;
public:
	// Constructors, destructor
	Server(/* args */);
	~Server();

	//Member functions
	void		init();
	void		run();
	void		acceptClient();
	void		receiveData(int fd);
	void		sendtoClient();
	void		broadcastToChannel();
	t_message	parse_message(char *buff);

	//Signal Handler + clean exit
	static void	signalHandler(int signum);
	void		closeFds();
	void		clearClient(int fd);

	//Member commands
	void	join();
	
	//Channel operator commands
	void	kick();
	void	invite();
	void	topic();
	void	mode();
};

Server::Server(/* args */)
{
}

Server::~Server()
{
}

#endif