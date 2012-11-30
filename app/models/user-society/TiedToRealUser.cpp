#include "TiedToRealUser.hpp"

#include "RealUser.hpp"


TiedToRealUser::TiedToRealUser(const std::string& real_user_id):
	m_real_user_id(real_user_id)
{
}

TiedToRealUser::TiedToRealUser(const RealUser& real_user):
	m_real_user_id(real_user.get_name())
{
}

TiedToRealUser::~TiedToRealUser()
{
}
