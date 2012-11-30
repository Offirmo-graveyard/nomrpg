
#include "debug_infos_page.hpp"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <Wt/WApplication>
#include <Wt/WEnvironment>

#include <Wt/WLogger>

#include <Wt/WBreak>
#include <Wt/WText>
#include <Wt/WAnchor>




void DebugInfosPage::addKeyValueLine(const std::string &key, const std::string &value)
{
	addLine(key + " = " + value);
}
void DebugInfosPage::addKeyValueLine(const std::string &key, bool value)
{
	addLine(key + " = " + (value?"true":"false") );
}
void DebugInfosPage::addKeyValueLine(const std::string &key, double value)
{
	addLine(key + " = " + boost::lexical_cast<std::string>(value) );
}

void DebugInfosPage::addLine(const std::string &text)
{
	this->addWidget(new Wt::WText(text));
	this->addWidget(new Wt::WBreak());   // REM : insert a line break
}

DebugInfosPage::DebugInfosPage(Wt::WApplication& parentApp)
{
	this->setId("debug-informations-page");

	// link back to main page
	Wt::WAnchor *a1 = new Wt::WAnchor( this );
	a1->setRefInternalPath( "/landing" );
	new Wt::WText( "back to landing", a1 );
	this->addWidget(new Wt::WBreak());

	// Now we'll print every debug info listed here :
	// http://www.webtoolkit.eu/wt/doc/reference/html/classWt_1_1WEnvironment.html
	const Wt::WEnvironment &env = parentApp.environment();

	// example line
	addKeyValueLine("[key]", "[value]");

	// Parameters passed to the application.
	const Wt::Http::ParameterMap &param_map = env.getParameterMap();
	BOOST_FOREACH( const Wt::Http::ParameterMap::value_type &param, param_map )
	{
		BOOST_FOREACH( const std::string &param_value, param.second )
		{
			addKeyValueLine("param[" + param.first + "]", param_value);
		}
	}

	// cookies
	Wt::WEnvironment::CookieMap cookie_map = env.cookies();
	BOOST_FOREACH( const Wt::WEnvironment::CookieMap::value_type &cookie, cookie_map )
	{
		//parentApp->log("notice") << "|~| cookie : " << cookie.first << " = " << cookie.second << "\n";
		addKeyValueLine("cookie[" + cookie.first + "]", cookie.second);
	}

	/*
	const std::string 	headerValue (const std::string &field) const
	 	Returns a header value.
	 */

	addKeyValueLine("browser has enabled support for cookies", env.supportsCookies());
	addKeyValueLine("browser has enabled support for javascript", env.javaScript());
	addKeyValueLine("browser has enabled support for ajax", env.ajax());

	addKeyValueLine("browser-side DPI scaling factor", env.dpiScale());

	addKeyValueLine("preferred language indicated in the request header", env.locale());

	addKeyValueLine("server host name that is used by the client", env.hostName());
	addKeyValueLine("URL scheme used for the current request", env.urlScheme());
	addKeyValueLine("user agent", env.userAgent());
	addKeyValueLine("referer", env.referer());
	addKeyValueLine("the accept header", env.accept());
	addKeyValueLine("user agent is a (known) indexing spider bot", env.agentIsSpiderBot());
	addKeyValueLine("web server signature", env.serverSignature());
	addKeyValueLine("web server software", env.serverSoftware());
	addKeyValueLine("email address of the server admin", env.serverAdmin());
	addKeyValueLine("IP address of the client", env.clientAddress());
	addKeyValueLine("path info of the original request (deprecated)", env.pathInfo());
	addKeyValueLine("initial internal path", env.internalPath());
	addKeyValueLine("deployment path", env.deploymentPath());

	int series, major, minor;
	series = major = minor = -1;
	env.libraryVersion (series, major, minor);
	addKeyValueLine("version of the Wt library", boost::lexical_cast<std::string>(series) + "." + boost::lexical_cast<std::string>(major) + "." + boost::lexical_cast<std::string>(minor) );
	addKeyValueLine("version of the Wt library",  env.libraryVersion() );

	addKeyValueLine("Wt session id (deprecated)", env.sessionId());

	/* std::string 	getCgiValue (const std::string &varName) const
 	Returns a raw CGI environment variable.
	 */

	//ContentType
	addKeyValueLine("type of the content provided to the browser", static_cast<double>(env.contentType())); // contenttype is an enum
	//UserAgent
	addKeyValueLine("user agent type", static_cast<double>(env.agent())); // from enum

	addKeyValueLine("user agent is Microsoft Internet Explorer", env.agentIsIE());
	int ie_lower_than = 7;
	addKeyValueLine("user agent is an older version of IE than " + boost::lexical_cast<std::string>(ie_lower_than), env.agentIsIElt(ie_lower_than));
	addKeyValueLine("user agent is Internet Explorer Mobile", env.agentIsIEMobile());
	addKeyValueLine("user agent is Opera", env.agentIsOpera());
	addKeyValueLine("user agent is WebKit-based", env.agentIsWebKit());
	addKeyValueLine("user agent is Mobile WebKit-based", env.agentIsMobileWebKit());
	addKeyValueLine("user agent is Safari", env.agentIsSafari());
	addKeyValueLine("user agent is Chrome", env.agentIsChrome());
	addKeyValueLine("user agent is Gecko-based", env.agentIsGecko());
	//addKeyValueLine("", env.());

	// link back to main page
	// again, because this is a big page.
	Wt::WAnchor *a1b = new Wt::WAnchor( this );
	a1b->setRefInternalPath( "/landing" );
	new Wt::WText( "back to landing", a1b );
	this->addWidget(new Wt::WBreak());
}
