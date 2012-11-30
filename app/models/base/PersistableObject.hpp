/* Base class for persistable objects via DBO
 *
 */
/*
#include "PersistableObject.hpp"
 */

#ifndef PERSISTABLE_OBJECT_HPP_INCLUDED__
#define PERSISTABLE_OBJECT_HPP_INCLUDED__



#include "Object.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>


#ifdef USE_WT
#include <typeinfo> // for a hack

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/WtSqlTraits>
#include <Wt/Dbo/DbAction> // for a hack
#endif // USE_WT


class PersistableObject : public Object
{
public:
	PersistableObject();

	const std::string& get_STI_type() const { return m_STI_type; }

	//////////////////////////////////// DBO ////////////////////////////////////
#ifdef USE_WT
	// this method is to be called inside the persist() method
	// of the immediate child.
	template<class Action>
	void persist_virtual(Action& a)
	{
		//std::cout << "  typeid = " << typeid(a).name() << std::endl;

		if (typeid(&a) == typeid(Wt::Dbo::SessionAddAction*))
		{
			visited_by_session_add_action();
		}

		Wt::Dbo::field(a, m_creation_date,   "creation_date");
		Wt::Dbo::field(a, m_last_modif_date, "last_modif_date");
		Wt::Dbo::field(a, m_STI_type,        "sti_type");
	}

	// DBO callbacks
	virtual void visited_by_session_add_action() { register_modification(); }
#endif // USE_WT
	//////////////////////////////////// DBO ////////////////////////////////////

	const boost::posix_time::ptime get_creation_date() const { return m_creation_date; }
	const boost::posix_time::ptime get_last_modification_date() const { return m_last_modif_date; }

	// normally called automatically when modified
	void register_modification() { m_last_modif_date = boost::posix_time::microsec_clock::universal_time(); }

protected:
	// common fields we want every object to have
	std::string   m_STI_type; // this is for STI (Single Type Inheritance).
	                          // It MUST be equals to the class name.
	                          // It MUST be set by the latest child constructor
	                          // Note : not really used so far ;)
	boost::posix_time::ptime m_creation_date;
	boost::posix_time::ptime m_last_modif_date;
};

#endif // PERSISTABLE_OBJECT_HPP_INCLUDED__
