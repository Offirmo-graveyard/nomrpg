#include "AppSessionUtils.hpp"

/////// specializations
/// RealUser
template<>
Wt::Dbo::collection< Wt::Dbo::ptr<RealUser> > find_virtual_world_elements_request(boost::shared_ptr<AppSession>& app_session, Wt::Dbo::Session& dbo_session)
{
	Wt::Dbo::Transaction transaction(dbo_session);
	Wt::Dbo::collection< Wt::Dbo::ptr<RealUser> > elements = dbo_session.find<RealUser>()
										.where("denomination = ?").bind(app_session->auto_user_id());
	transaction.commit();
	return elements;
}
/// VirtualWorld
template<>
Wt::Dbo::collection< Wt::Dbo::ptr<VirtualWorld> > find_virtual_world_elements_request(boost::shared_ptr<AppSession>& app_session, Wt::Dbo::Session& dbo_session)
{
	Wt::Dbo::Transaction transaction(dbo_session);
	Wt::Dbo::collection< Wt::Dbo::ptr<VirtualWorld> > elements = dbo_session.find<VirtualWorld>();
	transaction.commit();
	return elements;
}
/// VirtualWorld GP
template<>
Wt::Dbo::collection< Wt::Dbo::ptr<VirtualWorldGeneralParticipation> > find_virtual_world_elements_request(boost::shared_ptr<AppSession>& app_session, Wt::Dbo::Session& dbo_session)
{
	Wt::Dbo::Transaction transaction(dbo_session);
	Wt::Dbo::collection< Wt::Dbo::ptr<VirtualWorldGeneralParticipation> > elements = dbo_session.find<VirtualWorldGeneralParticipation>()
								.where("virtual_world = ?").bind(app_session->virtual_world()->get_denomination())
								.where("user = ?").bind(app_session->real_user()->get_denomination());
	transaction.commit();
	return elements;
}
/// Instance
template<>
Wt::Dbo::collection< Wt::Dbo::ptr<VirtualWorldInstance> > find_virtual_world_elements_request(boost::shared_ptr<AppSession>& app_session, Wt::Dbo::Session& dbo_session)
{
	Wt::Dbo::Transaction transaction(dbo_session);

	std::string last_instance_name = app_session->general_participation()->get_last_instance_name();
	Wt::Dbo::collection< Wt::Dbo::ptr<VirtualWorldInstance> > elements;
	if (!last_instance_name.empty())
	{
		elements = dbo_session.find<VirtualWorldInstance>()
								.where("virtual_world = ?").bind(app_session->virtual_world()->get_denomination())
								.where("denomination = ?").bind(last_instance_name);
	}
	if (elements.size() == 0)
	{
		// either no last instance name or this instance was not found -> find any instance
		elements = dbo_session.find<VirtualWorldInstance>()
								.where("virtual_world = ?").bind(app_session->virtual_world()->get_denomination());
	}
	transaction.commit();
	return elements;
}
/// InstanceParticipation
template<>
Wt::Dbo::collection< Wt::Dbo::ptr<VirtualWorldInstanceParticipation> > find_virtual_world_elements_request(boost::shared_ptr<AppSession>& app_session, Wt::Dbo::Session& dbo_session)
{
	Wt::Dbo::Transaction transaction(dbo_session);
	Wt::Dbo::collection< Wt::Dbo::ptr<VirtualWorldInstanceParticipation> > elements = dbo_session.find<VirtualWorldInstanceParticipation>()
								.where("virtual_world = ?").bind(app_session->virtual_world()->get_denomination())
								.where("instance = ?").bind(app_session->instance()->get_denomination())
								.where("user = ?").bind(app_session->real_user()->get_denomination());
	transaction.commit();
	return elements;
}
/// Avatar
template<>
Wt::Dbo::collection< Wt::Dbo::ptr<Avatar> > find_virtual_world_elements_request(boost::shared_ptr<AppSession>& app_session, Wt::Dbo::Session& dbo_session)
{
	Wt::Dbo::Transaction transaction(dbo_session);

	std::string last_avatar_name = app_session->instance_participation()->get_last_avatar_name();
	Wt::Dbo::collection< Wt::Dbo::ptr<Avatar> > elements;
	if (!last_avatar_name.empty())
	{
		elements = dbo_session.find<Avatar>()
								.where("virtual_world = ?").bind(app_session->virtual_world()->get_denomination())
								.where("user = ?").bind(app_session->real_user()->get_denomination())
								.where("denomination = ?").bind(last_avatar_name);
	}
	if (elements.size() == 0)
	{
		// either no last instance name or this instance was not found -> find any instance
		elements = dbo_session.find<Avatar>()
								.where("virtual_world = ?").bind(app_session->virtual_world()->get_denomination())
								.where("user = ?").bind(app_session->real_user()->get_denomination());
	}
	transaction.commit();
	return elements;
}
