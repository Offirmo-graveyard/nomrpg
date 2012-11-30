/*
#include "BoringRPGWidget.hpp"
 */

#ifndef BORING_RPG_WIDGET_HPP_INCLUDED__
#define BORING_RPG_WIDGET_HPP_INCLUDED__


#include <Wt/WTemplate>
#include <Wt/WPushButton>
#include <Wt/WText>

#include "TBRPGCountdownWidget.hpp"

class OffirmoWebApp;

// the model
#include "TBRPGGame.hpp"

/*
 * Navigator Online Role Playing Game
 */
class BoringRPGWidget : public Wt::WTemplate
{
public:
	static const std::string scm_i18n_radix;
	static const std::string scm_i18n_default_sub_radix;

	BoringRPGWidget(OffirmoWebApp& parentApp, std::string sub_radix = scm_i18n_default_sub_radix);

protected:

	void load_or_create_model();

	OffirmoWebApp& m_parentApp;
	Wt::Dbo::ptr<TBRPGGame> m_model;

	std::string m_i18n_radix;

	Wt::WPushButton m_play_btn;
	Wt::WText m_result_txt;
	TBRPGCountdownWidget m_countdown_wgt;
	Wt::WText m_click_count_caption;
	Wt::WText m_click_count_value;

	void play();

	void update_controls();

};


#endif // BORING_RPG_WIDGET_HPP_INCLUDED__
