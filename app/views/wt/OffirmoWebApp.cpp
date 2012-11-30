#include "OffirmoWebApp.hpp"

#include <boost/foreach.hpp>

#include <Wt/WLogger>

#include <Wt/Auth/PasswordService>

#include "Mok/Exception.hpp"

#include "AppSessionInitializer.hpp"

#include "landing_page.hpp"
#include "debug_infos_page.hpp"
#include "app_page.hpp"
#include "about_page.hpp"


enum sub_pages
{
	OWA_PAGE_LANDING,
	OWA_PAGE_DEBUG,
	OWA_PAGE_APP,
	OWA_PAGE_MISC
};

const std::string OffirmoWebApp::m_default_internal_path = "/";


OffirmoWebApp::OffirmoWebApp(const Wt::WEnvironment& p_env, boost::shared_ptr<WtDboData> auth_dbo_data, boost::shared_ptr<WtDboData> main_dbo_data):
		Wt::WApplication(p_env),
		m_lang_wgt(*this),
		m_wt_datas(auth_dbo_data, main_dbo_data)
{
	/////// Intro ////////
	// a log to celebrate the launch of our application !
	this->log("notice") << "\n"
		<< "|~| Starting an Offirmo Web App instance...\n"
		<< "|~| by browsing to : " << url() << "\n"
		<< "|~| Resources : '" << resourcesUrl() << "' (" << bookmarkUrl() << ")" << "\n"
		<< "|~| App root : '" << appRoot() << "'\n"
		;

	initialize_model();


	/////// Configure Wt ///////

	// enable server-initiated updates from outside this session (another thread for example)
	// note : this is tricky and should be done carefully.
	enableUpdates();

	//// Content
	/// prepare localization and templates
	// Note : experiments have shown that files are searched in order of declaration,
	//        so override files must be declared first
	// http://www.webtoolkit.eu/wt/doc/reference/html/classWt_1_1WMessageResourceBundle.html
	// TBRPG
	m_msg_rsrc_bundles.push_back("TBRPGStrings");
	m_msg_rsrc_bundles.push_back("TBRPGTemplates");
	// App
	m_msg_rsrc_bundles.push_back("AppStrings");
	m_msg_rsrc_bundles.push_back("AppTemplates");
	// auth (from Wt)
	m_msg_rsrc_bundles.push_back("AuthOverride");
	m_msg_rsrc_bundles.push_back("AuthOriginal"); // redeclare it to be able to translate it
	load_msg_rsrc_bundles();
	// enable UTF-8 (hopefully)
	//addMetaHeader("charset", "utf-8", "lang");

	// The authentification widget
	// we create it here because it add a stylesheet we want to override
	// and stylesheets are taken in reverse order of addition
	m_pAuthWidget = getAuthWidget();

	///// stylesheets
	initialize_css();

	/// javascript
	//require("javascripts/jquery.js");
	//require("javascripts/jquery-ui.js");

	/// actual content
	setTitle(Wt::WString::tr("App.title"));   // application title
	// to comply with blueprint css, we give the class 'container' to the root div
	//root()->addStyleClass("container");

	/// the app

	m_lang_wgt.init();
	root()->addWidget(&m_lang_wgt);

	// create the stack
	m_app_stack.addStyleClass("owa-main-stack", true);
	// and add widgets to the stack
	m_app_stack.insertWidget(OWA_PAGE_LANDING, new LandingPage(*this));
	m_app_stack.insertWidget(OWA_PAGE_DEBUG,   new DebugInfosPage(*this));
	m_app_stack.insertWidget(OWA_PAGE_APP,     new AppPage(*this));
	m_app_stack.insertWidget(OWA_PAGE_MISC,    new DebugInfosPage(*this));
	m_app_stack.setCurrentIndex(0);
	
	root()->addWidget(&m_app_stack);

	// now that our app is ready, we can connect some event handlers :
	/// Authentification
	// ask to be notified of authentication events
	m_wt_datas.m_auth_session.login().changed().connect(this, &OffirmoWebApp::handle_auth_event);
	// ask to be notified of path changes
	internalPathChanged().connect(this, &OffirmoWebApp::handle_internal_path_event);
	// what is the current path ? Maybe the user accessed our app with a special path ?
	// For the first time, we manually call the handler with the current path.
	handle_internal_path_event(internalPath());

	// most likely needed
	handle_auth_event();
}


