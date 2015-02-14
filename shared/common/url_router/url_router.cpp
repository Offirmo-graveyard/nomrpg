#include "url_router.hpp"

#include <iostream>

#include <sstream>
#include <cstring>

#include <cstdlib>

#include <cassert>





RouteSegmentPtr RouteSegment::getStringLeafFor(const std::string& subSegment)
{
    RouteSegmentPtr res; // REM : points to nothing

    StringLeavesContainer::const_iterator it = stringLeaves_.find(subSegment);
    if( it == stringLeaves_.end() )
    {
        // not found, res stays NULL
    }
    else
    {
        res = it->second;
    }

    return res;
}



std::string RouteRequest::getCanonicalRoute() const
{
    if(!matchingSegment_)
    {
        return "XXX no canonical route XXX";
    }
    else
    {
        return matchingSegment_->canonicalRoute_;
    }
}


int RouteRequest::getNumericalIdForSegment(const std::string& segment) const
{
    std::map<std::string, int>::const_iterator it = numericalIds_.find(segment);
    if( it == numericalIds_.end() )
    {
        // not found
        return -1; // hope that'll be seen
    }
    else
    {
        return it->second;
    }
}
std::string RouteRequest::getAlphanumericalIdForSegment(const std::string& segment) const
{
    std::map<std::string, std::string>::const_iterator it = alphanumericalIds_.find(segment);
    if( it == alphanumericalIds_.end() )
    {
        return "xxx not found xxx";
    }
    else
    {
        return it->second;
    }
}



const char* BaseUrlRouter::NumericalIdCode      = "<id>";
const char* BaseUrlRouter::AlphanumericalIdCode = "<code>";
const char* BaseUrlRouter::zeroString = "0";


////////////
BaseUrlRouter::BaseUrlRouter():
        rootSegment_(new RouteSegment("/"))
{
    rootSegment_->canonicalRoute_ = "/";
}


void BaseUrlRouter::createRouteAndAttachInfos(const std::string& route, BaseRouteDataPtr d)
{
    RouteRequestPtr routeRequest = findAndOptionallyCreateRoute(route, /* codesAllowed */ true, /* insertionAllowed */ true);
    if(!routeRequest || !routeRequest->matchingSegment_)
    {
        // ???
        // should never arrive here, previous call should create or throw
        throw UnknownRoutingException("in createRoute");
    }
    else
    {
        // TODO check overwrite ?
        routeRequest->matchingSegment_->routeData_ = d;
    }
}

BaseRouteDataPtr BaseUrlRouter::findOrCreateRouteAndGetInfos(const std::string& route)
{
    RouteRequestPtr routeRequest = findAndOptionallyCreateRoute(route, /* codesAllowed */ true, /* insertionAllowed */ true);
    if(!routeRequest || !routeRequest->matchingSegment_)
    {
        // ???
        // should never arrive here, previous call should create or throw
        throw UnknownRoutingException("in findOrCreateRouteInfos");
    }
    else
    {
        return routeRequest->matchingSegment_->routeData_;
    }
}

RouteRequestPtr BaseUrlRouter::getMatchingRouteInfos(const std::string& route)
{
    RouteRequestPtr routeRequest = findAndOptionallyCreateRoute(route, /* codesAllowed */ false, /* insertionAllowed */ false);

    if(!routeRequest || !routeRequest->matchingSegment_)
    {
        // 404
        throw NoMatchingRouteException("");
    }
    else
    {
        return routeRequest;
    }
}

//doBaseChecks(const std::string& route, std::istringstream&

