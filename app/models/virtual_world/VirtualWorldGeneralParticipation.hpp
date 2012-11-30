/*
#include "virtual_world_general_participation.hpp"
 */

#ifndef VIRTUAL_WORLD_GENERAL_PARTICIPATION_HPP_
#define VIRTUAL_WORLD_GENERAL_PARTICIPATION_HPP_


// ancestor classes
#include "TiedToVirtualWorld.hpp"
#include "TiedToRealUser.hpp"
#include "PersistableObject.hpp"

class RealUser;
class VirtualWorld;
class VirtualWorldInstance;


#ifdef USE_WT
#include <Wt/Dbo/Dbo>
// typedefs
/*
class VirtualWorldGeneralParticipation;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<VirtualWorldGeneralParticipation> > VirtualWorldGeneralParticipations;
*/
// our primary key
/*struct VirtualWorldGeneralParticipationKey
{
	std::string real_user_id;
	std::string virtual_world_id;
};*/
#endif // USE_WT


/** Store informations about a user
 * related to a specific virtual world.
 * (but NOT infos related to an instance or an avatar)
 */
class VirtualWorldGeneralParticipation : public TiedToRealUser, public TiedToVirtualWorld, public PersistableObject
{
public:
	VirtualWorldGeneralParticipation(const RealUser& real_user, const VirtualWorld& vw);
	VirtualWorldGeneralParticipation(const std::string& real_user_id = "", const std::string& virtual_world_id = "");

	std::string get_last_instance_name() const { return m_last_used_instance_id; }

	void register_instance_selection(const VirtualWorldInstance& vwi);

	//////////////////////////////////// DBO ////////////////////////////////////
	// for STI (Single Type Inheritance). It MUST be equals to the class name.
	virtual const std::string get_class_name() const { return "VirtualWorldGeneralParticipation"; }

#ifdef USE_WT
	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a,     m_last_used_instance_id,  "last_used_instance");

		TiedToRealUser::persist_virtual(a);
		TiedToVirtualWorld::persist_virtual(a);
		PersistableObject::persist_virtual(a);
	}
#endif // USE_WT
	//////////////////////////////////// DBO ////////////////////////////////////

protected:
	// primary key
	// std::string m_real_user_id;
	//std::string m_virtual_world_id;
	// end of primary key

	std::string m_last_used_instance_id;
};

#endif /* VIRTUAL_WORLD_GENERAL_PARTICIPATION_HPP_ */
