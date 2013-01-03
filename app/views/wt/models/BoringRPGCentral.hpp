/*
#include "BoringRPGCentral.hpp"
 */

#ifndef BORINGRPGCENTRAL_HPP_
#define BORINGRPGCENTRAL_HPP_

#include "Wt/WObject"
#include "TBRPGGame.hpp"

class AppSession;

#include "BoringRPGPlayWidget.hpp"
#include "BoringRPGStatsWidget.hpp"

// inheriting WObject just to be able to use Wt signals
class BoringRPGCentral : public Wt::WObject
{
public:
	BoringRPGCentral(boost::shared_ptr<AppSession> app_session);

	Wt::Dbo::ptr<TBRPGGame> get_model() { return m_model; }

	void play();
	void update();

	boost::shared_ptr<BoringRPGPlayWidget>  get_play_widget();
	boost::shared_ptr<BoringRPGStatsWidget> get_stats_widget();

protected:
	Wt::Dbo::ptr<TBRPGGame> m_model;
	boost::shared_ptr<AppSession> m_app_session;
	boost::shared_ptr<BoringRPGCentral> m_shared_this;
	boost::shared_ptr<BoringRPGPlayWidget> m_play_wgt;
	boost::shared_ptr<BoringRPGStatsWidget> m_stats_wgt;
};

#endif /* BORINGRPGCENTRAL_HPP_ */
