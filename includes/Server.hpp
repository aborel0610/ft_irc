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
	int			fd;
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
	void	join(t_message message);
	
	//Channel operator commands
	void	kick(t_message message);
	void	invite(t_message message);
	void	topic(t_message message);
	void	mode(t_message message);
};

Server::Server(/* args */)
{
}

Server::~Server()
{
}

#endif