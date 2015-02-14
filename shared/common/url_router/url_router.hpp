#ifndef URL_ROUTER_HPP__
#define URL_ROUTER_HPP__
/*
#include "url_router.hpp"
 */

#include <iostream> // debug traces
#include <string>
#include <map>
#include <vector>
#include <utility> // for pair

#include "misc/smart_pointers.hpp"
#include "misc/extended_exceptions.hpp"





/* Custom data may be attached to routes.
 * They must inherit from this class.
 */
class BaseRouteData
{
public:
    virtual ~BaseRouteData() {}
};
// We must use smart pointers for polymorphism
typedef spns::shared_ptr<BaseRouteData> BaseRouteDataPtr;



/* Routes will obviously be stored as a tree of "segments".
 * This is a leaf.
 * Those objects are meant to be handled by the Router only.
 */
class RouteSegment;
typedef spns::shared_ptr<RouteSegment> RouteSegmentPtr;
typedef spns::shared_ptr<const RouteSegment> ConstRouteSegmentPtr;
class RouteSegment
{
public:
    enum Type
    {
        UNKNOWN,           //< unknown leaf type
        NUMERICAL_ID,
        ALPHANUMERICAL_ID,
        FIXED              //< fixed string leaf (ex. /foo)
    };
protected:

    RouteSegment(enum RouteSegment::Type type = UNKNOWN):
        type_(type)
    {
        std::cout << "(Route segment created [" << type << "])" << std::endl;
    }

    RouteSegment(std::string segment):
        type_(FIXED),
        fixedSegmentString_(segment)
    {
        std::cout << "(Fixed route segment created : \"" << segment << "\")" << std::endl;
    }

    RouteSegmentPtr getStringLeafFor(const std::string& subSegment);

    void setNumericalLeaf(RouteSegmentPtr seg)
    {
        numericalLeaf_ = seg;
        std::cout << "(Numerical route segment added as child of \"" << this->fixedSegmentString_ << "\"" << std::endl;
    }
    void setAlphanumericalLeaf(RouteSegmentPtr seg)
    {
        alphanumericalLeaf_ = seg;
        std::cout << "(Alphanumerical route segment added as child of \"" << this->fixedSegmentString_ << "\"" << std::endl;
    }
    void addStringLeaf(RouteSegmentPtr seg)
    {
        //assert(seg->type == FIXED)
        stringLeaves_[seg->fixedSegmentString_] = seg;
        std::cout << "(Fixed route segment \"" << seg->fixedSegmentString_ << "\" added as child of \"" << this->fixedSegmentString_ << "\"" << std::endl;
    }

    Type type_;
    std::string fixedSegmentString_;
    std::string canonicalRoute_;

    RouteSegmentPtr numericalLeaf_;
    RouteSegmentPtr alphanumericalLeaf_;
    typedef std::map<std::string, RouteSegmentPtr> StringLeavesContainer;
    StringLeavesContainer stringLeaves_;

    spns::shared_ptr<BaseRouteData> routeData_;

    friend class BaseUrlRouter;
    friend class RouteRequest;
};



class RouteRequest
{
public:
    enum IdType
    {
        NONE,
        NUMERICAL,
        ALPHANUMERICAL,
    };
    typedef std::pair<RouteSegment::Type, std::string> SegmentBasicInfo;
    typedef std::vector<SegmentBasicInfo> SegmentsBasicInfos;

    RouteRequest(const std::string& route):requestedRoute_(route),lastIdType_(NONE),lastNumericalId_(-1) {}
    virtual ~RouteRequest() {}

    std::string getCurrentRoute() const;
    std::string getCanonicalRoute() const;

    RouteSegment::Type getSegmentType(const std::string& segment) const;

    enum IdType getLastIdType() const { return lastIdType_; }

    // for typedness, we must duplicate all ID methods
    int getNumericalIdForSegment(const std::string& segment) const;
    int getLastNumericalId() const { return lastNumericalId_; }
    std::string getAlphanumericalIdForSegment(const std::string& segment) const;
    std::string getLastAlphanumericalId() const { return lastAlphanumericalId_; }

    BaseRouteDataPtr getRouteData() const { return matchingSegment_->routeData_; }

    int getSegmentCount() const { return segments_.size(); }
    RouteSegment::Type getNthSegmentType(unsigned int i) const { return (i < segments_.size() ? segments_[i].first : RouteSegment::UNKNOWN); }
    // will return previous segment name if id
    std::string getNthSegmentName(unsigned int i) const { return (i < segments_.size() ? segments_[i].second : ""); }

protected:

    void addNumericalIdSegment(int id, const std::string& correspondingSegment)
    {
        numericalIds_[correspondingSegment] = id;
        lastNumericalId_ = id;
        lastIdType_ = NUMERICAL;
        segments_.push_back(std::make_pair(RouteSegment::NUMERICAL_ID, correspondingSegment));
    }

