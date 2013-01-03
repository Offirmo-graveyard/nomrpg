#include "BoringRPGCentral.hpp"

#include <boost/algorithm/string.hpp>

#include <Wt/Dbo/Dbo>

#include "TBRPGEventRepo.hpp"

#include "AppSessionUtils.hpp"




BoringRPGCentral::BoringRPGCentral(boost::shared_ptr<AppSession> app_session):
	Wt::WObject(),
	m_app_session(app_session)
{
	m_shared_this.reset(this);

	// attempt to load existing model for current user
	m_model = find_unique_virtual_world_element<TBRPGGame>(app_session);

	if(!m_model)
	{
		// none found (with no errors)
		// -> attempt to create one
		Wt::Dbo::Session& dbo_session = *(app_session->main_dbo_session());
		Wt::Dbo::Transaction transaction(dbo_session);
		TBRPGGame* p = new TBRPGGame(*(app_session->avatar()));
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

boost::shared_ptr<BoringRPGPlayWidget> BoringRPGCentral::get_play_widget()
{
	if (!m_play_wgt)
	{
		m_play_wgt.reset(new BoringRPGPlayWidget(m_app_session, m_shared_this));
	}

	return m_play_wgt;
}
boost::shared_ptr<BoringRPGStatsWidget> BoringRPGCentral::get_stats_widget()
{
	if (!m_stats_wgt)
	{
		m_stats_wgt.reset(new BoringRPGStatsWidget(m_app_session, m_shared_this));
	}

	return m_stats_wgt;
}


void BoringRPGCentral::play()
{
	bool updated_ok = false;

	try
	{
		Wt::Dbo::Transaction transaction(*(m_app_session->main_dbo_session().get()));
		m_model.modify()->click();
		m_model.flush(); // write to DB
		transaction.commit();
		update();
		updated_ok = true;
	}
	catch(std::exception& e)
	{
		std::cout << std::endl << "XXX " << e.what() << " XXX" << std::endl << std::endl;
	}
	catch(...)
	{
		std::cout << std::endl << "XXX Unknown exception ! XXX" << std::endl << std::endl;
	}

	if(!updated_ok)
	{
		// there was an exception
		get_play_widget()->signal_internal_error();
	}
}


void BoringRPGCentral::update()
{
	get_play_widget()->update();
	get_stats_widget()->update();
}
