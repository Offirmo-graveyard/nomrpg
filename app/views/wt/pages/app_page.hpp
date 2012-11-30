#ifndef APP_PAGE_HPP_
#define APP_PAGE_HPP_

#include "generic_page.hpp"

class AppPage: public GenericPage
{
public:
	AppPage(OffirmoWebApp& parentApp);
	virtual ~AppPage() {}

protected:

	// override of parent methods
	virtual void is_no_longer_visible_callback();
	virtual void will_be_displayed_callback();

	bool m_stylesheet_registered;
};

#endif /* APP_PAGE_HPP_ */
