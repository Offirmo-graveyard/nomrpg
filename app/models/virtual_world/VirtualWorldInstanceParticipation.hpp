/*
#include "VirtualWorldInstanceParticipation.hpp"
 */

#ifndef VIRTUAL_WORLD_PARTICIPATION_HPP_
#define VIRTUAL_WORLD_PARTICIPATION_HPP_


// ancestor classes
#include "TiedToRealUser.hpp"
#include "TiedToVirtualWorldInstance.hpp"
#include "PersistableObject.hpp"

class RealUser;
class VirtualWorldInstance;
class Avatar;


#ifdef USE_WT
#include <Wt/Dbo/Dbo>
// typedefs
/*class VirtualWorldInstanceParticipation;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<VirtualWorldInstanceParticipation> > VirtualWorldInstanceParticipations;

// our primary key
struct VirtualWorldInstanceParticipationKey
{
	std::string real_user_id;
	std::string virtual_world_id;
	std::string instance_id;
};*/
#endif // USE_WT


/** Store informations about a user
 * related to a specific instance of a specific virtual world.
 * (but NOT infos related to an avatar)
 */
class VirtualWorldInstanceParticipation : public TiedToRealUser, public TiedToVirtualWorldInstance, public PersistableObject
{
public:
	VirtualWorldInstanceParticipation(const RealUser& ru, const VirtualWorldInstance& vwi);
	VirtualWorldInstanceParticipation();

	std::string get_last_avatar_name() const { return m_last_used_avatar_id; }

	void register_avatar_selection(const Avatar& av);

	//////////////////////////////////// DBO ////////////////////////////////////
	// for STI (Single Type Inheritance). It MUST be equals to the class name.
	virtual const std::string get_class_name() const { return "VirtualWorldInstanceParticipation"; }

#ifdef USE_WT
	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a,     m_last_used_avatar_id,    "last_used_avatar");

		TiedToRealUser::persist_virtual(a);
		TiedToVirtualWorldInstance::persist_virtual(a);
		PersistableObject::persist_virtual(a);
	}
#endif // USE_WT
	//////////////////////////////////// DBO ////////////////////////////////////

protected:
	// primary key begin
	//std::string m_real_user_id;
	//std::string m_virtual_world_id;
	//std::string m_instance_id;
	// primary key end

	std::string m_last_used_avatar_id;
};

#endif /* VIRTUAL_WORLD_PARTICIPATION_HPP_ */
