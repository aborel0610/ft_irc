#ifndef SERVER_HPP
# define SERVER_HPP

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
	void	init();
	void	run();
	void	acceptClient();
	void	receiveData(int fd);
	void	sendtoClient();
	void	broadcastToChannel();

	//Signal Handler + clean exit
	static void	signalHandler(int signum);
	void		closeFds();
};

Server::Server(/* args */)
{
}

Server::~Server()
{
}

#endif