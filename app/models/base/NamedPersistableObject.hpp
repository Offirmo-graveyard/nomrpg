/*
 * A lot of persistable objects also have a name / designation.
 * With this class, we add this common trait.
 *
#include "NamedPersistableObject.hpp"
 */

#ifndef NAMED_PERSISTABLE_OBJECT_HPP_
#define NAMED_PERSISTABLE_OBJECT_HPP_


#include "PersistableObject.hpp"


class NamedPersistableObject : public PersistableObject
{
public:
	static const int MAX_DENOMINATION_SIZE = 70; // for persistence

	NamedPersistableObject(const std::string& denomination = ""):
		PersistableObject(),
		m_denomination(denomination)
	{}

	//////////////////////////////////// DBO ////////////////////////////////////
	// STI : None. This class is not meant to be persisted, only its children.
#ifdef USE_WT
	template<class Action>
	void persist_virtual(Action& a)
	{
		PersistableObject::persist_virtual(a);

		// set the denomination as primary key
		// TODO redesign for allowing composite keys !
		Wt::Dbo::id(a, m_denomination, "denomination", NamedPersistableObject::MAX_DENOMINATION_SIZE);
	}
#endif // USE_WT
	//////////////////////////////////// DBO ////////////////////////////////////

	// above all, this object has a denomination
	virtual const std::string get_denomination() const { return m_denomination; }
	virtual const std::string get_name() const { return get_denomination(); }

	// since the denomination is most likely used as a key,
	// we need a special method to rename such an object
	// (we may need to alter related objects)
	virtual void rename(const std::string& new_denomination) { throw "Not implemented"; }


protected:
	std::string m_denomination;
};

#ifdef USE_WT
// http://www.webtoolkit.eu/wt/doc/tutorial/dbo.html "Specifying a natural primary key"
#define WT_DBO_NAMED_OBJECT_CHILD_TEMPLATE_SPECIALIZATION(class_name) \
	class class_name;                \
	namespace Wt {                \
		namespace Dbo {                \
			template<>                \
			struct dbo_traits< class_name > : public dbo_default_traits {                \
				typedef std::string IdType;                \
				static IdType invalidId() { return std::string(); }                \
				static const char *surrogateIdField() { return 0; }                \
			};                \
		}                \
	}
#endif // USE_WT


#endif /* NAMED_PERSISTABLE_OBJECT_HPP_ */
