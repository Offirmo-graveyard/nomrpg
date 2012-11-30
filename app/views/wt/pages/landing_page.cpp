#include "landing_page.hpp"

#include <Wt/WBreak>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WAnchor>

#include "OffirmoWebApp.hpp"


LandingPage::LandingPage(OffirmoWebApp& parentApp):
	GenericPage(parentApp)
{
	this->setId("landing-instructions");

	// init our members
	m_link_contact.setInternalPath( "/contact" );
	m_link_about  .setInternalPath( "/about"   );
	m_link_credits.setInternalPath( "/credits" );
	m_link_sources.setInternalPath( "/sources" );
	m_link_terms  .setInternalPath( "/terms"   );

	Wt::WTemplate* p_template = new Wt::WTemplate();

	// prepare for template
	p_template->bindWidget( "auth-wgt",     parentApp.getAuthWidget());
	p_template->bindWidget( "new-game-btn", new Wt::WPushButton(Wt::WString::tr("App.Landing.new_game")));
	p_template->bindWidget( "contact-lnk",  new Wt::WAnchor(m_link_contact, "contact"));
	p_template->bindWidget( "about-lnk",    new Wt::WAnchor(m_link_about,   "about"));
	p_template->bindWidget( "credits-lnk",  new Wt::WAnchor(m_link_credits, "credits"));
	p_template->bindWidget( "sources-lnk",  new Wt::WAnchor(m_link_sources, "sources"));
	p_template->bindWidget( "terms-lnk",    new Wt::WAnchor(m_link_terms,   "terms"));

	// use the template
	p_template->setTemplateText(tr("App.landing_page"));
	p_template->addFunction("tr", &Wt::WTemplate::Functions::tr);
	this->addWidget(p_template);
}

void LandingPage::attemptLogin()
{
	/*
	 * Update the text, using text input into the nameEdit_ field.
	 */
	m_result->setText("Sadly, nothing happened.");
	//m_parentApp->loginOK();
}
