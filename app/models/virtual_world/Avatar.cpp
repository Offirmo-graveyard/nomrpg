#include "Avatar.hpp"

#include "RealUser.hpp"
#include "VirtualWorldInstance.hpp"

Avatar::Avatar(const RealUser& ru, const VirtualWorldInstance& vwi, const std::string& p_denomination):
	NaturalPerson(p_denomination),
	TiedToRealUser(ru),
	TiedToVirtualWorldInstance(vwi)
{
	m_STI_type = get_class_name();
}

Avatar::Avatar(const std::string& p_denomination):
		NaturalPerson(p_denomination)
{
	m_STI_type = get_class_name();
}