    void addAlphanumericalIdSegment(const std::string& id, const std::string& correspondingSegment)
    {
        alphanumericalIds_[correspondingSegment] = id;
        lastAlphanumericalId_ = id;
        lastIdType_ = ALPHANUMERICAL;
        segments_.push_back(std::make_pair(RouteSegment::ALPHANUMERICAL_ID, correspondingSegment));
    }

    void addFixedSegment(const std::string& segment)
    {
        segments_.push_back(std::make_pair(RouteSegment::FIXED, segment));
    }

    std::string requestedRoute_;
    SegmentsBasicInfos segments_;
    enum IdType lastIdType_;
    int lastNumericalId_;
    std::string lastAlphanumericalId_;
    std::map<std::string, int>         numericalIds_;
    std::map<std::string, std::string> alphanumericalIds_;

    RouteSegmentPtr matchingSegment_;

    friend class BaseUrlRouter;
};
typedef spns::shared_ptr<RouteRequest> RouteRequestPtr;
typedef spns::shared_ptr<const RouteRequest> ConstRouteRequestPtr;



// custom exceptions
// Grr : C++ constructors are not inherited
////////////
class RoutingException: public utils::Exception
{
public: RoutingException(const std::string& what_str = "") throw():utils::Exception(what_str) {}
};
class UnknownRoutingException: public RoutingException
{
public: UnknownRoutingException(const std::string& what_str = "") throw():RoutingException(what_str) {}
};
class RouteTooLongException:   public RoutingException
{
public: RouteTooLongException(const std::string& what_str = "") throw():RoutingException(what_str) {}
};
class MalformedRouteException: public RoutingException
{
public: MalformedRouteException(const std::string& what_str = "") throw():RoutingException(what_str) {}
};
class NoMatchingRouteException: public RoutingException
{
public: NoMatchingRouteException(const std::string& what_str = "") throw():RoutingException(what_str) {}
};




/* The router
 * Create a "routing tree"
 * And attach any kind of custom data to the leaves.
 * This is the base version, an simplified helper version is available : UrlRooter
 * @see UrlRooter
 */
class BaseUrlRouter
{
public:
    ////////////
    static const std::string::size_type maxRouteLength = 200;
    static const char* NumericalIdCode;
    static const char* AlphanumericalIdCode;


    ////////////
    BaseUrlRouter();
    virtual ~BaseUrlRouter() {}


    ////////////
    // REM use of pointers is mandatory for polymorphism
    void createRouteAndAttachInfos(const std::string& route, BaseRouteDataPtr rd);
    BaseRouteDataPtr findOrCreateRouteAndGetInfos(const std::string& route);

    RouteRequestPtr getMatchingRouteInfos(const std::string& route);

protected:
    static const std::streamsize maxRouteSegmentDecodingBufferSize = 30;
    static const char* zeroString;

    RouteRequestPtr findAndOptionallyCreateRoute(const std::string& route, bool codesAllowed = false, bool insertionAllowed = false);

    RouteSegmentPtr rootSegment_; // root of the segment tree
};




template<typename RouteDataInnerType>
class CustomRouteData: public BaseRouteData
{
public:
    CustomRouteData(RouteDataInnerType d):
        details_(d)
    {}

    RouteDataInnerType details_;
};


template<typename RouteDataInnerType>
class UrlRouter: public BaseUrlRouter
{
public:

    void createRouteAndAttachInfos(const std::string& route, RouteDataInnerType d)
    {
        BaseRouteDataPtr rd(new CustomRouteData<RouteDataInnerType>(d));
        BaseUrlRouter::createRouteAndAttachInfos(route, rd);
    }

    RouteDataInnerType getMatchingRouteInfos(const std::string& route)
    {
        RouteRequestPtr routeRequest = BaseUrlRouter::getMatchingRouteInfos(route);
        if(!routeRequest || !routeRequest->getRouteData())
        {
            // ???
            throw UnknownRoutingException("in getRoute");
        }
        else
        {
            spns::shared_ptr<CustomRouteData<RouteDataInnerType> > rrd = spns::dynamic_pointer_cast<CustomRouteData<RouteDataInnerType> >(routeRequest->getRouteData());
            // check NULL ? Impossible... TODO
            return rrd->details_;
        }
    }

    RouteRequestPtr getRouteDetailledInfos(const std::string& route)
    {
        return BaseUrlRouter::getMatchingRouteInfos(route);
    }

    static RouteDataInnerType getRouteDataFor(RouteRequestPtr routeRequest)
    {
        if(!routeRequest || !routeRequest->getRouteData())
        {
            // can't work...
            throw UnknownRoutingException("in getRouteDataFor");
        }
        else
        {
            spns::shared_ptr<CustomRouteData<RouteDataInnerType> > rrd = spns::dynamic_pointer_cast<CustomRouteData<RouteDataInnerType> >(routeRequest->getRouteData());
            // check NULL ? Impossible... TODO
            return rrd->details_;
        }
    }
};



#endif // URL_ROUTER_HPP__
