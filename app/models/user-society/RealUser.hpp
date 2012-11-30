/*
#include "RealUser.hpp"
 */

#ifndef REAL_USER_HPP_
#define REAL_USER_HPP_


// ancestor class
#include "NaturalPerson.hpp"

#ifdef USE_WT
#include <Wt/Dbo/Dbo>
// specialization of some ancestor methods
WT_DBO_NAMED_OBJECT_CHILD_TEMPLATE_SPECIALIZATION(RealUser)

// typedefs
class RealUser;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<RealUser> > RealUsers;
#endif // USE_WT


/** A "real" user is an object representing a "user" (email + password)
 * of our systems.
 * Most frameworks already provide a "user" class we don't want to mess with,
 * so we create this "real user" class tied to a "framework generated user".
 */
class RealUser: public NaturalPerson
{
public:
	RealUser(const std::string& p_denomination = Person::cm_default_denomination);

	//////////////////////////////////// DBO ////////////////////////////////////
	// for STI (Single Type Inheritance). It MUST be equals to the class name.
	virtual const std::string get_class_name() const { return "RealUser"; }

#ifdef USE_WT
	//dbo::collection< dbo::ptr<VirtualWorldGeneralParticipation> > m_general_participations;

	template<class Action>
	void persist(Action& a)
	{
		//Wt::Dbo::field(a, m_email, "email");

		NaturalPerson::persist_virtual(a);
	}
#endif // USE_WT
	//////////////////////////////////// DBO ////////////////////////////////////

	//virtual bool set_email(std::string p_email) { m_email = p_email; return true; }
	//virtual const std::string& get_email() { return m_email; }

protected:
	//std::string m_email;
};


// TODO
// will need disconnect TODO
//void rename_real_user(Wt::Dbo::ptr<RealUser> p, const std::string& new_name);
//void delete_real_user(Wt::Dbo::ptr<RealUser> p);


#endif /* REAL_USER_HPP_ */
