/*
#include "generic_page.hpp"
 */

#ifndef GENERICPAGE_HPP_
#define GENERICPAGE_HPP_


#include <Wt/WContainerWidget>

class OffirmoWebApp;

/*
 * Generic page.
 * Base class with common routines.
 */
class GenericPage : public Wt::WContainerWidget
{
public:
	GenericPage(OffirmoWebApp& parentApp):
		m_parentApp(parentApp)
	{ }

	virtual ~GenericPage() {} // this class is meant to be subclassed

protected:
	virtual void setHidden(bool hidden, const Wt::WAnimation& animation = Wt::WAnimation())
	{
		bool will_change(hidden == isVisible());

		if (will_change && !hidden)
			this->will_be_displayed_callback();
		this->Wt::WContainerWidget::setHidden(hidden, animation);
		if (will_change && hidden)
			this->is_no_longer_visible_callback();
	}

	virtual void is_no_longer_visible_callback() {}
	virtual void will_be_displayed_callback() {}

	OffirmoWebApp& m_parentApp;
};


#endif // GENERICPAGE_HPP_
