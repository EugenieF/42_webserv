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

Client::Client(serverMapNode server, int sockfd):
	_sockfd(sockfd),
    _runningServer(server),
    _request(0),
    _response(0)
{}

Client::Client(Client const& other)
{
    *this = other;
}

Client::~Client()
{
    clear();
}

Client&     Client::operator=(const Client& other)
{
    if (this != &other)
    {
		_sockfd = other.getFd();
        _runningServer = other.getRunningServer();
        _request = other.getRequest();
        _response = other.getResponse();
    }
    return (*this);
}

/******************************************************************************/
/*                                 REQUEST                                    */
/******************************************************************************/

t_requestStatus     Client::parseRequest(const std::string& buffer)
{
    t_requestStatus requestStatus;

    if (!_request)
			_request = new Request(buffer);
    else
        _request->completeRequest(buffer);
    requestStatus = _request->parseRequest();
    return (requestStatus);
}

/******************************************************************************/
/*                                 RESPONSE                                   */
/******************************************************************************/

#ifndef COOKIE
std::string     Client::generateResponse()
{
	if (_response)
		delete _response;
	_response = new Response(_selectVirtualServer(), _request);
	_response->generateResponse();
	return (_response->getResponse());
}
#else
std::string     Client::generateResponse()
{
    Cookie*		cookies;

	if (_response)
		delete _response;
	cookies = _runningServer.first->getSessionCookies(_request->getCookies());
	_response = new Response(_selectVirtualServer(), _request, *cookies);
	_response->generateResponse();
	return (_response->getResponse());
}
#endif

bool	Client::_matchingServerName(const listOfStrings& serverNames, int listeningPort)
{
	listOfStrings::const_iterator	currentName;
	int								requestedPort;
	
	requestedPort = _request->getPort();
    for (currentName = serverNames.begin(); currentName != serverNames.end(); currentName++)
	{
		if (_request->getHost() == *currentName
			&& (requestedPort == UNDEFINED_PORT || requestedPort == listeningPort))
			return (true);
	}
    return (false);
}

Block*  Client::_selectVirtualServer()
{
	listOfServers				virtualHosts;
	listOfServers::iterator		currentServer;

	virtualHosts = _runningServer.second->getVirtualHosts();
    if (virtualHosts.empty())
		return (_runningServer.second);
	for (currentServer = virtualHosts.begin(); currentServer != virtualHosts.end(); currentServer++)
	{
		if (_matchingServerName((*currentServer)->getServerNames(), (*currentServer)->getPort()))
			return (*currentServer);
	}
	return (_runningServer.second);
}

/******************************************************************************/
/*                                  GETTER                                    */
/******************************************************************************/

Client::serverMapNode	Client::getRunningServer() const
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

int	Client::getFd() const {
	return (_sockfd);
}

/******************************************************************************/
/*                                   UTILS                                    */
/******************************************************************************/

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