void OffirmoWebApp::initialize_css()
{
	// common base stylesheets / CSS reset
	// Note : really useful for having good looking fonts, etc.
	// Wt doesn't provide a clean sheet by default.
	useStyleSheet("stylesheets/blueprint/screen.css",          "",        "screen, projection");
	useStyleSheet("stylesheets/blueprint/ie.css",              "IE lt 8", "screen, projection");

	/// Wt theme
	// NOTE : this one don't respect ordering, it always come first !
	//setCssTheme(""); // default theme
	//setCssTheme("polished"); // another built-in theme
	setCssTheme("polished_dark"); // a custom theme based on the polished theme
	// due to ordering problems, we include again the main sheet
	useStyleSheet("resources/themes/polished_dark/wt.css");
	//useStyleSheet("resources/moz-transitions.css");
	//useStyleSheet("resources/form.css");

	/// default Wt css
	//useStyleSheet("css/style.css"); XXX where is it ?

	// jquery
	useStyleSheet("stylesheets/jquery-ui-themes/dark-hive/jquery-ui.css");

	// main / override
	//useStyleSheet("themes/debug/style/style.css",              "",        "screen");
	useStyleSheet("themes/stargate/style/main.css", "",        "screen");
	//useStyleSheet("themes/stargate/style/special-landing.css", "",        "screen");
}

std::vector<std::string> m_msg_rsrc_bundles;

void OffirmoWebApp::load_msg_rsrc_bundles()
{
	BOOST_FOREACH(std::string bundle_name, m_msg_rsrc_bundles)
	{
		load_msg_rsrc_bundle(bundle_name);
	}
}
void OffirmoWebApp::load_msg_rsrc_bundle(std::string bundle_id)
{
	static const std::string bundle_path = "../wt-xp/config/";
	messageResourceBundle().use(appRoot() + bundle_path + bundle_id);

	check_msg_rsrc_bundle(bundle_id);

}
void OffirmoWebApp::check_msg_rsrc_bundle(std::string bundle_id)
{
	check_specific_msg_rsrc_bundle(bundle_id);
	if(locale() != "en")
		check_specific_msg_rsrc_bundle(bundle_id + "_" + locale() );
}
void OffirmoWebApp::check_specific_msg_rsrc_bundle(std::string bundle_id)
{
	check_specific_msg_rsrc_bundle_marker(bundle_id + ".xml.begin");
	check_specific_msg_rsrc_bundle_marker(bundle_id + ".xml.end");
}
void OffirmoWebApp::check_specific_msg_rsrc_bundle_marker(std::string marker)
{
	std::string result;
	bool ok = messageResourceBundle().resolveKey(marker, result);
	//if(!ok)
	//	throw Mok::MissingRequiredDataException(" rsrc bundle marker " + marker);
	//if(result != "ok")
	//	throw Mok::IncoherentDataException(" rsrc bundle marker " + marker);
}


Wt::Auth::AuthWidget* OffirmoWebApp::getAuthWidget()
{
	Wt::Auth::AuthWidget* pAW = new Wt::Auth::AuthWidget(Session::auth(), m_wt_datas.m_auth_session.users(), m_wt_datas.m_auth_session.login());
	pAW->model()->addPasswordAuth(&Session::passwordAuth());
	pAW->model()->addOAuth(Session::oAuth());
	pAW->setRegistrationEnabled(true);
	pAW->processEnvironment();

	return pAW;
}


