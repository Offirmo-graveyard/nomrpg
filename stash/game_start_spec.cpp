#include <iostream>
#include <typeinfo>

#include "off_spec.hpp"
#include "off_spec_wt_model.hpp"

#include "wt_spec_helpers.hpp"


// XXX todo comment, refactor, redesign


DESCRIBE(GameStart)
{
	IT(can_get_or_create_the_current_user)
	{
		TRACE_CURRENT_TEST();

		// creating if missing : should we ? No, we can't,
		// because a user is identified by a username
		// that should be asked to the user.

		// retrieving
		Wt::Dbo::ptr<RealUser> pdbo = SpecHelp::Wt::s_app_session->user();
		SHOULD_BE_TRUE(pdbo);

		std::cout << "Current user is " << pdbo->get_name() << std::endl;
	}

	IT(can_get_or_create_the_current_virtual_world)
	{
		TRACE_CURRENT_TEST();

		// creating if missing : should we ? No need for now.

		// retrieving
		Wt::Dbo::ptr<VirtualWorld> pdbo = SpecHelp::Wt::s_app_session->virtual_world();
		SHOULD_BE_TRUE(pdbo);

		std::cout << "Current virtual world = " << pdbo->get_name() << std::endl;
	}

	IT(can_get_or_create_infos_about_this_user_presence_in_this_virtual_world)
	{
		TRACE_CURRENT_TEST();

		Wt::Dbo::ptr<VirtualWorldGeneralParticipation> pdbo = SpecHelp::Wt::s_app_session->general_participation();
		SHOULD_BE_TRUE(pdbo);
	}

	IT(can_get_or_create_infos_about_this_user_current_instance)
	{
		TRACE_CURRENT_TEST();

		Wt::Dbo::ptr<VirtualWorldInstance> pdbo = SpecHelp::Wt::s_app_session->instance();
		SHOULD_BE_TRUE(pdbo);

		std::cout << "Current virtual world instance = " << pdbo->get_name() << std::endl;
	}

	IT(can_get_or_create_infos_about_this_user_presence_in_this_instance)
	{
		TRACE_CURRENT_TEST();

		Wt::Dbo::ptr<VirtualWorldInstanceParticipation> pdbo = SpecHelp::Wt::s_app_session->instance_participation();
		SHOULD_BE_TRUE(pdbo);
	}

	IT(can_get_or_create_the_current_avatar)
	{
		TRACE_CURRENT_TEST();

		// retrieving
		Wt::Dbo::ptr<Avatar> pdbo = SpecHelp::Wt::s_app_session->avatar();
		SHOULD_BE_TRUE(pdbo);

		std::cout << "Current avatar = " << pdbo->get_name() << std::endl;
	}

}
