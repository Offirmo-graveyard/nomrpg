/*
 * Strongly inspired from Wt exemple countdown : examples/mission/CountDownWidget
 * Copyright (C) 2008 Emweb bvba, Kessel-Lo, Belgium.
 */
#include "TBRPGCountdownWidget.hpp"

#include <boost/lexical_cast.hpp>

#include <Wt/WTimer>


TBRPGCountdownWidget::TBRPGCountdownWidget(Wt::WContainerWidget *parent_wgt, std::string i18n_radix):
//	WContainerWidget(parent_wgt),
	m_i18n_radix(i18n_radix),
	m_start(0),
	m_count(0),
	//m_countdown_txt(this),
	//	m_countdown_max_progress;
	//m_countdown_current_progress("0"),
	m_timer(NULL)
{
	//Wt::WString temp("0");
	//m_countdown_current_progress.setText(temp);

	//m_countdown_progress_indicator.setTextFormat(Wt::XHTMLText);

	bindWidget( "countdown-instructions",         &m_countdown_txt);

	// funny but not good for the user
	//bindWidget( "countdown-progress-indicator",   &m_countdown_progress_indicator);

	// use the template
	setTemplateText(tr(m_i18n_radix + "countdown"));
}

TBRPGCountdownWidget::~TBRPGCountdownWidget()
{
	if (m_timer)
		m_timer->stop(); // safer
	delete m_timer;
	m_timer = NULL; // good practice
}

void TBRPGCountdownWidget::update_display()
{
	if(m_count)
	{
		update_countdown_display();
	}
	else
	{
		m_countdown_txt.setText( Wt::WString::tr(m_i18n_radix + "instructions.ready") );
	}
	//m_countdown_progress_indicator.setText("<progress max=\"" + boost::lexical_cast<std::string>(m_start) + "\" value =\"" + boost::lexical_cast<std::string>(m_start-m_count) + "\" />");
}

static const int sc_num_of_seconds_in_a_minute = 60;
static const int sc_num_of_minutes_in_an_hour = 60;
static const int sc_num_of_seconds_in_an_hour = sc_num_of_minutes_in_an_hour * sc_num_of_seconds_in_a_minute;
static const int sc_num_of_hours_in_an_day = 24;
static const int sc_num_of_seconds_in_an_day = sc_num_of_seconds_in_an_hour * sc_num_of_hours_in_an_day;

// note : this is NOT very well internationalizable
// javascript version will be better (more powerful i18n lib)
void TBRPGCountdownWidget::add_separator(Wt::WString& delay_string, int num_parts, int displayed_parts)
{
	if (!displayed_parts)
	{
		// no separator of course, nothing to separate
	}
	else
	{
		int remaining_parts = num_parts - displayed_parts;
		switch(remaining_parts)
		{
		case 0:
			// nothing
			break;
		case 1:
			delay_string += " " + Wt::WString::tr(m_i18n_radix + "countdown.separator") + " ";
			break;
		default:
			delay_string += ", ";
			break;
		}
	}
}



void TBRPGCountdownWidget::update_countdown_display()
{
	// We make a copy :
	// 1) to be able to "consume" seconds into hours, minutes, etc.
	// 2) to prevent it from changing by a callback while we process it
	int seconds_not_displayed = m_count;

	// let's compute the sub-parts
	int num_days = seconds_not_displayed / sc_num_of_seconds_in_an_day;
	seconds_not_displayed = seconds_not_displayed % sc_num_of_seconds_in_an_day;
	int num_hours = seconds_not_displayed / sc_num_of_seconds_in_an_hour;
	seconds_not_displayed = seconds_not_displayed % sc_num_of_seconds_in_an_hour;
	int num_minutes = seconds_not_displayed / sc_num_of_seconds_in_a_minute;
	seconds_not_displayed = seconds_not_displayed % sc_num_of_seconds_in_a_minute;
	int num_seconds = seconds_not_displayed;

	// now let's build the string
	// note : this is NOT very well internationalizable
	// javascript version will be better (more powerful i18n lib)

	int num_parts = (num_days ? 1 : 0) + (num_hours ? 1 : 0) + (num_minutes ? 1 : 0) + (num_seconds ? 1 : 0);
	int displayed_parts = 0; // for now

	Wt::WString delay_string;

	std::string temp_key(m_i18n_radix + "countdown.days");

	if (num_days)
	{
		delay_string += Wt::WString::trn(temp_key, num_days).arg(num_days);
		displayed_parts++;
	}

	if (num_hours)
	{
		add_separator(delay_string, num_parts, displayed_parts);
		temp_key = m_i18n_radix + "countdown.hours";
		delay_string += Wt::WString::trn(temp_key, num_hours).arg(num_hours);
		displayed_parts++;
	}
	if (num_minutes)
	{
		add_separator(delay_string, num_parts, displayed_parts);
		temp_key = m_i18n_radix + "countdown.minutes";
		delay_string += Wt::WString::trn(temp_key, num_minutes).arg(num_minutes);
		displayed_parts++;
	}
	if (num_seconds)
	{
		add_separator(delay_string, num_parts, displayed_parts);
		temp_key = m_i18n_radix + "countdown.seconds";
		Wt::WString temp = Wt::WString::trn(temp_key, num_seconds).arg(num_seconds);
		delay_string += temp;
		displayed_parts++;
	}
	assert(displayed_parts == num_parts); // obviously

	temp_key = m_i18n_radix + "instructions.wait";

	m_countdown_txt.setText( Wt::WString::tr(temp_key).arg(delay_string) );
}

void TBRPGCountdownWidget::reset(int new_delay_in_s)
{
	if (m_timer)
	{
		m_timer->stop(); // just in case
		delete m_timer;
		m_timer = NULL; // good practice
	}

	m_count = m_start = std::max(new_delay_in_s, 0);
	update_display();

	m_timer = new Wt::WTimer(this);
	m_timer->setInterval(1000); // ms
	m_timer->timeout().connect(this, &TBRPGCountdownWidget::timer_tick);
	m_timer->start();
}

void TBRPGCountdownWidget::timer_tick()
{
	if (m_count)
	{
		--m_count;
	}
	else
	{
		m_timer->stop();
	}
	update_display();
}
