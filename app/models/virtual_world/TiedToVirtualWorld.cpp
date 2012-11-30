#include "TiedToVirtualWorld.hpp"

#include "VirtualWorld.hpp"


TiedToVirtualWorld::TiedToVirtualWorld(const std::string& virtual_world_id):
	m_virtual_world_id(virtual_world_id)
{
}
TiedToVirtualWorld::TiedToVirtualWorld(const VirtualWorld& virtual_world):
	m_virtual_world_id(virtual_world.get_name())
{
}

TiedToVirtualWorld::~TiedToVirtualWorld()
{
}
