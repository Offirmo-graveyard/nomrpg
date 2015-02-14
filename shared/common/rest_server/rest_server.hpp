#ifndef REST_SERVER_HPP_
#define REST_SERVER_HPP_
/*
#include "rest_server.hpp"
 */

#include "http/http_codes.hpp"

#include "delegate/delegate.hpp" // generalized callbacks

#include "url_router/url_router.hpp"




class RestRequest
{
public:

    enum HttpRequestMethod getMethod() const { return method_; }
    std::string getUri() const;
    enum SimpleContentType getContentType() const { return contentType_; }
    std::string getFullContentType() const { return fullContentType_; }

    std::string getCanonicalUri() const;

    // public member : easier
    ConstRouteRequestPtr route;

protected:
    enum HttpRequestMethod method_;
    enum SimpleContentType contentType_;
    std::string fullContentType_;
    RouteRequestPtr routeInfos_;
};



class RestResponse
{
public:
    typedef int ApiVersion;

    RestResponse():
        statusCode_(HTTP_STATUS_SERVER_ERROR_INTERNAL_ERROR),
        apiVersion_(0)
    {}


    enum HttpStatusCode getStatusCode() { return statusCode_; };

    void setStatusCode(enum HttpStatusCode code) { statusCode_ = code; };
    void setStatusCodeOk() { statusCode_ = HTTP_STATUS_OK; };
    void setFullContentType(const std::string& type) { fullContentType_ = type; }
    void setApiVersion(ApiVersion v) { apiVersion_ = v; }

    void addBodyContent(const std::string& text);
    void addBodyContent(const std::ostringstream& textStream);

protected:
    enum HttpStatusCode statusCode_;
    ApiVersion apiVersion_;
    std::string fullContentType_;
    std::string bodyContent_;
};



typedef srutil::delegate2<bool, const RestRequest&, RestResponse&> RestSpecificHandler;

class RestResourceHandler
{
public:
    typedef int InternalRsrcId;
    typedef int NumericalRsrcId;

    virtual ~RestResourceHandler() {}

    struct OperationResult
    {
        OperationResult():isImplemented(true),isSuccessful(false),resultRsrcId(-1) {}
        bool isImplemented;
        bool isSuccessful;
        InternalRsrcId resultRsrcId;
    };
    ////////////////////////////////////
    // mandatory override
    virtual std::string getUrlSegment() const = 0;

    ////////////////////////////////////
    // override strongly recommended,
    // but will work anyway (will return "not implemented")
    // in order of importance :

    // precondition : id is valid
    virtual struct OperationResult isRsrcExisting(InternalRsrcId id) const
    { OperationResult res; res.isImplemented = false; return res; }

    // precondition : id is valid
    virtual struct OperationResult dumpRsrcRepresentation(InternalRsrcId id, std::ostringstream& body) const
    { OperationResult res; res.isImplemented = false; return res; }

    virtual struct OperationResult createNewRsrc() const
    { OperationResult res; res.isImplemented = false; return res; }

    // precondition : id is valid and rsrc exists
    virtual struct OperationResult updateRsrc(InternalRsrcId id, void* dataTODO) const
    { OperationResult res; res.isImplemented = false; return res; }

    // precondition : id is valid and rsrc exists
    virtual struct OperationResult deleteRsrc(InternalRsrcId id) const
    { OperationResult res; res.isImplemented = false; return res; }

    virtual struct OperationResult convertAlphanumericalIdToInternalId(const std::string& stringId) const
    { OperationResult res; res.isImplemented = false; return res; }

    // 0 so will always fail by default
    virtual NumericalRsrcId getMaxAllowedNumericalId() const { return 0; }

    // 0 so will always fail by default
    virtual size_t getMaxAlphanumericalIdSize() const { return 0; }

    // 0 = useless by default
    virtual RestResponse::ApiVersion getApiVersion() const { return 0; }

    // not allowed by default (safer)
    virtual bool isCreationWithPutAllowed() const { return false; }


    ////////////////////////////////////
    // optional override,
    // default implementation should work in most cases

    virtual bool isNumericalIdAcceptable(NumericalRsrcId id) const { return (id >= 0 and id < getMaxAllowedNumericalId()); }
    virtual bool isAlphanumericalIdAcceptable(const std::string& id) const { return id.length() < getMaxAlphanumericalIdSize(); }
    virtual struct OperationResult convertNumericalIdToInternalId(int externalId) const;
    virtual struct OperationResult getRequestedRsrcInternalId(const RestRequest& request);


protected:
    void handleCommon(const RestRequest& request, RestResponse& response);
    void handleGet   (const RestRequest& request, RestResponse& response);
    void handlePost  (const RestRequest& request, RestResponse& response);
    void handleDelete(const RestRequest& request, RestResponse& response);
    void handlePut   (const RestRequest& request, RestResponse& response);
    struct OperationResult locateRsrcAndHandlePotentialProblems(const RestRequest& request, RestResponse& response);
};
typedef spns::shared_ptr<RestResourceHandler> RestResourceHandlerPtr;


class RestServer
{
public:
    RestServer() {}
    virtual ~RestServer() {}

    void addSpecificRequestHandler(enum HttpRequestMethod m, const std::string& route, RestSpecificHandler handler);
    void addRsrcHandler(RestResourceHandlerPtr handler, const std::string& specificRoute = "");

protected:

    BaseUrlRouter urlRouter_;
};




#endif /* REST_SERVER_HPP_ */
