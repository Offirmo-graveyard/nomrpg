/*
#include "Avatar.hpp"
 */

#ifndef AVATAR_HPP_
#define AVATAR_HPP_


// ancestor classes
#include "NaturalPerson.hpp"
#include "TiedToVirtualWorldInstance.hpp"
#include "TiedToRealUser.hpp"

//class RealUser;


#ifdef USE_WT
#include <Wt/Dbo/Dbo>
// specialization of some ancestor methods
WT_DBO_NAMED_OBJECT_CHILD_TEMPLATE_SPECIALIZATION(Avatar)

// typedefs
class Avatar;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<Avatar> > Avatars;
#endif // USE_WT


/* An avatar is the incarnation of a user
 * inside a specific instance of a specific virtual world.
 * It is tied to a user.
 * It cannot (really ?) exists without a user.
 * It has its own denomination, different from the user's.
 */
class Avatar: public NaturalPerson, public TiedToRealUser, public TiedToVirtualWorldInstance
{
public:
	Avatar(const std::string& p_denomination = Person::cm_default_denomination);
	Avatar(const RealUser& ru, const VirtualWorldInstance& vwi, const std::string& p_denomination = Person::cm_default_denomination);

	//////////////////////////////////// DBO ////////////////////////////////////
	// for STI (Single Type Inheritance). It MUST be equals to the class name.
	virtual const std::string get_class_name() const { return "Avatar"; }

#ifdef USE_WT
	template<class Action>
	void persist(Action& a)
	{
		NaturalPerson::persist_virtual(a);
		TiedToRealUser::persist_virtual(a);
		TiedToVirtualWorldInstance::persist_virtual(a);
	}
#endif // USE_WT
	//////////////////////////////////// DBO ////////////////////////////////////

protected:
	// primary key begin
	// + real_user_id (inherited)
	// + m_virtual_world_id (inherited)
	// + m_instance_id (inherited)
	// + m_name (inherited)
	// primary key end
};


#endif /* AVATAR_HPP_ */
