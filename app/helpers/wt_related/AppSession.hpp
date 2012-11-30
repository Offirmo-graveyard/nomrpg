/* This class conveniently stores
 * pointers on all important session objects.
 *
#include "AppSession.hpp"
 */

#ifndef APP_SESSION_HPP_
#define APP_SESSION_HPP_


#include <boost/shared_ptr.hpp>

#include "Wt/WException"

#include <Wt/Dbo/Dbo>


// objects we will store
class OffirmoWebApp;
#include "RealUser.hpp"
#include "VirtualWorld.hpp"
#include "VirtualWorldGeneralParticipation.hpp"
#include "VirtualWorldInstance.hpp"
#include "VirtualWorldInstanceParticipation.hpp"
#include "Avatar.hpp"

class AppSessionSetter;



/** This class is meant to store pointers
 * on a lot of "singleton-like" objects
 * that are tied to the App session.
 * They should live along the session
 * and are needed in a lot of places.
 * A cache system is used to avoid unnecessary DB access.
 */
class AppSession
{
public:

	/** This class could be a singleton,
	 * design is not fixed yet.
	 * So keep it an ordinary class for now.
	 */
	AppSession();
	virtual ~AppSession();

	// a helper class
	friend class AppSessionSetter;

	/** Register for storage for :
	 * - easy access by any other class
	 * - persistence of those pointers
	 */
	void register_main_dbo_session(boost::shared_ptr<Wt::Dbo::SqlConnection>& backend, boost::shared_ptr<Wt::Dbo::Session>& session);
	boost::shared_ptr<Wt::Dbo::Session> main_dbo_session() const;

	void register_web_app(boost::shared_ptr<OffirmoWebApp>& web_app);

	void register_current_auto_user_id(std::string id) { m_auto_user_id = id; }
	std::string auto_user_id() { return m_auto_user_id; }

	void register_current_user(Wt::Dbo::ptr<RealUser>& user);
	Wt::Dbo::ptr<RealUser> real_user() const;

	// optional
	void register_virtual_world(Wt::Dbo::ptr<VirtualWorld>& virtual_world);
	Wt::Dbo::ptr<VirtualWorld> virtual_world(); // we can find or create one if none registered

	// no setter, set automatically
	Wt::Dbo::ptr<VirtualWorldGeneralParticipation> general_participation();

	// optional
	void register_instance(Wt::Dbo::ptr<VirtualWorldInstance>& vwi);
	Wt::Dbo::ptr<VirtualWorldInstance> instance(); // we can find or create one if none registered

	// no setter, set automatically
	Wt::Dbo::ptr<VirtualWorldInstanceParticipation> instance_participation();

	void register_current_avatar(Wt::Dbo::ptr<Avatar>& avatar);
	Wt::Dbo::ptr<Avatar> avatar();  // we *sometime* can find or create one if none registered

protected:
	// the dbo session
	// main because there may be several
	boost::shared_ptr<Wt::Dbo::Session>       m_main_dbo_session;
	boost::shared_ptr<Wt::Dbo::SqlConnection> m_main_dbo_session_backend;

	// the web app
	boost::shared_ptr<OffirmoWebApp> m_web_app;

	// the current user
	std::string m_auto_user_id; // the "auto" user
	Wt::Dbo::ptr<RealUser> m_user; // the "real" user

	// The current virtual world
	Wt::Dbo::ptr<VirtualWorld> m_virtual_world;

	// the current general participation
	Wt::Dbo::ptr<VirtualWorldGeneralParticipation> m_general_participation;

	// the current instance
	Wt::Dbo::ptr<VirtualWorldInstance> m_instance;

	// the current instance participation
	Wt::Dbo::ptr<VirtualWorldInstanceParticipation> m_instance_participation;

	// the current avatar
	Wt::Dbo::ptr<Avatar> m_avatar;
};

#endif /* APP_SESSION_HPP_ */
