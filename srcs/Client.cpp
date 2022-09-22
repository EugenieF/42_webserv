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
    if (_request)
        delete _request;
    if (_response)
        delete _response;
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
        _request = new Request(_runningServer, buffer);
    else
        _request->completeRequest(buffer);
    requestStatus = _request->parseRequest();
    return (requestStatus);
}

std::string     Client::generateResponse()
{
    if (_response)
        delete _response;
    _response = new Response(_request);
    _response->generateResponse();
	if (_request)
	{
		delete _request;
		_request = NULL;
	}
    return (_response->getResponse());
}

/******************************************************************************/
/*                                  GETTER                                    */
/******************************************************************************/

Block*  Client::getRunningServer() const
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
