/*
#include "TiedToVirtualWorld.hpp"
 */

#ifndef TIED_TO_VIRTUAL_WORLD_HPP_
#define TIED_TO_VIRTUAL_WORLD_HPP_


#include <string>

#ifdef USE_WT
#include <Wt/Dbo/Dbo>
#endif

class VirtualWorld;


class TiedToVirtualWorld
{
public:
	TiedToVirtualWorld(const std::string& virtual_world_id = "");
	TiedToVirtualWorld(const VirtualWorld& virtual_world);

	virtual ~TiedToVirtualWorld(); // just for declaring it virtual

	std::string get_virtual_world_name() const { return m_virtual_world_id; }

	//////////////////////////////////// DBO ////////////////////////////////////
#ifdef USE_WT
	// this method is to be called inside the persist() method
	// of the immediate child.
	template<class Action>
	void persist_virtual(Action& a)
	{
		Wt::Dbo::field(a, m_virtual_world_id, "virtual_world");
	}
#endif // USE_WT
	//////////////////////////////////// DBO ////////////////////////////////////

protected:
	std::string m_virtual_world_id;
};

#endif /* TIED_TO_VIRTUAL_WORLD_HPP_ */
