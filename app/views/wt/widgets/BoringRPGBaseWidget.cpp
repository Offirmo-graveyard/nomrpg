#include "BoringRPGBaseWidget.hpp"

#include "BoringRPGCentral.hpp"



BoringRPGBaseWidget::BoringRPGBaseWidget(boost::shared_ptr<AppSession> app_session,
                                         boost::shared_ptr<BoringRPGCentral> boring_rpg_central):
	OffirmoTemplatedWidget(app_session),
	m_boring_rpg_central(boring_rpg_central),
	m_model(m_boring_rpg_central->get_model())
{
	// nothing
}

