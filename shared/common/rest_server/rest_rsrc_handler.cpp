#include "rest_server.hpp"

#include <sstream>


struct RestResourceHandler::OperationResult RestResourceHandler::convertNumericalIdToInternalId(int externalId) const
{
    struct OperationResult res;

    // default implementation : no conversion
    if(!isNumericalIdAcceptable(externalId))
    {
        res.resultRsrcId = externalId;
        res.isSuccessful = true;
    }

    return res;
}

struct RestResourceHandler::OperationResult RestResourceHandler::getRequestedRsrcInternalId(const RestRequest& request)
{
    switch(request.route->getLastIdType())
    {
    case RouteRequest::NUMERICAL:
        return convertNumericalIdToInternalId(request.route->getLastNumericalId());
        break;
    case RouteRequest::ALPHANUMERICAL:
        return convertAlphanumericalIdToInternalId(request.route->getLastAlphanumericalId());
        break;
    case RouteRequest::NONE:
        /* fallthrough */
    default:
        // pb...
        return OperationResult(); // default isSuccessful false
        break;
    }

    // useless, just to remove eclipse warning
    return OperationResult();
}





void RestResourceHandler::handleCommon(const RestRequest& request, RestResponse& response)
{
    response.setApiVersion(getApiVersion());
    response.setFullContentType(request.getFullContentType());
}

void RestResourceHandler::handlePost(const RestRequest& request, RestResponse& response)
{
    handleCommon(request, response);

    struct OperationResult res = createNewRsrc();
    if(!res.isImplemented)
    {
        response.setStatusCode(HTTP_STATUS_SERVER_ERROR_NOT_IMPLEMENTED);
    }
    else if(!res.isSuccessful)
    {
        // no free slot...
        response.setStatusCode(HTTP_STATUS_INSUFFICIENT_STORAGE);
    }
    else
    {
        const InternalRsrcId rsrcId = res.resultRsrcId;
        std::ostringstream body;

        body << "POST class #" << rsrcId;
        res = dumpRsrcRepresentation(rsrcId, body);
        if(!res.isImplemented)
        {
            response.setStatusCode(HTTP_STATUS_SERVER_ERROR_NOT_IMPLEMENTED);
        }
        else if(!res.isSuccessful)
        {
            // dump failed for whatever reason
            response.setStatusCode(HTTP_STATUS_SERVER_ERROR_INTERNAL_ERROR);
        }
        else
        {
            response.addBodyContent(body);
            response.setStatusCode(HTTP_STATUS_CREATED);
        }
    }
}



struct RestResourceHandler::OperationResult RestResourceHandler::locateRsrcAndHandlePotentialProblems(const RestRequest& request, RestResponse& response)
{
    struct OperationResult res = getRequestedRsrcInternalId(request);
    if(!res.isImplemented)
    {
        response.setStatusCode(HTTP_STATUS_SERVER_ERROR_NOT_IMPLEMENTED);
    }
    else if(!res.isSuccessful)
    {
        // can't understand rsrc id
        response.setStatusCode(HTTP_STATUS_CLIENT_ERROR_BAD_REQUEST);
    }
    else
    {
        const InternalRsrcId rsrcId = res.resultRsrcId;
        res = isRsrcExisting(rsrcId);
        if(!res.isImplemented)
        {
            response.setStatusCode(HTTP_STATUS_SERVER_ERROR_NOT_IMPLEMENTED);
        }
        else if(!res.isSuccessful)
        {
            // doesn't exist
            response.setStatusCode(HTTP_STATUS_CLIENT_ERROR_NOT_FOUND);
        }
        else
        {
            res.resultRsrcId = rsrcId;
        }
    }

    return res;
}

