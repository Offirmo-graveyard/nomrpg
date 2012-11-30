/*
#include "TiedToVirtualWorldInstance.hpp"
 */

#ifndef TIED_TO_VIRTUAL_WORLD_INSTANCE_HPP_
#define TIED_TO_VIRTUAL_WORLD_INSTANCE_HPP_


// Ancestor class
#include "TiedToVirtualWorld.hpp"

class VirtualWorldInstance;


class TiedToVirtualWorldInstance : public TiedToVirtualWorld
{
public:
	TiedToVirtualWorldInstance(const std::string& virtual_world_id = "", const std::string& virtual_world_instance_id = "");
	TiedToVirtualWorldInstance(const VirtualWorldInstance& vwi);

	std::string get_instance_name() const { return m_instance_id; }

	//////////////////////////////////// DBO ////////////////////////////////////
#ifdef USE_WT
	// this method is to be called inside the persist() method
	// of the immediate child.
	template<class Action>
	void persist_virtual(Action& a)
	{
		Wt::Dbo::field(a, m_instance_id, "instance");
		TiedToVirtualWorld::persist_virtual(a);
	}
#endif // USE_WT
	//////////////////////////////////// DBO ////////////////////////////////////

protected:
	std::string m_instance_id;
};

#endif /* TIED_TO_VIRTUAL_WORLD_INSTANCE_HPP_ */
