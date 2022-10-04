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

Client::Client(Block* server):
    _runningServer(server),
    _request(0),
    _response(0)
{}

Client::Client(Client const& other):
    _runningServer(other.getRunningServer()),
    _request(other.getRequest()),
    _response(other.getResponse())
{
    *this = other;
}

Client::~Client()
{
    clear();
}

Client&     Client::operator=(Client const& other)
{
    if (this != &other)
    {
        _runningServer = other.getRunningServer();
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
    if (_response)
        delete _response;
    _response = new Response(selectVirtualServer(), _request);
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

bool	Client::_matchingServerName(listOfStrings serverNames, int listeningPort)
{
	listOfStrings::iterator	currentName;
	int						requestedPort;
	
	requestedPort = _request->getPort();
    for (currentName = serverNames.begin(); currentName != serverNames.end(); currentName++)
	{
		if (_request->getHost() == *currentName
			&& (requestedPort == UNDEFINED_PORT || requestedPort == listeningPort))
			return (true);
	}
    return (false);
}

Block*  Client::selectVirtualServer()
{
	listOfServers				virtualHosts;
	listOfServers::iterator		currentServer;

	virtualHosts = _runningServer->getVirtualHosts();
	if (virtualHosts.empty())
		return (_runningServer);
	for (currentServer = virtualHosts.begin(); currentServer != virtualHosts.end(); currentServer++)
	{
		if (_matchingServerName((*currentServer)->getServerNames(), (*currentServer)->getPort()))
		{
			std::cout << YELLOW << "---- Matching server name ----" << RESET << std::endl;
			return (*currentServer);
		}
	}
	return (_runningServer);
}

/******************************************************************************/
/*                                  GETTER                                    */
/******************************************************************************/

Block*	Client::getRunningServer() const
{
    return (_runningServer);
}

Request*    Client::getRequest() const
{
    return (_request);
}

Response*   Client::getResponse() const
{
    return (_response);
}
