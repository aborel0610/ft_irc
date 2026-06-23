#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include <map>
# include "Client.hpp"
# include "Channel.hpp"
# include <vector>

class Server
{
private:
	int _serverfd;
	int _port;
	std::string _password;
	std::map<int, Client> _clients;
	std::map<std::string, Channel> _channels;
	std::vector<int> _fds;
public:
	Server(/* args */);
	~Server();
	void	run();
	void	acceptClient();
	void	recieveData();
	void	sendtoClient();
	void	broadcastToChannel();
};

Server::Server(/* args */)
{
}

Server::~Server()
{
}

#endif