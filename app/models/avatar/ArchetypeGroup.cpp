#include "ArchetypeGroup.hpp"


ArchetypeGroup::ArchetypeGroup():
		NamedPersistableObject()
{
	m_STI_type = get_class_name();
}
