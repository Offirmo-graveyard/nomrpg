#include "VirtualWorld.hpp"


const std::string VirtualWorld::cm_default_name = "An unknown realm";


VirtualWorld::VirtualWorld(const std::string& name):
		NamedPersistableObject(name)
{
	m_STI_type = get_class_name();
}
