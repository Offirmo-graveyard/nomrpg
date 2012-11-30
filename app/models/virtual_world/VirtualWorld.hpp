/*
#include "VirtualWorld.hpp"
 */

#ifndef VIRTUAL_WORLD_HPP_
#define VIRTUAL_WORLD_HPP_


// ancestor class
#include "NamedPersistableObject.hpp"

#ifdef USE_WT
#include <Wt/Dbo/Dbo>
// specialization of some ancestor methods
WT_DBO_NAMED_OBJECT_CHILD_TEMPLATE_SPECIALIZATION(VirtualWorld)

// typedefs
/*class VirtualWorld;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<VirtualWorld> > VirtualWorlds;*/
#endif // USE_WT


struct VirtualWorldUniqueKey
{
	std::string name;
};

/** A virtual world.
 * Like "world of warcraft", "Middle-Earth", etc.
 */
class VirtualWorld : public NamedPersistableObject
{
public:
	static const std::string cm_default_name;

	VirtualWorld(const std::string& name = VirtualWorld::cm_default_name);

	//////////////////////////////////// DBO ////////////////////////////////////
	// for STI (Single Type Inheritance). It MUST be equals to the class name.
	virtual const std::string get_class_name() const { return "VirtualWorld"; }

#ifdef USE_WT
	template<class Action>
	void persist(Action& a)
	{
		NamedPersistableObject::persist_virtual(a);

		Wt::Dbo::field(a, m_multiple_avatars_allowed, "multiple_avatars_allowed");
	}
#endif // USE_WT
	//////////////////////////////////// DBO ////////////////////////////////////

protected:
	// primary key begin
	// + m_name (inherited)
	// primary key end

	bool m_multiple_avatars_allowed;
};


// TODO
// will need server stop TODO
//void rename_virtual_world(Wt::Dbo::ptr<VirtualWorld> p, const std::string& new_name);
//void delete_virtual_world(Wt::Dbo::ptr<VirtualWorld> p);


#endif /* VIRTUAL_WORLD_HPP_ */
