/**
 *
 *
#include "HTML_widgets.hpp"
 */

#ifndef __HTMLWIDGETS_HPP_INCLUDED__
#define __HTMLWIDGETS_HPP_INCLUDED__


#include <Wt/WContainerWidget>

//class Wt::WWidget;


/* base class for defining HTML (5) tags
 */
class WHTMLContainerWidget : public Wt::WContainerWidget
{
public:
	WHTMLContainerWidget(Wt::DomElementType p_elmType, Wt::WContainerWidget *p_parent = NULL):
		Wt::WContainerWidget(p_parent), m_elmType(p_elmType) { }
	
	virtual WHTMLContainerWidget *addChildF(Wt::WWidget *p_widget) { addWidget(p_widget); return this; }
	virtual WHTMLContainerWidget *addChildF(Wt::WWidget *p_widget1, Wt::WWidget *p_widget2) { addWidget(p_widget1); addWidget(p_widget2); return this; }
	
protected:
	virtual Wt::DomElementType domElementType() const { return m_elmType; }
	Wt::DomElementType m_elmType;
};

/* base class for defining ?
 */
class WUnknownHTMLContainerWidget : public Wt::WContainerWidget
{
public:
	WUnknownHTMLContainerWidget(const char *p_type, Wt::WContainerWidget *p_parent = NULL):
		Wt::WContainerWidget(p_parent) { this->addStyleClass(p_type); }
	
	virtual WUnknownHTMLContainerWidget *addStyleClassF(const Wt::WString & styleClass, bool force = false)
	{ addStyleClass( styleClass, force); return this; }

	virtual WUnknownHTMLContainerWidget *addChildF(Wt::WWidget *p_widget1)
	{ addWidget(p_widget1); return this; }
	virtual WUnknownHTMLContainerWidget *addChildF(Wt::WWidget *p_widget1, Wt::WWidget *p_widget2)
	{ addWidget(p_widget1); addChildF(p_widget2); return this; }
	virtual WUnknownHTMLContainerWidget *addChildF(Wt::WWidget *p_widget1, Wt::WWidget *p_widget2, Wt::WWidget *p_widget3)
	{ addWidget(p_widget1); addChildF(p_widget2, p_widget3); return this; }
	virtual WUnknownHTMLContainerWidget *addChildF(Wt::WWidget *p_widget1, Wt::WWidget *p_widget2, Wt::WWidget *p_widget3, Wt::WWidget *p_widget4)
	{ addWidget(p_widget1); addChildF(p_widget2, p_widget3, p_widget4); return this; }
	virtual WUnknownHTMLContainerWidget *addChildF(Wt::WWidget *p_widget1, Wt::WWidget *p_widget2, Wt::WWidget *p_widget3, Wt::WWidget *p_widget4, Wt::WWidget *p_widget5)
	{ addWidget(p_widget1); addChildF(p_widget2, p_widget3, p_widget4, p_widget5); return this; }
};


class WH1 : public WHTMLContainerWidget
{ public: WH1(Wt::WContainerWidget *p_parent = NULL):WHTMLContainerWidget(Wt::DomElement_H1, p_parent) { } };

class WH2 : public WHTMLContainerWidget
{ public: WH2(Wt::WContainerWidget *p_parent = NULL):WHTMLContainerWidget(Wt::DomElement_H2, p_parent) { } };

class WP : public WHTMLContainerWidget
{ public:  WP(Wt::WContainerWidget *p_parent = NULL):WHTMLContainerWidget(Wt::DomElement_P,  p_parent) { } };

class WArticle : public WUnknownHTMLContainerWidget
{ public: WArticle(Wt::WContainerWidget *p_parent = NULL):WUnknownHTMLContainerWidget("article", p_parent) { } };

class WHeader : public WUnknownHTMLContainerWidget
{ public: WHeader(Wt::WContainerWidget *p_parent = NULL):WUnknownHTMLContainerWidget("header", p_parent) { } };

class WHGroup : public WUnknownHTMLContainerWidget
{ public: WHGroup(Wt::WContainerWidget *p_parent = NULL):WUnknownHTMLContainerWidget("hgroup", p_parent) { } };

class WFooter : public WUnknownHTMLContainerWidget
{ public: WFooter(Wt::WContainerWidget *p_parent = NULL):WUnknownHTMLContainerWidget("footer", p_parent) { } };

class WSection : public WUnknownHTMLContainerWidget
{ public: WSection(Wt::WContainerWidget *p_parent = NULL):WUnknownHTMLContainerWidget("section", p_parent) { } };

class WNav : public WUnknownHTMLContainerWidget
{ public: WNav(Wt::WContainerWidget *p_parent = NULL):WUnknownHTMLContainerWidget("nav", p_parent) { } };

class WUl : public WUnknownHTMLContainerWidget
{ public: WUl(Wt::WContainerWidget *p_parent = NULL):WUnknownHTMLContainerWidget("ul", p_parent) { } };


#endif // __HTMLWIDGETS_HPP_INCLUDED__
