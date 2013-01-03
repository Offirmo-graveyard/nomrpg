/*
#include "BoringRPGBaseWidget.hpp"
 */

#ifndef BORINGRPGBASEWIDGET_HPP_
#define BORINGRPGBASEWIDGET_HPP_


#include "OffirmoTemplatedWidget.hpp"

#include <Wt/Dbo/Dbo>

class BoringRPGCentral;
class TBRPGGame;


class BoringRPGBaseWidget: public OffirmoTemplatedWidget
{
public:
	BoringRPGBaseWidget(boost::shared_ptr<AppSession> app_session,
	                    boost::shared_ptr<BoringRPGCentral> boring_rpg_central);

protected:
	virtual std::string get_template_radix() { return "TBRPG"; }
	boost::shared_ptr<BoringRPGCentral> m_boring_rpg_central;
	Wt::Dbo::ptr<TBRPGGame> m_model;
};

#endif /* BORINGRPGBASEWIDGET_HPP_ */
