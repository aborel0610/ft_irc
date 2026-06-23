#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <iostream>
# include <set>
# include "Server.hpp"
# include "Client.hpp"
# include <vector>

class Channel
{
private:
	std::string _name;
	std::string _topic;
	std::set<int> _members;
	std::set<int> _operators;
	std::set<int> _invited;
	bool _inviteOnly;
	bool _topicRestricted;
	int _userLimit;
public:
	Channel(/* args */);
	~Channel();
	void	addMember();
	void	removeMember();
	bool	isOperator();
};

Channel::Channel(/* args */)
{
}

Channel::~Channel()
{
}

#endif