/*
#include "BoringRPGPlayWidget.hpp"
 */

#ifndef BORING_RPG_WIDGET_HPP_INCLUDED__
#define BORING_RPG_WIDGET_HPP_INCLUDED__


#include "BoringRPGBaseWidget.hpp"

#include <Wt/WPushButton>
#include <Wt/WText>

#include "TBRPGCountdownWidget.hpp"


/*
 * Navigator Online Role Playing Game
 */
class BoringRPGPlayWidget : public BoringRPGBaseWidget
{
public:
	BoringRPGPlayWidget(boost::shared_ptr<AppSession> app_session,
         boost::shared_ptr<BoringRPGCentral> boring_rpg_central);

	virtual void bind_components();
	virtual void update();
	virtual void signal_internal_error();

protected:

	virtual std::string get_template_name() { return "play_area"; }

	Wt::WPushButton m_play_btn;
	Wt::WText m_result_txt;
	TBRPGCountdownWidget m_countdown_wgt;
};


#endif // BORING_RPG_WIDGET_HPP_INCLUDED__
