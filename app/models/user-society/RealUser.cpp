#include "RealUser.hpp"


RealUser::RealUser(const std::string& p_denomination):
		NaturalPerson(p_denomination)
{
	m_STI_type = get_class_name();
}
