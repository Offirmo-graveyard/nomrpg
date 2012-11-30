/*
#include "TiedToRealUser.hpp"
 */
#ifndef TIED_TO_REAL_USER_HPP_
#define TIED_TO_REAL_USER_HPP_


#include <string>

#ifdef USE_WT
#include <Wt/Dbo/Dbo>
#endif

class RealUser;


class TiedToRealUser
{
public:
	TiedToRealUser(const std::string& real_user_id = "");
	TiedToRealUser(const RealUser& real_user);

	virtual ~TiedToRealUser(); // just for declaring it virtual

	std::string get_real_user_name() const { return m_real_user_id; }

	//////////////////////////////////// DBO ////////////////////////////////////
#ifdef USE_WT
	// this method is to be called inside the persist() method
	// of the immediate child.
	template<class Action>
	void persist_virtual(Action& a)
	{
		Wt::Dbo::field(a, m_real_user_id, "user");
	}
#endif // USE_WT
	//////////////////////////////////// DBO ////////////////////////////////////

protected:
	std::string m_real_user_id;
};

#endif /* TIED_TO_REAL_USER_HPP_ */
