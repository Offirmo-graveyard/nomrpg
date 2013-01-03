#include "BoringRPGPlayWidget.hpp"

#include "BoringRPGCentral.hpp"


BoringRPGPlayWidget::BoringRPGPlayWidget(boost::shared_ptr<AppSession> app_session,
                                         boost::shared_ptr<BoringRPGCentral> boring_rpg_central):
	BoringRPGBaseWidget(app_session, boring_rpg_central),
	m_play_btn(Wt::WString::tr(get_template_base() + "click_btn_txt")),
	m_countdown_wgt(NULL, get_template_base())
{
	init_from_last_child();
}


void BoringRPGPlayWidget::bind_components()
{
	bindWidget( "result-txt",     &m_result_txt);
	bindWidget( "countdown-wgt",  &m_countdown_wgt);
	bindWidget( "play-btn",       &m_play_btn);

	// Connect signals with slots
	// simple Wt-way
	m_play_btn.clicked().connect(m_boring_rpg_central.get(), &BoringRPGCentral::play);
}


void BoringRPGPlayWidget::update()
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
			response = Wt::WString::tr(get_template_base() + "events." + event.get_id());
		}
	}
	else
	{
		response = Wt::WString::tr(get_template_base() + "results.bad_click").arg(m_model->get_actual_punishment_delay_s());
	}
	delay = m_model->get_actual_click_delay_s();

	std::ostringstream out;
	out << "ok : " << ok << ", delay = " << delay;

	m_countdown_wgt.reset(delay);

	m_result_txt.setText(response);
}

void BoringRPGPlayWidget::signal_internal_error()
{
	m_result_txt.setText(Wt::WString::tr(get_template_base() + "results.error"));
}
