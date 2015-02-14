#ifndef HTTP_CODES_HPP_
#define HTTP_CODES_HPP_
/*
#include "http_codes.hpp"
 */


enum HttpRequestMethod
{
    GET,
    PUT,
    POST,
    DELETE,
    OPTIONS,
    HEAD,
    TRACE,
    CONNECT
};
static const int gk_maxHttpMethodCount = 8;



enum HttpStatusCode
{
    // cf. http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html

    // 100 : informational

    // 200 : success
    HTTP_STATUS_OK                         = 200,
    HTTP_STATUS_CREATED                    = 201,

    // 300 redirection

    // 400 : Client errors
    HTTP_STATUS_CLIENT_ERROR_BAD_REQUEST          = 400,
    HTTP_STATUS_CLIENT_ERROR_NOT_FOUND            = 404,
    HTTP_STATUS_CLIENT_ERROR_METHOD_NOT_ALLOWED   = 405,
    HTTP_STATUS_CLIENT_ERROR_REQUEST_URI_TOO_LONG = 414,

    // 500 : Server error
    HTTP_STATUS_SERVER_ERROR_INTERNAL_ERROR       = 500,
    HTTP_STATUS_SERVER_ERROR_NOT_IMPLEMENTED      = 501,
    HTTP_STATUS_INSUFFICIENT_STORAGE              = 507,
};



enum SimpleContentType
{
    UNKNOWN,
    XML,
    JSON,
    OTHER
};

#endif /* HTTP_CODES_HPP_ */
