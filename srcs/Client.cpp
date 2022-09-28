#include "Client.hpp"

/* All the operations :
    - accepting connections from clients
    - reading
    - writing data
are done in a non-blocking manner. */

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/

Client::Client() {}

Client::Client(Client::listOfServers servers):
    _servers(servers),
    _request(0),
    _response(0)
{}

Client::Client(Client const& other):
    _servers(other.getServers()),
    _request(other.getRequest()),
    _response(other.getResponse())
{
    *this = other;
}

Client::~Client()
{
    if (_request)
        delete _request;
    if (_response)
        delete _response;
}

Client&     Client::operator=(Client const& other)
{
    if (this != &other)
    {
        _servers = other.getServers();
        _request = other.getRequest();
        _response = other.getResponse();
    }
    return (*this);
}

t_requestStatus     Client::parseRequest(std::string const& buffer)
{
    t_requestStatus requestStatus;

    if (!_request)
        _request = new Request(buffer);
    else
        _request->completeRequest(buffer);
    requestStatus = _request->parseRequest();
    return (requestStatus);
}

std::string     Client::generateResponse()
{
    Block*  matchingServer;

    matchingServer = findMatchingServer(_request->getHost());
    if (_response)
        delete _response;
    _response = new Response(matchingServer, _request);
    _response->generateResponse();
    return (_response->getResponse());
}

void	Client::clear()
{
	if (_request)
	{
		delete _request;
		_request = NULL;
	}
	if (_response)
	{
		delete _response;
		_response = NULL;
	}
}

void	Client::_parseHostHeader(std::string const& hostHeader, std::string& host, int& port)
{
	size_t	pos;

	host = hostHeader;
	port = UNDEFINED_PORT;
	pos = hostHeader.find(":");
	if (pos != std::string::npos)
		host = host.substr(0, pos);
	if (pos + 1 != std::string::npos)
	{
		port = atoi(hostHeader.substr(pos + 1).c_str());
		std::cout << "**************************" << std::endl;
	}
}

Client::listOfServers	Client::_evaluateServerListen(std::string const& host, int const& port)
{
	listOfServers				matchingServers;
	listOfServers::iterator		currentServer;

    for (currentServer = _servers.begin(); currentServer != _servers.end(); currentServer++)
    {
		if (host == (*currentServer)->getHost() && (port == (*currentServer)->getPort() || port == UNDEFINED_PORT))
		{
			matchingServers.push_back(*currentServer);
			std::cout << BLUE << "host: '" << host << "' | serverHost: '" << (*currentServer)->getHost() << "'" << RESET << std::endl;
		}
    }
	return (matchingServers);
}

Block*	Client::_evaluateServerNames(listOfServers matchingServers, std::string const& host)
{
	listOfServers::iterator			currentServer;
	Block::listOfStrings::iterator	currentName;

	for (currentServer = matchingServers.begin(); currentServer != matchingServers.end(); currentServer++)
	{
    	for (currentName = (*currentServer)->getServerNames().begin(); currentName != (*currentServer)->getServerNames().end(); currentName++)
		{
			if (host == *currentName)
				return (*currentServer);
		}
	}
    return (matchingServers[0]);
}

Block*  Client::findMatchingServer(std::string hostHeader)
{
	Block*							matchingServer;
	listOfServers					matchingServers;
	std::string						host;
	int								port;

	// Check listen -> host and listen
	// If multiple -> check server_names
	// Default server
	
	matchingServer = _servers[0];
	_parseHostHeader(hostHeader, host, port);
	matchingServers = _evaluateServerListen(host, port);
	std::cout << "size : " << matchingServers.size() << std::endl;
	if (matchingServers.size() == 1)
		matchingServer = matchingServers.front();
	if (matchingServers.size() > 1)
		matchingServer = _evaluateServerNames(matchingServers, host);
	std::cout << YELLOW << "Host: " << host << " | Port: " << port << std::endl;
	std::cout << "Matching server first name : " << matchingServer->getServerNames()[0] << RESET << std::endl;
	return (matchingServer);
}

/******************************************************************************/
/*                                  GETTER                                    */
/******************************************************************************/

Client::listOfServers   Client::getServers() const
{
    return (_servers);
}

Request*    Client::getRequest() const
{
    return (_request);
}

Response*   Client::getResponse() const
{
    return (_response);
}
