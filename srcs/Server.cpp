#include "../includes/Server.hpp"

bool Server::signal = false;
void	Server::signalHandler(int signum)
{
	(void) signum;
	std::cout << "\nSignal received\n";
	Server::signal = true;
}

void	Server::run()
{
	struct sockaddr_in	sockAddr;
	struct pollfd		newPoll;

	//Create socket (interprocess communication)
	//AF_INET = IPv4, SOCK_STREAM = TCP
	int	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		throw(std::runtime_error("Error: socket()"));
	
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(this->_port);
	sockAddr.sin_addr.s_addr = INADDR_ANY; //accept connections on any IP
	if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("Error: O_NONBLOCK"));
	if (bind(sock, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) == -1)
		throw(std::runtime_error("Error: bind()"));
	if (listen(sock, SOMAXCONN) == -1)
		throw(std::runtime_error("Error: listen()"));
	
	newPoll.fd = sock;
	newPoll.events = POLLIN;
	this->_fds.push_back(newPoll);
	std::cout << "Server " << sock << "connected. Listening...\n";

	while (Server::signal == false)
	{
		if ((poll(&_fds[0], _fds.size(), -1) == -1) && Server::signal == false)
			throw(std::runtime_error("Error: poll()"));
		for (size_t i = 0; i < _fds.size(); i++)
		{
			if (_fds[i].revents & POLLIN)
			{
				if (_fds[i].fd == sock)
					acceptClient();
				else
					receiveData(_fds[i].fd);
			}
		}
	}
	closeFds();
}

