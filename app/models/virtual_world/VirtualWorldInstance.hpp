/*
#include "VirtualWorldInstance.hpp"
 */

#ifndef VIRTUAL_WORLD_INSTANCE_HPP_
#define VIRTUAL_WORLD_INSTANCE_HPP_


// ancestor class
#include "TiedToVirtualWorld.hpp"
#include "NamedPersistableObject.hpp"

class VirtualWorld;

#ifdef USE_WT
#include <Wt/Dbo/Dbo>
// specialization of some ancestor methods
WT_DBO_NAMED_OBJECT_CHILD_TEMPLATE_SPECIALIZATION(VirtualWorldInstance)

// typedefs
//class VirtualWorldInstance;
//typedef Wt::Dbo::collection< Wt::Dbo::ptr<VirtualWorldInstance> > VirtualWorldInstances;
#endif // USE_WT


/** An instance of a virtual world.
 * Several instances allow :
 * - different languages
 * - different game styles
 * - load balancing
 * - etc.
 */
class VirtualWorldInstance : public TiedToVirtualWorld, public NamedPersistableObject
{
public:
	static const std::string cm_default_name;

	VirtualWorldInstance(const VirtualWorld& vw, const std::string& name = VirtualWorldInstance::cm_default_name);
	VirtualWorldInstance(const std::string& virtual_world_id = "", const std::string& name = VirtualWorldInstance::cm_default_name);

	//////////////////////////////////// DBO ////////////////////////////////////
	// for STI (Single Type Inheritance). It MUST be equals to the class name.
	virtual const std::string get_class_name() const { return "VirtualWorldInstance"; }

#ifdef USE_WT
	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, m_lang,                     "lang");
		Wt::Dbo::field(a, m_jdr,                      "jdr");
		Wt::Dbo::field(a, m_pvp,                      "pvp");

		TiedToVirtualWorld::persist_virtual(a);
		NamedPersistableObject::persist_virtual(a);
	}
#endif // USE_WT
	//////////////////////////////////// DBO ////////////////////////////////////

protected:
	// primary key begin
	// + m_virtual_world_id (inherited)
	// + m_name (inherited)
	// primary key end

	std::string m_lang;
	bool m_jdr;
	bool m_pvp;
};


#endif /* VIRTUAL_WORLD_INSTANCE_HPP_ */
