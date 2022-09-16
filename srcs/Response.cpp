#include "Response.hpp"

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/

Response::Response()
{
	generateResponse();
}

Response::Response(const Response &other)
{
	*this = other;
}

Response::~Response() {}

Response&	Response::operator=(const Response &other)
{
	if (this != &other)
	{

	}
	return (*this);
}

void	Response::generateResponse()
{
	_response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 13\n\nHello world !";
}

const char*		Response::getResponse() const
{
	return (_response.c_str());
}

const std::string	Response::getStatusMessage(int statusCode)
{
	std::map<int, std::string>::const_iterator	ite;

	ite = _statusCodes.find(statusCode);
	if (ite != _statusCodes.end())
		return ite->second;
	return ("");
}

const std::string	Response::getMimeType(const std::string &ext)
{
	std::map<std::string, std::string>::const_iterator	ite;

	ite = _mimeTypes.find(ext);
	if (ite != _mimeTypes.end())
		return ite->second;
	return (_mimeTypes[".bin"]);
}


/******************************************************************************/
/*                                   INIT                                     */
/******************************************************************************/

void	Response::initStatusCodes()
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

/* This table lists important MIME types for the Web: */
void	Response::initMimeTypes()
{
	_mimeTypes[".aac"] = "audio/aac";
	_mimeTypes[".abw"] = "application/x-abiword";
	_mimeTypes[".arc"] = "application/x-freearc";
	_mimeTypes[".avif"] = "image/avif";
	_mimeTypes[".avi"] = "video/x-msvideo";
	_mimeTypes[".azw"] = "application/vnd.amazon.ebook";
	_mimeTypes[".bin"] = "application/octet-stream";
	_mimeTypes[".bmp"] = "image/bmp";
	_mimeTypes[".bz"] = "application/x-bzip";
	_mimeTypes[".bz2"] = "application/x-bzip2";
	_mimeTypes[".cda"] = "application/x-cdf";
	_mimeTypes[".csh"] = "application/x-csh";
	_mimeTypes[".css"] = "text/css";
	_mimeTypes[".csv"] = "text/csv";
	_mimeTypes[".doc"] = "application/msword";
	_mimeTypes[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	_mimeTypes[".eot"] = "application/vnd.ms-fontobject";
	_mimeTypes[".epub"] = "application/epub+zip";
	_mimeTypes[".gz"] = "application/gzip";
	_mimeTypes[".gif"] = "image/gif";
	_mimeTypes[".htm"] = "text/html";
	_mimeTypes[".html"] = "text/html";
	_mimeTypes[".ico"] = "image/vnd.microsoft.icon";
	_mimeTypes[".ics"] = "text/calendar";
	_mimeTypes[".jar"] = "application/java-archive";
	_mimeTypes[".jpeg"] = "image/jpeg";
	_mimeTypes[".jpg"] = "image/jpeg";
	_mimeTypes[".js"] = "text/javascript";
	_mimeTypes[".json"] = "application/json";
	_mimeTypes[".jsonld"] = "application/ld+json";
	_mimeTypes[".mid"] = "audio/midi";
	_mimeTypes[".midi"] = "audio/x-midi";
	_mimeTypes[".mjs"] = "text/javascript";
	_mimeTypes[".mp3"] = "audio/mpeg";
	_mimeTypes[".mp4"] = "video/mp4";
	_mimeTypes[".mpeg"] = "video/mpeg";
	_mimeTypes[".mpkg"] = "application/vnd.apple.installer+xml";
	_mimeTypes[".odp"] = "application/vnd.oasis.opendocument.presentation";
	_mimeTypes[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	_mimeTypes[".odt"] = "application/vnd.oasis.opendocument.text";
	_mimeTypes[".oga"] = "audio/ogg";
	_mimeTypes[".ogv"] = "video/ogg";
	_mimeTypes[".ogx"] = "application/ogg";
	_mimeTypes[".opus"] = "audio/opus";
	_mimeTypes[".otf"] = "font/otf";
	_mimeTypes[".png"] = "image/png";
	_mimeTypes[".pdf"] = "application/pdf";
	_mimeTypes[".php"] = "application/x-httpd-php";
	_mimeTypes[".ppt"] = "application/vnd.ms-powerpoint";
	_mimeTypes[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	_mimeTypes[".rar"] = "application/vnd.rar";
	_mimeTypes[".rtf"] = "application/rtf";
	_mimeTypes[".sh"] = "application/x-sh";
	_mimeTypes[".svg"] = "image/svg+xml";
	_mimeTypes[".tar"] = "application/x-tar";
	_mimeTypes[".tif"] = "image/tiff";
	_mimeTypes[".tiff"] = "image/tiff";
	_mimeTypes[".ts"] = "video/mp2t";
	_mimeTypes[".ttf"] = "font/ttf";
	_mimeTypes[".txt"] = "text/plain";
	_mimeTypes[".vsd"] = "application/vnd.visio";
	_mimeTypes[".wav"] = "audio/wav";
	_mimeTypes[".weba"] = "audio/webm";
	_mimeTypes[".webm"] = "video/webm";
	_mimeTypes[".webp"] = "image/webp";
	_mimeTypes[".woff"] = "font/woff";
	_mimeTypes[".woff2"] = "font/woff2";
	_mimeTypes[".xhtml"] = "application/xhtml+xml";
	_mimeTypes[".xls"] = "application/vnd.ms-excel";
	_mimeTypes[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	_mimeTypes[".xml"] = "application/xml";
	_mimeTypes[".xul"] = "application/vnd.mozilla.xul+xml";
	_mimeTypes[".zip"] = "application/zip";
	_mimeTypes[".3gp"] = "video/3gpp";
	_mimeTypes[".3g2"] = "video/3gpp2";
	_mimeTypes[".7z"] = "application/x-7z-compressed";
}
