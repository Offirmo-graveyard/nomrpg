/* Offirmo base web application
 * using the Wt framework http://www.webtoolkit.eu/wt
 *
#include "OffirmoWebApp.hpp"
 */

#ifndef __OFFIRMOWEBAPP_HPP_INCLUDED__
#define __OFFIRMOWEBAPP_HPP_INCLUDED__


#include <Wt/WApplication>
#include <Wt/Auth/AuthWidget> // we use Wt authentication system
#include <Wt/WStackedWidget>

#include "session.hpp"

#include "AppSession.hpp"
#include "AuthStateMachine.hpp"
#include "OWAStateMachine.hpp"
#include "WtDboData.hpp"
#include "LangDisplayAndSelectionWidget.hpp"

class WEnvironment;




struct wt_datas
{
	// to do switch to auth_dbo / main_dbo
	wt_datas(boost::shared_ptr<WtDboData> auth_dbo, boost::shared_ptr<WtDboData> main_dbo):
		m_auth_session(auth_dbo->info_str),
		m_auth_dbo(auth_dbo),
		m_main_dbo(main_dbo)
	{}

	// the Wt auth session object
	Session m_auth_session;

	// a dbo connexion (we may have several later)
	boost::shared_ptr<WtDboData> m_auth_dbo;
	boost::shared_ptr<WtDboData> m_main_dbo;
};


/*
 * Offirmo base web application
 * inheriting from default Wt::WApplication
 */
class OffirmoWebApp : public Wt::WApplication
{
public:
	/*
	 * The env argument contains information about the new session, and
	 * the initial request. It must be passed to the WApplication
	 * constructor so it is typically also an argument for your custom
	 * application constructor.
	 */
	OffirmoWebApp(const Wt::WEnvironment& env, boost::shared_ptr<WtDboData> auth_dbo_data, boost::shared_ptr<WtDboData> main_dbo_data);

	/* the auth widget should be available to other widgets/pages
	 */
	Wt::Auth::AuthWidget* getAuthWidget();

	// the very important app session !
	boost::shared_ptr<AppSession> get_app_session() const { return m_session; }


protected:

	// callback to handle an auth event (user registering, login, etc.)
	void handle_auth_event();
	// callback for when changing path (= user entered an url with a path)
	void handle_internal_path_event(const std::string &internalPath);

	void initialize_model();

	void initialize_css();

	void initialize_app_session_following_login();

	/* To be called when we think that the model has changed
	 * (TODO design better)
	 */
	void refresh();

protected:
	// list of message ressource bundles used for localization
	std::vector<std::string> m_msg_rsrc_bundles;

	void load_msg_rsrc_bundles();
	void load_msg_rsrc_bundle(std::string bundle_id);
	void check_msg_rsrc_bundle(std::string bundle_id);
	void check_specific_msg_rsrc_bundle(std::string bundle_id);
	void check_specific_msg_rsrc_bundle_marker(std::string bundle_id);


	/* Our internal widget stack,
	 * containing the pages that could be switched to.
	 */
	Wt::WStackedWidget m_app_stack;

	LangDisplayAndSelectionWidget m_lang_wgt;

	/* The default internal path we want (usually blank)
	 */
	static const std::string m_default_internal_path;

	/* Authentication state
	 */
	AuthStateMachine m_auth_state_machine;
	/* App state
	 */
	OWAStateMachine m_web_app_state_machine;

	/* Important variables we need for Wt auth system
	 */
	Wt::Auth::AuthModel  *m_pAuthModel;
	Wt::Auth::AuthWidget *m_pAuthWidget;

	struct wt_datas m_wt_datas;

	//
	boost::shared_ptr<AppSession> m_session;
};


#endif // __OFFIRMOWEBAPP_HPP_INCLUDED__
