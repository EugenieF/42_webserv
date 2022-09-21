#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <map>

#include "Request.hpp"

class   Response
{
    private:
	/**********************     MEMBER VARIABLES     ********************/
		std::map<int, std::string>			_statusCodes;
		std::map<std::string, std::string>	_mimeTypes;

		std::string							_response;
		t_statusCode						_statusCode;
		std::string							_statusLine;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
		Request								*_request;

    public:
	/**********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*------    Main    ------*/
        Response(Request* request);
        Response(const Response& other);
        ~Response();
        Response&    						operator=(const Response& other);

		void								generateResponse();

						/*-------   Getter  ------*/
		const std::string					getStatusMessage(int statusCode);
		const std::string					getMimeType(const std::string &ext);
		Request*							getRequest() const;
		std::string							getResponse() const;
		t_statusCode						getStatusCode() const;
		std::string							getStatusCodeStr() const;

	private:
						/*-------   Init    ------*/
		void								_initStatusCodes();
		void								_initMimeTypes();

        std::string							_generateStatusPage();
		
};

#endif
