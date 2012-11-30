#include "VirtualWorldInstanceParticipation.hpp"

#include "Mok/Exception.hpp"

#include "RealUser.hpp"
#include "VirtualWorldInstance.hpp"
#include "Avatar.hpp"


VirtualWorldInstanceParticipation::VirtualWorldInstanceParticipation(const RealUser& ru, const VirtualWorldInstance& vwi):
	TiedToRealUser(ru),
	TiedToVirtualWorldInstance(vwi)
{
}


VirtualWorldInstanceParticipation::VirtualWorldInstanceParticipation()
{
}


void VirtualWorldInstanceParticipation::register_avatar_selection(const Avatar& av)
{
	// first ensure that this avatar is of our world
	if (av.get_virtual_world_name() != m_virtual_world_id)
	{
		throw Mok::IncoherentDataException("avatar does not match virtual world");
	}
	if (av.get_instance_name() != m_instance_id)
	{
		throw Mok::IncoherentDataException("instance does not match virtual world");
	}

	m_last_used_avatar_id = av.get_name();
}
