/*
#include "TiedToAvatar.hpp"
 */

#ifndef TIED_TO_AVATAR_HPP_
#define TIED_TO_AVATAR_HPP_


#include "TiedToVirtualWorldInstance.hpp"

class Avatar;

class TiedToAvatar: public TiedToVirtualWorldInstance //, public TiedToRealUser
{
public:
	TiedToAvatar(const std::string& virtual_world_id = "", const std::string& virtual_world_instance_id = ""/*, const std::string& real_user_id = ""*/, const std::string& avatar_id = "");
	TiedToAvatar(const Avatar& avatar);

	std::string get_avatar_name() const { return m_avatar_id; }

	//std::string get_real_user_name() const { return m_real_user_id; }

	//////////////////////////////////// DBO ////////////////////////////////////
#ifdef USE_WT
	// this method is to be called inside the persist() method
	// of the immediate child.
	template<class Action>
	void persist_virtual(Action& a)
	{
		Wt::Dbo::field(a, m_avatar_id, "avatar");
		TiedToVirtualWorldInstance::persist_virtual(a);
	}
#endif // USE_WT
	//////////////////////////////////// DBO ////////////////////////////////////

protected:
	std::string m_avatar_id; //< the avatar name
	//std::string m_real_user_id; //< the user it belongs to
};

#endif /* TIED_TO_AVATAR_HPP_ */
