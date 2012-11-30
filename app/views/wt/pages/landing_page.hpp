#ifndef LANDING_PAGE_HPP_INCLUDED__
#define LANDING_PAGE_HPP_INCLUDED__


#include <Wt/WLink>

#include "generic_page.hpp"

class WLineEdit;
class WText;
class NOMRPGApplication;


class LandingPage : public GenericPage
{
public:
	LandingPage(OffirmoWebApp& parentApp);
	~LandingPage() {}

protected:
	Wt::WLineEdit *m_emailField;
	Wt::WLineEdit *m_pwdField;
	Wt::WText *m_result;

	Wt::WLink m_link_contact;
	Wt::WLink m_link_about;
	Wt::WLink m_link_credits;
	Wt::WLink m_link_sources;
	Wt::WLink m_link_terms;

	void attemptLogin();
};


#endif // LANDING_PAGE_HPP_INCLUDED__
