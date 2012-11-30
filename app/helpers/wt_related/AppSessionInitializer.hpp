/* This class is a helper class
 * for filling a AppSession.
 *
#include "AppSessionInitializer.hpp"
 */

#ifndef APP_SESSION_START_HPP_
#define APP_SESSION_START_HPP_


#include "AppSession.hpp"


class AppSessionSetter
{
public:
	AppSessionSetter();
	AppSessionSetter(boost::shared_ptr<AppSession> app_session);
	~AppSessionSetter();

	/** This method is able to automatically figure out the remaining data,
	 * and even to create missing one if needed.
	 * Note : this method may be called repetedly
	 * if new needed infos become available.
	 */
	void set_up();

	// farther we can go without a real_user
	void set_up_to_vw();


	void attempt_set_up_to_avatar();
	void attempt_set_up_to_vwip();
	void attempt_set_up_to_vwi();
	void attempt_set_up_to_vwgp();
	void attempt_set_up_to_vw();

	void attempt_set_up_ru();

private:
	void ensure_virtual_world();
	void ensure_general_participation();

	void common_pre_setup_check();

	boost::shared_ptr<AppSession> m_session;
};


#endif /* APP_SESSION_START_HPP_ */
