#ifndef STARGATE_PANEL_WIDGET_HPP_
#define STARGATE_PANEL_WIDGET_HPP_


#include <Wt/WContainerWidget>

class StargatePanelWidget : public Wt::WContainerWidget
{
public:
	StargatePanelWidget(Wt::WContainerWidget *p_parent = NULL):
		Wt::WContainerWidget(p_parent)
	{ };
	virtual ~StargatePanelWidget() {}
};

#endif /* STARGATE_PANEL_WIDGET_HPP_ */
