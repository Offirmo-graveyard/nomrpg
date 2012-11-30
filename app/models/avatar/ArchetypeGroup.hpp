/*
#include "ArchetypeGroup.hpp"
 */

#ifndef ARCHETYPE_GROUP_HPP_
#define ARCHETYPE_GROUP_HPP_

#include "NamedPersistableObject.hpp"

#ifdef USE_WT
#include <Wt/Dbo/Dbo>
// specialization of some ancestor methods
WT_DBO_NAMED_OBJECT_CHILD_TEMPLATE_SPECIALIZATION(ArchetypeGroup)
#endif // USE_WT

class ArchetypeGroup: public NamedPersistableObject
{
public:
	ArchetypeGroup();
	virtual ~ArchetypeGroup() {}

	//////////////////////////////////// DBO ////////////////////////////////////
	// STI
	virtual const std::string get_class_name() const { return "ArchetypeGroup"; }

#ifdef USE_WT
	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, m_required,     "required");
		Wt::Dbo::field(a, m_changeable,   "changeable");
		Wt::Dbo::field(a, m_predecessor,  "predecessor");

		NamedPersistableObject::persist_virtual(a);
	}
#endif // USE_WT
	//////////////////////////////////// DBO ////////////////////////////////////

protected:
	std::string m_predecessor; //< which other AG is immediately needed before this one ?
	                           // If none, this is the first choice that should be given to the user
	                           // of course, only one AG may have no predecessor !
	bool m_required; //< Is the choice of an A from this AG mandatory ? If yes, choice will be given at character creation.
	// default_specialization; TODO
	bool m_changeable; // can the user change its A for this AG ?
	//bool m_unique_on_PVP_server; // can the user create an avatar with another A for this AG on a PVP server ? (useful for ennemy factions)
};

#endif /* ARCHETYPE_GROUP_HPP_ */
