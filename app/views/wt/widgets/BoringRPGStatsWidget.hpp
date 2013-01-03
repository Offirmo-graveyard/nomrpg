/*
#include "BoringRPGStatsWidget.hpp"
 */

#ifndef BORINGRPGSTATSWIDGET_HPP_
#define BORINGRPGSTATSWIDGET_HPP_


#include "BoringRPGBaseWidget.hpp"

#include <Wt/WText>



class BoringRPGStatsWidget: public BoringRPGBaseWidget
{
public:
	BoringRPGStatsWidget(boost::shared_ptr<AppSession> app_session,
	                     boost::shared_ptr<BoringRPGCentral> boring_rpg_central);

	virtual void bind_components();
	virtual void update();

protected:
	virtual std::string get_template_name() { return "stats_area"; }

	Wt::WText m_player_name;
	Wt::WText m_click_count_caption;
	Wt::WText m_click_count_value;
};

#endif /* BORINGRPGSTATSWIDGET_HPP_ */