void RestResourceHandler::handleGet(const RestRequest& request, RestResponse& response)
{
    handleCommon(request, response);
    struct OperationResult res = locateRsrcAndHandlePotentialProblems(request, response);
    if(res.isSuccessful)
    {
        const InternalRsrcId rsrcId = res.resultRsrcId;
        std::ostringstream body;

        body << "GET class #" << rsrcId;
        res = dumpRsrcRepresentation(rsrcId, body);
        if(!res.isImplemented)
        {
            response.setStatusCode(HTTP_STATUS_SERVER_ERROR_NOT_IMPLEMENTED);
        }
        else if(!res.isSuccessful)
        {
            // dump failed for whatever reason
            response.setStatusCode(HTTP_STATUS_SERVER_ERROR_INTERNAL_ERROR);
        }
        else
        {
            response.addBodyContent(body);
            response.setStatusCodeOk();
        }
    }
}
void RestResourceHandler::handleDelete(const RestRequest& request, RestResponse& response)
{
    handleCommon(request, response);
    struct OperationResult res = locateRsrcAndHandlePotentialProblems(request, response);
    if(res.isSuccessful)
    {
        const InternalRsrcId rsrcId = res.resultRsrcId;
        res = deleteRsrc(rsrcId);
        if(!res.isImplemented)
        {
            response.setStatusCode(HTTP_STATUS_SERVER_ERROR_NOT_IMPLEMENTED);
        }
        else if(!res.isSuccessful)
        {
            // dump failed for whatever reason
            response.setStatusCode(HTTP_STATUS_SERVER_ERROR_INTERNAL_ERROR);
        }
        else
        {
            std::ostringstream body;

            body << "DELETE class #" << rsrcId;

            response.addBodyContent(body);
            response.setStatusCodeOk();
        }
    }
}
void RestResourceHandler::handlePut(const RestRequest& request, RestResponse& response)
{
    struct OperationResult res = getRequestedRsrcInternalId(request);
    if(!res.isImplemented)
    {
        response.setStatusCode(HTTP_STATUS_SERVER_ERROR_NOT_IMPLEMENTED);
    }
    else if(!res.isSuccessful)
    {
        // can't understand rsrc id
        response.setStatusCode(HTTP_STATUS_CLIENT_ERROR_BAD_REQUEST);
    }
    else
    {
        const InternalRsrcId rsrcId = res.resultRsrcId;
        res = isRsrcExisting(rsrcId);
        if(!res.isImplemented)
        {
            response.setStatusCode(HTTP_STATUS_SERVER_ERROR_NOT_IMPLEMENTED);
        }
        else
        {
            // PUT can create the rsrc if it does not exists.
            bool rsrcAlreadyExists = res.isSuccessful;
            // Do we support it ?
            if(!isCreationWithPutAllowed() and !rsrcAlreadyExists)
            {
                // rsrc doesn't exist and can't create
                response.setStatusCode(HTTP_STATUS_SERVER_ERROR_NOT_IMPLEMENTED);
            }
            else
            {
                if(!rsrcAlreadyExists)
                {
                    // special op needed ?
                }
                res = updateRsrc(rsrcId, NULL);
                if(!res.isImplemented)
                {
                    response.setStatusCode(HTTP_STATUS_SERVER_ERROR_NOT_IMPLEMENTED);
                }
                else if(!res.isSuccessful)
                {
                    // failed for whatever reason
                    response.setStatusCode(HTTP_STATUS_SERVER_ERROR_INTERNAL_ERROR);
                }
                else
                {
                    std::ostringstream body;

                    body << "PUT class #" << rsrcId;
                    res = dumpRsrcRepresentation(rsrcId, body);
                    if(!res.isImplemented)
                    {
                        // internal error because the rsrc was updated
                        // so returning not implemented would be ambiguous
                        response.setStatusCode(HTTP_STATUS_SERVER_ERROR_INTERNAL_ERROR);
                    }
                    else if(!res.isSuccessful)
                    {
                        // dump failed for whatever reason
                        response.setStatusCode(HTTP_STATUS_SERVER_ERROR_INTERNAL_ERROR);
                    }
                    else
                    {
                        response.addBodyContent(body);
                        if(rsrcAlreadyExists)
                            response.setStatusCodeOk();
                        else
                            response.setStatusCode(HTTP_STATUS_CREATED);
                    }
                }
            }
        }
    }
}



void RestServer::addSpecificRequestHandler(enum HttpRequestMethod m, const std::string& route, RestSpecificHandler handler)
{
    // TODO
}
void RestServer::addRsrcHandler(RestResourceHandlerPtr handler, const std::string& specificRoute)
{
    // TODO
}
