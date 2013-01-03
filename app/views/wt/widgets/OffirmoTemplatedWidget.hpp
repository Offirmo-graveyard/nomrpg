/*
#include "OffirmoTemplatedWidget.hpp"
 */

#ifndef OFFIRMOTEMPLATEDWIDGET_HPP_
#define OFFIRMOTEMPLATEDWIDGET_HPP_


#include <Wt/WTemplate>

class AppSession;


class OffirmoTemplatedWidget: public Wt::WTemplate
{
public:
	OffirmoTemplatedWidget(boost::shared_ptr<AppSession> app_session);
	virtual ~OffirmoTemplatedWidget() {} // just for the sake of making it virtual

	virtual void init_from_last_child();

	// to be overloaded
	virtual void update() {}

protected:

	// to be overloaded
	virtual void load_or_create_model() {}
	virtual void bind_components() {}

	virtual std::string get_template_radix() { return "App"; }
	virtual std::string get_template_sub_radix() { return "default"; }
	virtual std::string get_template_base() { return get_template_radix() + "." + get_template_sub_radix() + "."; }
	virtual std::string get_template_name() { return "coming_soon"; }
	virtual std::string get_template_full_name() { return get_template_base() + get_template_name(); }

	boost::shared_ptr<AppSession> m_app_session;
};

#endif /* OFFIRMOTEMPLATEDWIDGET_HPP_ */
