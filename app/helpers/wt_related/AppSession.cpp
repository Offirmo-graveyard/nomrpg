#include "AppSession.hpp"

#include "Mok/Exception.hpp"

#include "AppSessionInitializer.hpp"




AppSession::AppSession()
{
	// default values are OK
}

AppSession::~AppSession()
{
	// nothing special
}


void AppSession::register_main_dbo_session(boost::shared_ptr<Wt::Dbo::SqlConnection>& backend, boost::shared_ptr<Wt::Dbo::Session>& session)
{
	if (m_main_dbo_session_backend)
	{
		throw Mok::DataNotRedefinableException("main_dbo_session_backend");
	}
	if(m_main_dbo_session)
	{
		throw Mok::DataNotRedefinableException("main_dbo_session");
	}

	m_main_dbo_session_backend = backend;
	m_main_dbo_session = session;
}

boost::shared_ptr<Wt::Dbo::Session> AppSession::main_dbo_session() const
{
	if (!m_main_dbo_session)
	{
		throw Mok::DataNotAvailableException("main_dbo_session");
	}
	return m_main_dbo_session;
}


void AppSession::register_web_app(boost::shared_ptr<OffirmoWebApp>& web_app)
{
	if(m_web_app)
	{
		throw Mok::DataNotRedefinableException("web_app");
	}
	m_web_app = web_app;
}


void AppSession::register_current_user(Wt::Dbo::ptr<RealUser>& user)
{
	if(m_user)
	{
		throw Mok::DataNotRedefinableException("user");
	}
	m_user = user;
}

Wt::Dbo::ptr<RealUser> AppSession::real_user() const
{
	if (!m_user)
	{
		throw Mok::DataNotAvailableException("user");
	}
	return m_user;
}


void AppSession::register_virtual_world(Wt::Dbo::ptr<VirtualWorld>& p_virtual_world)
{
	if(m_virtual_world)
	{
		throw Mok::DataNotRedefinableException("virtual_world");
	}
	m_virtual_world = p_virtual_world;
}

Wt::Dbo::ptr<VirtualWorld> AppSession::virtual_world()
{
	if (!m_virtual_world)
	{
		throw Mok::DataNotAvailableException("virtual_world");
	}
	return m_virtual_world;
}


Wt::Dbo::ptr<VirtualWorldGeneralParticipation> AppSession::general_participation()
{
	if (!m_general_participation)
	{
		throw Mok::DataNotAvailableException("general_participation");
	}
	return m_general_participation;
}


void AppSession::register_instance(Wt::Dbo::ptr<VirtualWorldInstance>& vwi)
{
	if (!m_virtual_world)
	{
		//register_virtual_world(...);
	}
	if (!m_virtual_world || m_virtual_world->get_name() != vwi->get_virtual_world_name())
	{
		throw Mok::IncoherentDataException("instance with a different virtual_world");
	}
	if (m_instance && vwi->get_name() == m_instance->get_name())
	{
		// no change, ignore
	}
	else
	{
		m_instance = vwi;
		m_avatar.reset();
	}
}

Wt::Dbo::ptr<VirtualWorldInstance> AppSession::instance()
{
	if (!m_instance)
	{
		throw Mok::DataNotAvailableException("instance");
	}
	return m_instance;
}


Wt::Dbo::ptr<VirtualWorldInstanceParticipation> AppSession::instance_participation()
{
	if (!m_instance_participation)
	{
		throw Mok::DataNotAvailableException("instance_participation");
	}
	return m_instance_participation;
}


void AppSession::register_current_avatar(Wt::Dbo::ptr<Avatar>& p_avatar)
{
	if (!m_user)
	{
		//register_current_user(...);
	}
	if (!m_user || m_user->get_name() != p_avatar->get_real_user_name())
	{
		throw Mok::IncoherentDataException("avatar with a different user");
	}
	if (!m_virtual_world)
	{
		//register_virtual_world(...);
	}
	if (!m_virtual_world || m_virtual_world->get_name() != p_avatar->get_virtual_world_name())
	{
		throw Mok::IncoherentDataException("avatar with a different virtual world");
	}
	if (!m_instance)
	{
		//register_virtual_world_instance(...);
	}
	if (!m_instance || m_instance->get_name() != p_avatar->get_instance_name())
	{
		throw Mok::IncoherentDataException("avatar with a different instance");
	}

	m_avatar = p_avatar;
}

Wt::Dbo::ptr<Avatar> AppSession::avatar()
{
	if (!m_avatar)
	{
		throw Mok::DataNotAvailableException("avatar");
	}
	return m_avatar;
}
