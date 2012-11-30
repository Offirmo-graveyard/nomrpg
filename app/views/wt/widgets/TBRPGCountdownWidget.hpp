/*
 * Strongly inspired from Wt exemple countdown : examples/mission/CountDownWidget
 * Copyright (C) 2008 Emweb bvba, Kessel-Lo, Belgium.
 *
#include "TBRPGCountdownWidget.hpp"
 */
#ifndef TBRPGCOUNTDOWNWIDGET_HPP__
#define TBRPGCOUNTDOWNWIDGET_HPP__


#include <Wt/WTemplate>
#include <Wt/WText>

namespace Wt {
  class WTimer;
}


/* This is not a generic countdown like the Emweb one.
 * It only handle seconds, counting back to 0.
 */
class TBRPGCountdownWidget : public Wt::WTemplate
{
public:
	// starts in stopped mode
	TBRPGCountdownWidget(Wt::WContainerWidget *parent_wgt = NULL, std::string i18n_radix = "");
	virtual ~TBRPGCountdownWidget();

	void reset(int new_delay_in_s);

	// for unit testing
	Wt::WString main_text() const { return m_countdown_txt.text(); }

protected:
	void update_display();
	// sub-function of the former
	void update_countdown_display();
	void add_separator(Wt::WString& delay_string, int num_parts, int displayed_parts);

	std::string m_i18n_radix;

	int m_start;
	int m_count;

	Wt::WText m_countdown_txt;
	//Wt::WText m_countdown_progress_indicator;

	Wt::WTimer* m_timer;

	/*! \brief Process one timer tick.
	 */
	void timer_tick();
};

#endif // TBRPGCOUNTDOWNWIDGET_HPP__
