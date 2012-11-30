/*
#include "LangDisplayAndSelectionWidget.hpp"
 */

#ifndef LANGDISPLAYANDSELECTIONWIDGET_HPP_
#define LANGDISPLAYANDSELECTIONWIDGET_HPP_

#include <Wt/WTemplate>

class OffirmoWebApp;

class LangDisplayAndSelectionWidget : public Wt::WTemplate
{
public:

	LangDisplayAndSelectionWidget(OffirmoWebApp& parentApp);

	void init();

protected:

	OffirmoWebApp& m_parentApp;

	std::string m_lang_code;
};

#endif /* LANGDISPLAYANDSELECTIONWIDGET_HPP_ */
