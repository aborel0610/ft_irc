#include "../includes/Server.hpp"

void	Server::run()
{
	struct sockaddr_in	sockAddr;
	struct pollfd		poll;

	//Create socket (interprocess communication)
	//AF_INET = IPv4, SOCK_STREAM = TCP
	int	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		throw(std::runtime_error("Error creating socket"));
	
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(this->_port);
	sockAddr.sin_addr.s_addr = INADDR_ANY; //accept connections on any IP
	if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("Error adding O_NONBLOCK flag to socket"));
	if (bind(sock, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) == -1)
		throw(std::runtime_error("Error bind socket to address"));
	if (listen(sock, SOMAXCONN) == -1)
		throw(std::runtime_error("Error listening"));
	
	poll.fd = sock;
	poll.events = POLLIN;
	this->_fds.push_back(poll);
}