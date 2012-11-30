#include "VirtualWorldGeneralParticipation.hpp"

#include "Mok/Exception.hpp"

#include "RealUser.hpp"
#include "VirtualWorld.hpp"
#include "VirtualWorldInstance.hpp"


VirtualWorldGeneralParticipation::VirtualWorldGeneralParticipation(const RealUser& real_user, const VirtualWorld& vw):
	TiedToRealUser(real_user),
	TiedToVirtualWorld(vw)
{

}


VirtualWorldGeneralParticipation::VirtualWorldGeneralParticipation(const std::string& real_user_id, const std::string& virtual_world_id):
	TiedToRealUser(real_user_id),
	TiedToVirtualWorld(virtual_world_id)
{
	
}


void VirtualWorldGeneralParticipation::register_instance_selection(const VirtualWorldInstance& vwi)
{
	// first ensure that this instance is an instance of our world
	if (vwi.get_virtual_world_name() != m_virtual_world_id)
	{
		throw Mok::IncoherentDataException("instance does not match virtual world");
	}

	m_last_used_instance_id = vwi.get_name();
}
