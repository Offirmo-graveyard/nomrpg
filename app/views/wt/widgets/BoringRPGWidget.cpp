#include "BoringRPGWidget.hpp"

#include <string>
#include <sstream>

#include <boost/algorithm/string.hpp>



#include <Wt/WBreak>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>

#include "OffirmoWebApp.hpp"

#include "AppSessionUtils.hpp"

#include "TBRPGEventRepo.hpp"


const std::string BoringRPGWidget::scm_i18n_radix = "TBRPG.";
const std::string BoringRPGWidget::scm_i18n_default_sub_radix = "default";


BoringRPGWidget::BoringRPGWidget(OffirmoWebApp& parentApp, std::string sub_radix):
	m_parentApp(parentApp),
	m_i18n_radix(scm_i18n_radix + sub_radix + "."),
	m_play_btn(Wt::WString::tr(m_i18n_radix + "click_btn_txt")),
	m_countdown_wgt(NULL, m_i18n_radix),
	m_click_count_caption(Wt::WString::tr(m_i18n_radix + "number_of_good_clicks_caption"))
{
	load_or_create_model();

	update_controls();

	bindWidget( "result-txt",     &m_result_txt);
	bindWidget( "countdown-wgt",  &m_countdown_wgt);
	bindWidget( "play-btn",       &m_play_btn);

	bindWidget( "number-of-clicks-caption-txt",  &m_click_count_caption);
	bindWidget( "number-of-clicks-value-txt",    &m_click_count_value);

	// Connect signals with slots
	// simple Wt-way
	m_play_btn.clicked().connect(this, &BoringRPGWidget::play);

	// use the template
	setTemplateText(tr(m_i18n_radix + "play_area"));

}


void BoringRPGWidget::load_or_create_model()
{
	// attempt to load existing model for current user
	m_model = find_unique_virtual_world_element<TBRPGGame>(m_parentApp.get_app_session());

	if(!m_model)
	{
		// none found (with no errors)
		// -> attempt to create one
		Wt::Dbo::Session& dbo_session = *(m_parentApp.get_app_session()->main_dbo_session());
		Wt::Dbo::Transaction transaction(dbo_session);
		TBRPGGame* p = new TBRPGGame(*(m_parentApp.get_app_session()->avatar()));
		m_model = dbo_session.add(p);
		transaction.commit();
		p = NULL;
	}

	if(m_model)
	{
		// fill the event repo
		TBRPGEventRepo event_repo;

		std::string radix = "TBRPG.default.events.";
		std::string events_list_as_comma_separated_string = Wt::WString::tr(radix + "list").narrow();

		if (events_list_as_comma_separated_string.compare( 0, 2, "??" ) == 0)
		{
			// problem : Wt i18n error
		}
		else
		{
			// split them
			// hat tip http://stackoverflow.com/a/236976/587407
			std::vector<std::string> events_ids;
			boost::split(events_ids, events_list_as_comma_separated_string, boost::is_any_of(","));
			for(std::vector<std::string>::const_iterator it = events_ids.begin(); it != events_ids.end(); ++it)
			{
				TBRPGEvent evt(*it);
				event_repo.register_event(evt);
			}

			m_model.modify()->register_possible_events(event_repo);
		}
	}
}


void BoringRPGWidget::play()
{
	bool updated = false;

	try
	{
		dbo::Transaction transaction(*(m_parentApp.get_app_session()->main_dbo_session().get()));
		m_model.modify()->click();
		m_model.flush(); // write to DB
		transaction.commit();
		update_controls();
		updated = true;
	}
	catch(std::exception& e)
	{
		std::cout << std::endl << "XXX " << e.what() << " XXX" << std::endl << std::endl;
	}
	catch(...)
	{
		std::cout << std::endl << "XXX Unknown exception ! XXX" << std::endl << std::endl;
	}

	if(!updated)
	{
		m_result_txt.setText(Wt::WString::tr(m_i18n_radix + "results.error"));
	}
}


void BoringRPGWidget::update_controls()
{
	bool ok = m_model->was_last_click_OK();
	Wt::WString response;
	int delay = 100000; // bad value

	if(ok)
	{
		TBRPGEventRealization event(m_model->get_last_correct_click_event());
		if (event.get_id().empty())
		{
			// no last event -> means we were unpersisted
			response = "";
		}
		else
		{
			response = Wt::WString::tr(m_i18n_radix + "events." + event.get_id());
		}
	}
	else
	{
		response = Wt::WString::tr(m_i18n_radix + "results.bad_click").arg(m_model->get_actual_punishment_delay_s());
	}
	delay = m_model->get_actual_click_delay_s();

	std::ostringstream out;
	out << "ok : " << ok << ", delay = " << delay;

	m_countdown_wgt.reset(delay);

	m_result_txt.setText(response);
	m_click_count_value.setText(boost::lexical_cast<std::string>(m_model->get_total_good_click_count()));
}

