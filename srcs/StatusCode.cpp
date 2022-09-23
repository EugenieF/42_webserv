#include "StatusCode.hpp"

StatusCode::StatusCode()
{
	_initStatusCodesMap();
}

StatusCode::~StatusCode() {}

std::string&	StatusCode::operator[](int statusCode)
{
	std::map<int, std::string>::iterator	ite;
	std::string								message;

	message = "";
	ite = _statusCodes.find(statusCode);
	if (ite != _statusCodes.end())
		message = ite->second;
	return (message);
}

void	StatusCode::_initStatusCodesMap()
{
/* Informational response */	
	_statusCodes[CONTINUE] = "Continue";
	_statusCodes[SWITCHING_PROTOCOLS] = "Switching Protocols";
	_statusCodes[PROCESSING] = "Processing";
	_statusCodes[EARLY_HINTS] = "Early Hints";
/* Success */
	_statusCodes[OK] = "OK";
	_statusCodes[CREATED] = "Created";
	_statusCodes[ACCEPTED] = "Accepted";
	_statusCodes[NON_AUTHORITATIVE_INFORMATION] = "Non-Authoritative Information";
	_statusCodes[NO_CONTENT] = "No Content";
	_statusCodes[RESET_CONTENT] = "Reset Content";
	_statusCodes[PARTIAL_CONTENT] = "Partial Content";
	_statusCodes[MULTI_STATUS] = "Multi-Status";
	_statusCodes[ALREADY_REPORTED] = "Already Reported";
	_statusCodes[IM_USED] = "IM Used";
/* Redirection */
	_statusCodes[MULTIPLE_CHOICES] = "Multiple Choices";
	_statusCodes[MOVED_PERMANENTLY] = "Moved Permanently";
	_statusCodes[FOUND] = "Found";
	_statusCodes[SEE_OTHER] = "See Other";
	_statusCodes[NOT_MODIFIED] = "Not Modified";
	_statusCodes[USE_PROXY] = "Use Proxy";
	_statusCodes[SWITCH_PROXY] = "Switch Proxy";
	_statusCodes[TEMPORARY_REDIRECT] = "Temporary Redirect";
	_statusCodes[PERMANENT_REDIRECT] = "Permanent Redirect";
/* Client errors */
	_statusCodes[BAD_REQUEST] = "Bad Request";
	_statusCodes[UNAUTHORIZED] = "Unauthorized";
	_statusCodes[PAYMENT_REQUIRED] = "Payment Required";
	_statusCodes[FORBIDDEN] = "Forbidden";
	_statusCodes[NOT_FOUND] = "Not Found";
	_statusCodes[METHOD_NOT_ALLOWED] = "Method Not Allowed";
	_statusCodes[NOT_ACCEPTABLE] = "Not Acceptable";
	_statusCodes[PROXY_AUTHENTICATION_REQUIRED] = "Proxy Authentication Required";
	_statusCodes[REQUEST_TIMEOUT] = "Request Timeout";
	_statusCodes[CONFLICT] = "Conflict"; 
	_statusCodes[GONE] = "Gone";
	_statusCodes[LENGTH_REQUIRED] = "Length Required";
	_statusCodes[PRECONDITION_FAILED] = "Precondition Failed";
	_statusCodes[PAYLOAD_TOO_LARGE] = "Payload Too Large";
	_statusCodes[URI_TOO_LONG] = "URI Too Long";
	_statusCodes[UNSUPPORTED_MEDIA_TYPE] = "Unsupported Media Type";
	_statusCodes[RANGE_NOT_SATISFIABLE] = "Range Not Satisfiable";
	_statusCodes[EXPECTATION_FAILED] = "Expectation Failed";
	_statusCodes[IM_A_TEAPOT] = "I'm a teapot";
	_statusCodes[MISDIRECTED_REQUEST] = "Misdirected Request";
	_statusCodes[UNPROCESSABLE_ENTITY] = "Unprocessable Entity";
	_statusCodes[LOCKED] = "Locked";
	_statusCodes[FAILED_DEPENDENCY] = "Failed Dependency";
	_statusCodes[TOO_EARLY] = "Too Early";
	_statusCodes[UPGRADE_REQUIRED] = "Upgrade Required";
	_statusCodes[PRECONDITION_REQUIRED] = "Precondition Required";
	_statusCodes[TOO_MANY_REQUESTS] = "Too Many Requests";
	_statusCodes[REQUEST_HEADER_FIELDS_TOO_LARGE] = "Request Header Fields Too Large";
	_statusCodes[UNAVAILABLE_FOR_LEGAL_REASONS] = "Unavailable For Legal Reasons";
/* Server errors */
	_statusCodes[INTERNAL_SERVER_ERROR] = "Internal Server Error";
	_statusCodes[NOT_IMPLEMENTED] = "Not Implemented";
	_statusCodes[BAD_GATEWAY] = "Bad Gateway";
	_statusCodes[SERVICE_UNAVAILABLE] = "Service Unavailable";
	_statusCodes[GATEWAY_TIMEOUT] = "Gateway Timeout";
	_statusCodes[HTTP_VERSION_NOT_SUPPORTED] = "HTTP Version Not Supported";
	_statusCodes[VARIANT_ALSO_NEGOTIATES] = "Variant Also Negotiates";
	_statusCodes[INSUFFICIENT_STORAGE] = "Insufficient Storage";
	_statusCodes[LOOP_DETECTED] = "Loop Detected";
	_statusCodes[NOT_EXTENDED] = "Not Extended";
	_statusCodes[NETWORK_AUTHENTICATION_REQUIRED] = "Network Authentication Required";
}