void OffirmoWebApp::initialize_model()
{
	// Initiate our authentication state machine.
	m_auth_state_machine.initiate();
	// Initiate the web app state machine
	m_web_app_state_machine.initiate();
	// Initialize the app session
	m_session.reset(new AppSession());
	m_session->register_main_dbo_session(m_wt_datas.m_main_dbo->cnx, m_wt_datas.m_main_dbo->session); // take reference on 'cnx' and 'session' shared ptrs
	AppSessionSetter as(m_session);
	as.set_up_to_vw(); // we can't go farther for now
}


#define CHECK_STATE(state_machine, state) (void)state_machine.state_cast< const state & >()
void OffirmoWebApp::refresh()
{
	/* check the state of our state machine
	 * and display page accordingly.
	 */
	this->log("info") << "|~| refresh : starting...";
	if (m_web_app_state_machine.state_cast< const Landing * >())
	{
		this->log("info") << "|~| refresh : switching to LANDING.";
		m_app_stack.setCurrentIndex(OWA_PAGE_LANDING);
	}
	else if (m_web_app_state_machine.state_cast< const Debug * >())
	{
		this->log("info") << "|~| refresh : switching to DEBUG.";
		m_app_stack.setCurrentIndex(OWA_PAGE_DEBUG);
	}
	else if (m_web_app_state_machine.state_cast< const App * >())
	{
		this->log("info") << "|~| refresh : switching to APP.";
		m_app_stack.setCurrentIndex(OWA_PAGE_APP);
	}
	else if (m_web_app_state_machine.state_cast< const Misc * >())
	{
		this->log("info") << "|~| refresh : switching to MISC.";
		m_app_stack.setCurrentIndex(OWA_PAGE_MISC);
	}
	else
	{
		// not possible !
		this->log("error") << "|~| refresh : unknown OWA state !!! switching to LANDING.";
		m_app_stack.setCurrentIndex(OWA_PAGE_LANDING);
	}

	// will it work ?
	/*doJavaScript(""
"$(document).ready(function()"
"{"
"	$(\".resizable\").resizable();"
"	$(\".draggable\").draggable();"
//"	alert('exec');"
"});");*/

	this->log("info") << "|~| refresh : done.";

}

void OffirmoWebApp::handle_internal_path_event(const std::string &p_internalPath)
{
	this->log("notice") << "|~| handle_internal_path_event : '" << p_internalPath << "'";// << "\n";

	// TODO : redirection to inside app

	if (p_internalPath == m_default_internal_path)
	{
		m_web_app_state_machine.process_event( EvReqHome() );
	}
	else if (p_internalPath == "/debug")
	{
		m_web_app_state_machine.process_event( EvReqDebug() );
	}
	else
	{
		this->log("error") << "|~| handle_internal_path_event : unknown path : '" << p_internalPath << "', redirecting to default.\n";
		WApplication::instance()->setInternalPath(m_default_internal_path, true);
		m_web_app_state_machine.process_event( EvReqHome() );
	}
	// most likely needed
	refresh();
}

void OffirmoWebApp::handle_auth_event()
{
	this->log("notice") << "|~| handle_auth_event : "; // << "\n";

	if (m_wt_datas.m_auth_session.login().loggedIn())
	{
		Wt::log("notice") << "User (" << m_wt_datas.m_auth_session.login().user().id() << ") " << m_wt_datas.m_auth_session.login().user().email() << " logged in.";
		m_auth_state_machine.process_event( EvIdentify() );
		m_auth_state_machine.process_event( EvAuthentify() );
		// we can now find the current user and fill our app_session
		initialize_app_session_following_login();
		m_web_app_state_machine.process_event( EvReqApp() );
	}
	else
	{
		Wt::log("notice") << "User logged out.";
		m_auth_state_machine.process_event( EvUnauthentify() );
		m_web_app_state_machine.process_event( EvReqHome() );
	}
	// most likely needed
	refresh();
}


void OffirmoWebApp::initialize_app_session_following_login()
{
	m_session->register_current_auto_user_id(m_wt_datas.m_auth_session.login().user().id());
	AppSessionSetter as(m_session);
	as.set_up(); // full setup now that we have a user
}

