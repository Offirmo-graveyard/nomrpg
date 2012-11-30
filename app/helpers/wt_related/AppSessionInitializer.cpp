#include "AppSessionInitializer.hpp"

#include <boost/thread/thread.hpp>


#include "AppSessionUtils.hpp"


AppSessionSetter::AppSessionSetter()
{
	// nothing
}

AppSessionSetter::AppSessionSetter(boost::shared_ptr<AppSession> app_session):
	m_session(app_session)
{
	// nothing
}

AppSessionSetter::~AppSessionSetter()
{
	// nothing needed
}


template<class T>
T* allocate_virtual_world_element_with_correct_params(boost::shared_ptr<AppSession>& app_session)
{
	return new T(); // default params
}

template<class T>
Wt::Dbo::ptr<T> create_virtual_world_element(boost::shared_ptr<AppSession>& app_session, Wt::Dbo::Session& dbo_session)
{
	Wt::Dbo::ptr<T> element_ptr; // empty, evaluates to false
	T* p = NULL;

	// proceed with creation
	// first create it in memory
	{
		// in its own transaction because reads for params may incur db access ?
		Wt::Dbo::Transaction transaction(dbo_session);
		p = allocate_virtual_world_element_with_correct_params<T>(app_session);
		transaction.commit();
	}
	// then add it to db
	{
		// again, in its own transaction
		Wt::Dbo::Transaction transaction(dbo_session);
		element_ptr = dbo_session.add(p);
		transaction.commit();
		p = NULL;
	}

	return element_ptr;
}


template<class T>
void ensure_virtual_world_element(boost::shared_ptr<AppSession> app_session, Wt::Dbo::ptr<T>& element_ptr)
{
	if(element_ptr)
	{
		// already known and cached
		// nothing to do
	}
	else
	{
		// find or create one and cache it
		Wt::Dbo::Session& dbo_session = *(app_session->main_dbo_session());
		element_ptr = find_unique_virtual_world_element_int<T>(app_session, dbo_session);
		if (!element_ptr)
		{
			// none found (with no errors)
			// -> attempt to create one
			element_ptr = create_virtual_world_element<T>(app_session, dbo_session);
		}
	}
}

// specializations
template<>
RealUser* allocate_virtual_world_element_with_correct_params(boost::shared_ptr<AppSession>& app_session)
{
	return new RealUser(app_session->auto_user_id());
}
template<>
VirtualWorldGeneralParticipation* allocate_virtual_world_element_with_correct_params(boost::shared_ptr<AppSession>& app_session)
{
	return new VirtualWorldGeneralParticipation(*(app_session->real_user()), *(app_session->virtual_world()));
}
template<>
VirtualWorldInstance* allocate_virtual_world_element_with_correct_params(boost::shared_ptr<AppSession>& app_session)
{
	return new VirtualWorldInstance(*(app_session->virtual_world()));
}
template<>
VirtualWorldInstanceParticipation* allocate_virtual_world_element_with_correct_params(boost::shared_ptr<AppSession>& app_session)
{
	return new VirtualWorldInstanceParticipation(*(app_session->real_user()), *(app_session->instance()));
}
template<>
Avatar* allocate_virtual_world_element_with_correct_params(boost::shared_ptr<AppSession>& app_session)
{
	// XXX TO remove, avatar creation ?
	return new Avatar(*(app_session->real_user()), *(app_session->instance()), app_session->real_user()->get_name());
}


void AppSessionSetter::common_pre_setup_check()
{
	// prerequisite
	if (!m_session)
	{
		throw Mok::MissingRequiredDataException("the app session");
	}
	if (!m_session->main_dbo_session())
	{
		throw Mok::MissingRequiredDataException("the dbo session");
	}
}

void AppSessionSetter::set_up()
{
	common_pre_setup_check();

	attempt_set_up_to_avatar();
	//attempt_set_up_to_vwip();
}


void AppSessionSetter::set_up_to_vw()
{
	common_pre_setup_check();

	attempt_set_up_to_vw();
}


void AppSessionSetter::attempt_set_up_to_avatar()
{
	m_session->m_avatar.reset();

	attempt_set_up_to_vwip();

	if (m_session->m_instance_participation)
	{
		ensure_virtual_world_element<Avatar>(m_session, m_session->m_avatar);
		if (m_session->m_avatar)
		{
			std::cout << "Avatar selected; Updating last used avatar..." << std::endl;
			m_session->m_instance_participation.modify()->register_avatar_selection(*(m_session->m_avatar));
		}
	}
}
void AppSessionSetter::attempt_set_up_to_vwip()
{
	m_session->m_instance_participation.reset();

	attempt_set_up_to_vwi();

	if (m_session->m_instance)
	{
		ensure_virtual_world_element<VirtualWorldInstanceParticipation>(m_session, m_session->m_instance_participation);
	}
}
void AppSessionSetter::attempt_set_up_to_vwi()
{
	m_session->m_instance.reset();

	attempt_set_up_to_vwgp();

	if (m_session->m_general_participation)
	{
		ensure_virtual_world_element<VirtualWorldInstance>(m_session, m_session->m_instance);
		if (m_session->m_instance)
		{
			std::cout << "Instance selected; Updating last used instance..." << std::endl;
			m_session->m_general_participation.modify()->register_instance_selection(*(m_session->m_instance));
		}
	}
}
void AppSessionSetter::attempt_set_up_to_vwgp()
{
	m_session->m_general_participation.reset();

	attempt_set_up_to_vw();
	attempt_set_up_ru();

	if (m_session->m_virtual_world && m_session->m_user)
	{
		ensure_virtual_world_element<VirtualWorldGeneralParticipation>(m_session, m_session->m_general_participation);
	}
}
void AppSessionSetter::attempt_set_up_to_vw()
{
	m_session->m_virtual_world.reset();

	ensure_virtual_world_element<VirtualWorld>(m_session, m_session->m_virtual_world);
}
void AppSessionSetter::attempt_set_up_ru()
{
	m_session->m_user.reset();

	ensure_virtual_world_element<RealUser>(m_session, m_session->m_user);
}
