#include "Client.hpp"

/* All the operations :
    - accepting connections from clients
    - reading
    - writing data
are done in a non-blocking manner. */

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/

Client::Client(serverMapNode server, int sockfd, const Env& env):
	_sockfd(sockfd),
    _runningServer(server),
    _request(0),
    _response(0),
	_env(env)
{}

Client::Client(Client const& other)
{
    *this = other;
}

Client::~Client()
{
    clear();
	DEBUG("client destroyed: " + convertNbToString(getFd()));
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
		_request = new Request(buffer, getFd());
    else
        _request->completeRequest(buffer);
    requestStatus = _request->parseRequest();
    return (requestStatus);
}

/******************************************************************************/
/*                                 RESPONSE                                   */
/******************************************************************************/

#ifndef COOKIE
void	Client::generateResponse()
{
	if (_response)
		delete _response;
	_response = new Response(_selectVirtualServer(), _request, _env);
	_response->generateResponse();
}
#else
void	Client::generateResponse()
{
    Cookie*		cookies;

	if (_response)
		delete _response;
	cookies = _runningServer.first->getSessionCookies(_request->getCookies());
	cookies->display();
	_response = new Response(_selectVirtualServer(), _request, _env, *cookies);
	_response->generateResponse();
}
#endif

bool	Client::_matchingServerName(
	const listOfStrings& serverNames, int listeningPort, Block* currentServer)
{
	listOfStrings::const_iterator	currentName;
	int								requestedPort;
	
	requestedPort = _request->getPort();
    for (currentName = serverNames.begin(); currentName != serverNames.end(); currentName++)
	{
		if (_request->getHost() == *currentName
			&& (requestedPort == UNDEFINED_PORT || requestedPort == listeningPort))
		{
			/* Configure virtual server name */
			currentServer->setVirtualServerName(*currentName);
			return (true);
		}
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
		if (_matchingServerName((*currentServer)->getServerNames(), (*currentServer)->getPort(), *currentServer))
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

const Env&	Client::getEnv() const {
	return (_env);
}

Env&	Client::getEnv() {
	return (_env);
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
