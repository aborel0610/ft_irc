#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <iostream>
# include <map>
# include "Server.hpp"
# include "Channel.hpp"
# include <vector>

class Client
{
private:
	int	_fd;
	std::string _nickname;
	std::string _username;
	std::string	_buffer;
	bool _passOk;
	bool _nickOk;
	bool _userOk;
public:
	Client(/* args */);
	~Client();
	void	isRegistered();
	void	appendBuffer();
};

Client::Client(/* args */)
{
}

Client::~Client()
{
}

#endif