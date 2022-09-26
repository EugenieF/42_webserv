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

Block*  Client::findMatchingServer(std::string requestedHost)
{
    listOfServers::iterator  	 	ite;
	std::vector<Block*>				matchingServers;
	size_t							pos;
	std::string						serverHost;
	int								serverPort;

	// Check listen -> host and listen
	// If multiple -> check server_names
	// Default server
	pos = requestedHost.find(":");
	serverHost = requestedHost;
	serverPort = UNDEFINED_PORT;
	if (pos != std::string::npos)
	{
		serverHost = requestedHost.substr(0, pos);
		serverPort = atoi(requestedHost.substr(pos + 1).c_str());
	}
    for (ite = _servers.begin(); ite != _servers.end(); ite++)
    {
		if (serverHost == (*ite)->getHost() && (serverPort == (*ite)->getPort() || serverPort == UNDEFINED_PORT))
		{
			matchingServers.push_back(*ite);
		}
        std::cout << RED << "host : " << (*ite)->getHost() << " | port : " << (*ite)->getPort() << " | requestedHost : " << requestedHost << RESET << std::endl;
    }
    return (_servers[0]);
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
