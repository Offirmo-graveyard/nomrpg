#include "DistributedAccountReport.hpp"

#include "Mok/Exception.hpp"


DistributedAccountReport::DistributedAccountReport():
	m_need_compute(false)
{
}

DistributedAccountReport::DistributedAccountReport(const DistributedAccount& da):
	m_account(da),
	m_need_compute(false)
{
}

DistributedAccountReport::~DistributedAccountReport()
{
}


void DistributedAccountReport::register_sub_account(const DistributedSubAccount& dsa)
{
	m_need_compute = true;
	m_sub_accounts.push_back(dsa);
}
void DistributedAccountReport::register_deal(const Deal& d)
{
	m_need_compute = true;
	m_deals.push_back(d);
}


void DistributedAccountReport::update_results_if_needed()
{
	if(m_need_compute)
	{
		recompute_all();
	}
}


void DistributedAccountReport::recompute_all()
{
	reset_results();

	process_all_deals();

	m_need_compute = false;
}


void DistributedAccountReport::reset_results()
{
	m_theoretical_total_available = m_settled_total_available = MoneyAmount(0, m_account.get_currency());
	m_theoretical_sub_totals.clear();
}

/*
void DistributedAccountReport::intelligently_increase_totals(const MoneyAmount& amount, enum SettlementStatus settl_status)
{
	m_theoretical_total_available += amount;
	if(settl_status == SETTLED)
		m_settled_total_available += amount;
}*/


template<>
DistributedAccountReportInternalIDType DistributedAccountReport::get_internal_id_for(const DistributedSubAccount& dsa) const
{
	return dsa.get_name();
}


void DistributedAccountReport::process_all_deals()
{
	for(std::vector<Deal>::iterator it = m_deals.begin(); it != m_deals.end(); ++it)
	{
		const Deal* deal = &(*it);

		if(deal->get_currency() != m_account.get_currency())
		{
			// we need to convert the currency and this is not trivial at all !
			throw Mok::NotImplementedException("deal currency conversion");
		}

		// date ?
		enum SettlementStatus settlement_status_for_value_date = NOT_SETTLED; // TODO

		std::cout << "theoretical_total_available before = " << m_theoretical_total_available << std::endl;
		switch(deal->get_direction())
		{
		case Deal::INPUT:
			// yay ! more money for us !
//			intelligently_increase_totals(deal->get_amount(), settlement_status_for_value_date);
			m_theoretical_total_available += deal->get_amount();
			m_theoretical_sub_totals[get_internal_id_for(deal->get_destination())] += deal->get_amount();
			if(settlement_status_for_value_date == SETTLED)
			{
				m_settled_total_available += deal->get_amount();
			}
			// and less money for the source
			m_theoretical_sub_totals[get_internal_id_for(deal->get_source())] -= deal->get_amount();
			break;
		case Deal::OUTPUT:
			// uh oh... money is leaving !
			m_theoretical_total_available -= deal->get_amount();
			if(settlement_status_for_value_date == SETTLED)
				m_settled_total_available -= deal->get_amount();
			break;
		case Deal::INTERNAL:
			// nothing gained, nothing lost
			break;
		default:
			throw Mok::UnknownEnumValueException("Deal::Direction");
			break;
		}
		std::cout << "theoretical_total_available after = " << m_theoretical_total_available << std::endl;
	}
}


MoneyAmount DistributedAccountReport::theoretical_total_available()
{
	update_results_if_needed();

	return m_theoretical_total_available;
}


void DistributedAccountReport::check_if_sub_account_is_known(const DistributedSubAccount& dsa) const
{
	// TODO
}


MoneyAmount DistributedAccountReport::theoretical_balance_of_sub_account(const DistributedSubAccount& dsa)
{
	check_if_sub_account_is_known(dsa);

	update_results_if_needed();

	return m_theoretical_sub_totals[get_internal_id_for(dsa)];
}