RouteRequestPtr BaseUrlRouter::findAndOptionallyCreateRoute(const std::string& route, bool codesAllowed, bool insertionAllowed)
{
    if (route.length() > maxRouteLength)
    {
        throw RouteTooLongException();
    }

    RouteSegmentPtr bestMatchingSegmentSoFar; // REM : points to nothing for now
    RouteRequestPtr routeRequest(new RouteRequest(route)); // result

    std::cout << "* Processing route : \"" << route << "\"..." << std::endl;

    // We'll use the getline technique for splitting
    // http://en.cppreference.com/w/cpp/string/basic_string/getline

    // prepare
    std::istringstream routeDecoder(route);
    char segmentBuffer[maxRouteSegmentDecodingBufferSize+1];

    // check if begins with /
    routeDecoder.getline(segmentBuffer, maxRouteSegmentDecodingBufferSize, '/');
    if (segmentBuffer[0] != '\0' or routeDecoder.eof())
    {
        throw MalformedRouteException("route must start with a / !");
    }
    bestMatchingSegmentSoFar = rootSegment_;

    // now process remaining segments
    do
    {
        routeDecoder.getline(segmentBuffer, maxRouteSegmentDecodingBufferSize+1, '/');

        // debug
        std::cout << "  * current segment : \"" << segmentBuffer << "\"" << std::endl;
        std::cout << "  * current state : good ? " << routeDecoder.good() << ", eof ? " << routeDecoder.eof() << ", fail ? " << routeDecoder.fail() << std::endl;

        if (segmentBuffer[0] == '\0')
        {
            if(routeDecoder.eof())
            {
                // nothing more to parse
                break;
            }
            else
            {
                // empty string but not eof
                // means several '/' concatened,
                // -> this is not allowed
                throw MalformedRouteException("route may not contain consecutive '/' !");
            }
        }

        if(routeDecoder.fail())
        {
            // there was a problem
            // according to the doc, can happen for 2 reasons :
            // - no character was extracted
            //   -> cannot happen since we already tested eof
            // - the buffer was filled
            throw MalformedRouteException("segment too long !");
        }

        // first detect special segments
        if(codesAllowed)
        {
            if(0 == strncmp(NumericalIdCode, segmentBuffer, maxRouteSegmentDecodingBufferSize))
            {
                // this segment is the special code for a numeric id
                if(!bestMatchingSegmentSoFar->numericalLeaf_)
                {
                    if(insertionAllowed)
                    {
                        // an id refer to a rsrc
                        // so we must ensure that previous segment is a non-empty fixed segment
                        if(bestMatchingSegmentSoFar->type_ != RouteSegment::FIXED)
                        {
                            throw MalformedRouteException("an id must follow a fixed segment");
                        }
                        else
                        {
                            bestMatchingSegmentSoFar->setNumericalLeaf(RouteSegmentPtr(new RouteSegment(RouteSegment::NUMERICAL_ID)));
                            bestMatchingSegmentSoFar->numericalLeaf_->canonicalRoute_ = bestMatchingSegmentSoFar->canonicalRoute_ + "/" + NumericalIdCode;
                        }
                    }
                    else
                    {
                        // no match
                        bestMatchingSegmentSoFar.reset();
                        break;
                    }
                }
                else
                {
                    // ...
                }
                bestMatchingSegmentSoFar = bestMatchingSegmentSoFar->numericalLeaf_;
                continue;
            }
            else if(0 == strncmp(AlphanumericalIdCode, segmentBuffer, maxRouteSegmentDecodingBufferSize))
            {
                // this segment is the special code for an alphanumeric id
                if(!bestMatchingSegmentSoFar->alphanumericalLeaf_)
                {
                    if(insertionAllowed)
                    {
                        // an id refer to a rsrc
                        // so we must ensure that previous segment is a non-empty fixed segment
                        if(bestMatchingSegmentSoFar->type_ != RouteSegment::FIXED)
                        {
                            throw MalformedRouteException("an id must follow a fixed segment");
                        }
                        else
                        {
                            bestMatchingSegmentSoFar->setAlphanumericalLeaf(RouteSegmentPtr(new RouteSegment(RouteSegment::ALPHANUMERICAL_ID)));
                            bestMatchingSegmentSoFar->alphanumericalLeaf_->canonicalRoute_ = bestMatchingSegmentSoFar->canonicalRoute_ + "/" + AlphanumericalIdCode;
                        }
                    }
                    else
                    {
                        // no match
                        bestMatchingSegmentSoFar.reset();
                        break;
                    }
                }
                else
                {
                    // ...
                }
                bestMatchingSegmentSoFar = bestMatchingSegmentSoFar->alphanumericalLeaf_;
                continue;
            }
        }

        if(bestMatchingSegmentSoFar->numericalLeaf_)
        {
            // parent segment allow numeric sub-segment
            // is current segment an integer ?
            char *decodingEnd = NULL;
            unsigned long ulRepresentation = strtoul( segmentBuffer, &decodingEnd, 10 );
            // Warning : strtoul API is subtile,
            // cf. http://en.cppreference.com/w/cpp/string/byte/strtoul
            switch(ulRepresentation)
            {
            case 0:
                // error or real O ? (The API is ambiguous for this case)
                if(0 == strncmp(zeroString, segmentBuffer, maxRouteSegmentDecodingBufferSize))
                {
                    // OK, really string "0"
                    // store route info
                    routeRequest->addNumericalIdSegment(ulRepresentation, bestMatchingSegmentSoFar->fixedSegmentString_);
                    bestMatchingSegmentSoFar = bestMatchingSegmentSoFar->numericalLeaf_;
                    continue;
                }
                else
                {
                    // segment is not an integer
                    // fallback to fixed string (below)
                }
                break;
            case ULONG_MAX:
                throw MalformedRouteException("numeric segment is out of range !");
                break;
            default:
                // did the decoding used the whole segment
                // or did it used only a part ?
                // ex. "123foo" decode as 123 but this is not correct
                if (decodingEnd && *decodingEnd == '\0')
                {
                    // current segment is a valid integer, OK
                    routeRequest->addNumericalIdSegment(ulRepresentation, bestMatchingSegmentSoFar->fixedSegmentString_);
                    bestMatchingSegmentSoFar = bestMatchingSegmentSoFar->numericalLeaf_;
                    continue;
                }
                else
                {
                    // segment begins with a number but is not a number
                    // Should we report a malformed string instead ? -> let's be open for now.
                    // Meanwhile, fallback to fixed string (below)
                }
                break;
            }
        }

        if(bestMatchingSegmentSoFar->alphanumericalLeaf_)
        {
            // yes. Anything match.
            routeRequest->addAlphanumericalIdSegment(segmentBuffer, bestMatchingSegmentSoFar->fixedSegmentString_);
            bestMatchingSegmentSoFar = bestMatchingSegmentSoFar->alphanumericalLeaf_;
            continue;
        }

        // is current segment already registered ?
        RouteSegmentPtr candidateMatchingSegment = bestMatchingSegmentSoFar->getStringLeafFor(segmentBuffer);
        if(!candidateMatchingSegment)
        {
            // no route matching this
            if(insertionAllowed)
            {
                candidateMatchingSegment.reset(new RouteSegment(segmentBuffer));
                bestMatchingSegmentSoFar->addStringLeaf(candidateMatchingSegment);
                candidateMatchingSegment->canonicalRoute_ = bestMatchingSegmentSoFar->canonicalRoute_ + "/" + segmentBuffer;
                bestMatchingSegmentSoFar = candidateMatchingSegment;
                routeRequest->addFixedSegment(bestMatchingSegmentSoFar->fixedSegmentString_);
                continue;
            }
            else
            {
                // no match
                bestMatchingSegmentSoFar.reset();
                break;
            }
        }
        else
        {
            // fine
            bestMatchingSegmentSoFar = candidateMatchingSegment;
            routeRequest->addFixedSegment(bestMatchingSegmentSoFar->fixedSegmentString_);
            continue;
        }
    } while(bestMatchingSegmentSoFar and not routeDecoder.eof());

    routeRequest->matchingSegment_ = bestMatchingSegmentSoFar;
    return routeRequest;
}
