#include "TiedToVirtualWorldInstance.hpp"

#include "VirtualWorldInstance.hpp"


TiedToVirtualWorldInstance::TiedToVirtualWorldInstance(const std::string& virtual_world_id, const std::string& virtual_world_instance_id):
	TiedToVirtualWorld(virtual_world_id),
	m_instance_id(virtual_world_instance_id)
{
}
TiedToVirtualWorldInstance::TiedToVirtualWorldInstance(const VirtualWorldInstance& vwi):
	TiedToVirtualWorld(vwi.get_virtual_world_name()),
	m_instance_id(vwi.get_name())
{
}
