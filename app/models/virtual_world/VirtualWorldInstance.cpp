#include "VirtualWorldInstance.hpp"

#include "VirtualWorld.hpp"


const std::string VirtualWorldInstance::cm_default_name = "Beta";


VirtualWorldInstance::VirtualWorldInstance(const VirtualWorld& vw, const std::string& name):
				TiedToVirtualWorld(vw),
				NamedPersistableObject(name),
				m_lang("fr"),
				m_jdr(false),
				m_pvp(false)
{

}


VirtualWorldInstance::VirtualWorldInstance(const std::string& virtual_world_id, const std::string& name):
		TiedToVirtualWorld(virtual_world_id),
		NamedPersistableObject(name),
		m_lang("fr"),
		m_jdr(false),
		m_pvp(false)
{
	
}

