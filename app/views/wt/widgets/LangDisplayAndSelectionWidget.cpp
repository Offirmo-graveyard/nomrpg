#include "LangDisplayAndSelectionWidget.hpp"

#include "OffirmoWebApp.hpp"


LangDisplayAndSelectionWidget::LangDisplayAndSelectionWidget(OffirmoWebApp& parentApp):
	m_parentApp(parentApp)
{
	//bindWidget( "lang-code-txt",     &m_lang_code);

}

void LangDisplayAndSelectionWidget::init()
{
	// use the template
	setTemplateText(tr("App.lang_selector"));
}
