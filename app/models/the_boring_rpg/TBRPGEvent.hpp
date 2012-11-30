/*
#include "TBRPGEvent.hpp"
 */
#ifndef THEBORINGRPGEVENT_HPP_
#define THEBORINGRPGEVENT_HPP_


#include <string>

// ancestor classes
#include "PersistableObject.hpp"


/* To store a Boring RPG event.
 * In the simplest form, contain just a unique id
 * wich can be used as a key for localisation.
 * In more complicated form,
 * will hold instructions about what happen :
 * gain X gold, X experience, etc.
 */
class TBRPGEvent : public PersistableObject
{
public:
	static const int MAX_ID_SIZE = 70; // for persistence

	TBRPGEvent(const std::string& event_id = "");

	std::string get_id() const { return m_id; }

	//////////////////////////////////// DBO ////////////////////////////////////
	// for STI (Single Type Inheritance). It MUST be equals to the class name.
	virtual const std::string get_class_name() const { return "TBRPGEvent"; }

#ifdef USE_WT
	template<class Action>
	void persist(Action& a)
	{
		PersistableObject::persist_virtual(a);

		// set the id as primary key
		Wt::Dbo::id(a, m_id, "id", TBRPGEvent::MAX_ID_SIZE);
	}
#endif // USE_WT
	//////////////////////////////////// DBO ////////////////////////////////////

protected:
	std::string m_id;
};


#endif /* THEBORINGRPGEVENT_HPP_ */
