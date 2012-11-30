#include "TiedToAvatar.hpp"

#include "Avatar.hpp"

TiedToAvatar::TiedToAvatar(const std::string& virtual_world_id, const std::string& virtual_world_instance_id/*, const std::string& real_user_id*/, const std::string& avatar_id):
	TiedToVirtualWorldInstance(virtual_world_id,  virtual_world_instance_id),
	m_avatar_id(avatar_id)
	//m_real_user_id(real_user_id)
{
}

TiedToAvatar::TiedToAvatar(const Avatar& avatar):
	TiedToVirtualWorldInstance(avatar.get_virtual_world_name(), avatar.get_instance_name()),
	m_avatar_id(avatar.get_name())
	//m_real_user_id(avatar.get_real_user_name())
{
}


