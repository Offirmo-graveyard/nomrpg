#include "app_page.hpp"

#include <Wt/Auth/Identity>

#include "OffirmoWebApp.hpp"
#include "BoringRPGWidget.hpp"

AppPage::AppPage(OffirmoWebApp& parentApp):
	GenericPage(parentApp),
	m_stylesheet_registered(false)
{
	this->setId("app-page");
}

void AppPage::is_no_longer_visible_callback()
{
	m_parentApp.log("info") << "|~| App page has been hidden";
	this->clear (); // Removes and deletes all child widgets
}
void AppPage::will_be_displayed_callback()
{
	m_parentApp.log("info") << "|~| App page will be displayed";

	if(!m_stylesheet_registered)
	{
		m_stylesheet_registered = true;
	}

	Wt::Auth::AuthWidget* authWidget = m_parentApp.getAuthWidget();
	this->addWidget(authWidget);

	BoringRPGWidget* boring_widget = new BoringRPGWidget(m_parentApp);
	this->addWidget(boring_widget);
}
