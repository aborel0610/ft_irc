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
	_serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverfd < 0)
		throw(std::runtime_error("Error: socket()"));
	
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(this->_port);
	sockAddr.sin_addr.s_addr = INADDR_ANY; //accept connections on any IP
	if (fcntl(_serverfd, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("Error: O_NONBLOCK"));
	if (bind(_serverfd, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) == -1)
		throw(std::runtime_error("Error: bind()"));
	if (listen(_serverfd, SOMAXCONN) == -1)
		throw(std::runtime_error("Error: listen()"));
	
	newPoll.fd = _serverfd;
	newPoll.events = POLLIN;
	this->_fds.push_back(newPoll);
	std::cout << "Server " << _serverfd << "connected. Listening...\n";

	while (Server::signal == false)
	{
		if ((poll(&_fds[0], _fds.size(), -1) == -1) && Server::signal == false)
			throw(std::runtime_error("Error: poll()"));
		for (size_t i = 0; i < _fds.size(); i++)
		{
			if (_fds[i].revents & POLLIN)
			{
				if (_fds[i].fd == _serverfd)
					acceptClient();
				else
					receiveData(_fds[i].fd);
			}
		}
	}
	closeFds();
}

void	Server::acceptClient()
{
	Client c = Client();
	struct sockaddr_in	cliAddr;
	struct pollfd		newPoll;
	socklen_t len = sizeof(cliAddr);

	int sock = accept(_serverfd, (sockaddr *)&(cliAddr), &len);
	if (sock < 0)
		throw(std::runtime_error("Error: accept()"));
	newPoll.fd = sock;
	newPoll.events = POLLIN;
	newPoll.revents = 0;
	//c.setFd(sock);
	_clients.insert(std::pair(sock, c));
	_fds.push_back(newPoll);
}

void	Server::receiveData(int fd)
{
	char	buff[1024];
	std::memset(buff, 0, sizeof(buff));
	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1, 0);

	if (bytes <= 0)
	{
		std::cout << "Client <" << fd << "> Disconnected\n";
		clearClient(fd);
	}
	else
	{
		buff[bytes] = 0;
		t_message message = parse_message(buff);
		//Check prefix is real
	}
}
int	skipwhitespace(std::string str)
{
	int	i = 0;
	while (isspace(str[i]))
		i++;
	return i;
}
t_message	Server::parse_message(char* buff)
{
	t_message message;

	int		i = 0;
	int		pos;
	std::string	str = (std::string)buff;

	std::memset(message.args, 0, sizeof(message.args));
	if (buff[i] == ':')
	{
		i++;
		pos = i;
		while (str[pos] && !isspace(str[pos]))
			pos++;
		message.prefix = str.substr(1, pos - 2);
		i = skipwhitespace(&str[pos]);
	}
	pos = i;
	while (str[pos] && !isspace(str[pos]))
		pos++;
	message.cmd = str.substr(i, pos - i - 1);
	i = skipwhitespace(&str[pos]);
	for (int j = 0; j < 15; j++)
	{
		while (str[pos] && !isspace(str[pos]))
			pos++;
		message.args[j] = str.substr(i, pos - i - 1);
		if (str[pos] == 0)
			break;
		i = skipwhitespace(&str[pos]);
	}
}