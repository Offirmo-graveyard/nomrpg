#include "BoringRPGStatsWidget.hpp"

#include "TBRPGGame.hpp"

BoringRPGStatsWidget::BoringRPGStatsWidget(boost::shared_ptr<AppSession> app_session,
                                           boost::shared_ptr<BoringRPGCentral> boring_rpg_central):
	BoringRPGBaseWidget(app_session, boring_rpg_central),
	m_click_count_caption(Wt::WString::tr(get_template_base() + "number_of_good_clicks_caption"))
{
	init_from_last_child();
}


void BoringRPGStatsWidget::bind_components()
{
	bindWidget( "player-name-txt",               &m_player_name);
	bindWidget( "number-of-clicks-caption-txt",  &m_click_count_caption);
	bindWidget( "number-of-clicks-value-txt",    &m_click_count_value);
}


void BoringRPGStatsWidget::update()
{
	m_player_name.setText("(TODO)");
	m_click_count_value.setText(boost::lexical_cast<std::string>(m_model->get_total_good_click_count()));
}
