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


template<>
DistributedAccountReportInternalIDType DistributedAccountReport::get_internal_id_for(const DistributedSubAccount& dsa) const
{
	return dsa.get_name();
}



/*
bool DistributedAccountReport::is_sub_account_registered(const DistributedSubAccount& dsa) const
{
	DistributedAccountReportInternalIDType key = get_internal_id_for(dsa);

	return m_sub_accounts.count( key ) != 0;
}
void DistributedAccountReport::register_sub_account(const DistributedSubAccount& dsa)
{
	if(is_sub_account_registered(dsa))
	{
		// account already registered, nothing to do
	}
	else
	{
		m_need_compute = true;
		m_sub_accounts.push_back(dsa);
	}
}*/

void DistributedAccountReport::register_deal(const Deal& d)
{
	m_need_compute = true;
	//register_sub_account(d.get_source());
	//register_sub_account(d.get_destination());
	m_deals.push_back(d);
}


void DistributedAccountReport::ensure_up_to_date_results()
{
	if(m_need_compute)
	{
		recompute_all();
	}
}


void DistributedAccountReport::recompute_all()
{
	std::cout << "* starting recompute all..." << std::endl;

	reset_results();

	process_all_deals();

	m_need_compute = false;
}


void DistributedAccountReport::reset_results()
{
	m_theoretical_total_available = m_settled_total_available = MoneyAmount(0, m_account.get_currency());
	m_theoretical_balances.clear();
	m_theoretical_inputs.clear();
	m_theoretical_outputs.clear();
}

/*
void DistributedAccountReport::intelligently_increase_totals(const MoneyAmount& amount, enum SettlementStatus settl_status)
{
	m_theoretical_total_available += amount;
	if(settl_status == SETTLED)
		m_settled_total_available += amount;
}*/


void DistributedAccountReport::process_all_deals()
{
	std::cout << "* processing deals..." << std::endl;
	for(std::vector<Deal>::iterator it = m_deals.begin(); it != m_deals.end(); ++it)
	{
		const Deal* deal = &(*it);

		std::cout << "  * deal..." << std::endl;
		std::cout << "    - from : " << deal->get_source().get_name() << std::endl;
		std::cout << "    - to   : " << deal->get_destination().get_name() << std::endl;
		std::cout << "    - of   : " << deal->get_amount().get_human_representation() << std::endl;

		if(deal->get_currency() != m_account.get_currency())
		{
			// we need to convert the currency and this is not trivial at all !
			throw Mok::NotImplementedException("deal currency conversion");
		}

		// date ?
		enum SettlementStatus settlement_status_for_value_date = NOT_SETTLED; // TODO

		DistributedAccountReportInternalIDType src_id = get_internal_id_for(deal->get_source());
		DistributedAccountReportInternalIDType dst_id = get_internal_id_for(deal->get_destination());

		std::cout << "theoretical_total_available before = " << m_theoretical_total_available << std::endl;
		switch(deal->get_direction())
		{
		case Deal::INPUT:
			std::cout << "    - type : INPUT" << std::endl;
			// yay ! more money for us !
//			intelligently_increase_totals(deal->get_amount(), settlement_status_for_value_date);
			m_theoretical_total_available += deal->get_amount();
			// register change in internal side
			m_theoretical_balances[dst_id] += deal->get_amount();
			// an conditionally register settled change
			if(settlement_status_for_value_date == SETTLED)
			{
				m_settled_total_available += deal->get_amount();
			}
			// and less money for the source
			// balance -> No record because source is external (input)
			// input -> yes, record
			m_theoretical_inputs[src_id] += deal->get_amount();
			break;
		case Deal::OUTPUT:
			std::cout << "    - type : OUTPUT" << std::endl;
			// uh oh... money is leaving !
			m_theoretical_total_available -= deal->get_amount();
			// register change in internal side
			m_theoretical_balances[src_id] -= deal->get_amount();
			// an conditionally register settled change
			if(settlement_status_for_value_date == SETTLED)
				m_settled_total_available -= deal->get_amount();
			// and more money for the destination
			// balance -> No record because dest is external (output)
			// output -> yes, record
			m_theoretical_outputs[dst_id] += deal->get_amount();
			break;
		case Deal::INTERNAL:
			std::cout << "    - type : INTERNAL" << std::endl;
			// nothing gained, nothing lost
			// register change in internal sides
			m_theoretical_balances[src_id] -= deal->get_amount();
			m_theoretical_balances[dst_id] += deal->get_amount();
			// an conditionally register settled change
			break;
		case Deal::UNKNOWN:
			// what ?
			throw std::invalid_argument("Deal is of unknown type");
			break;
		default:
			throw Mok::UnknownEnumValueException("Deal::Direction");
			break;
		}
		std::cout << "theoretical_total_available after = " << m_theoretical_total_available << std::endl;
		debug_balance(deal->get_source());
		debug_balance(deal->get_destination());
		debug_input(deal->get_source());
		debug_output(deal->get_destination());
	}
}


MoneyAmount DistributedAccountReport::theoretical_total_available()
{
	ensure_up_to_date_results();

	// If available answer is default constructed, it has no currency and must have one.
	m_theoretical_total_available.set_currency(m_account.get_currency());

	return m_theoretical_total_available;
}




MoneyAmount DistributedAccountReport::theoretical_balance_of_sub_account(const DistributedSubAccount& dsa)
{
	ensure_up_to_date_results();

	// If available answer is default constructed, it has no currency and must have one.
	m_theoretical_balances[get_internal_id_for(dsa)].set_currency(m_account.get_currency());

	MoneyAmount res = m_theoretical_balances[get_internal_id_for(dsa)];

	debug_balance(dsa);

	return res;
}

MoneyAmount DistributedAccountReport::theoretical_input_from_sub_account(const DistributedSubAccount& dsa)
{
	ensure_up_to_date_results();

	// If available answer is default constructed, it has no currency and must have one.
	m_theoretical_inputs[get_internal_id_for(dsa)].set_currency(m_account.get_currency());

	MoneyAmount res = m_theoretical_inputs[get_internal_id_for(dsa)];

	debug_input(dsa);

	return res;
}

MoneyAmount DistributedAccountReport::theoretical_output_to_sub_account(const DistributedSubAccount& dsa)
{
	ensure_up_to_date_results();

	// If available answer is default constructed, it has no currency and must have one.
	m_theoretical_outputs[get_internal_id_for(dsa)].set_currency(m_account.get_currency());

	MoneyAmount res = m_theoretical_outputs[get_internal_id_for(dsa)];

	debug_output(dsa);

	return res;
}

void DistributedAccountReport::debug_generic(const DistributedSubAccount& dsa, std::string descr, const MoneyAmount& amount)
{
	std::cout << descr
	          << "sub_account \"" << dsa.get_name() << "\""
	          << " is " << amount.get_human_representation()
	          << std::endl;

}

void DistributedAccountReport::debug_balance(const DistributedSubAccount& dsa)
{
	debug_generic(dsa, "theoretical_balance_of_", m_theoretical_balances[get_internal_id_for(dsa)]);
}
void DistributedAccountReport::debug_output(const DistributedSubAccount& dsa)
{
	debug_generic(dsa, "theoretical_output_to_", m_theoretical_outputs[get_internal_id_for(dsa)]);
}

void DistributedAccountReport::debug_input(const DistributedSubAccount& dsa)
{
	debug_generic(dsa, "theoretical_input_from_", m_theoretical_inputs[get_internal_id_for(dsa)]);
}

void DistributedAccountReport::print_recap()
{
	ensure_up_to_date_results();

	std::cout << " Date                | T |   Amount    | Comment | Eff | Source                   | Destination             |" << std::endl;

	for(std::vector<Deal>::iterator it = m_deals.begin(); it != m_deals.end(); ++it)
	{
		const Deal* deal = &(*it);

		std::cout << boost::posix_time::to_simple_string(deal->get_trade_date());
		std::cout << " | ";

		std::cout << deal->get_direction_as_string_symbol();
		std::cout << " | ";

		std::cout << std::setw(8) << std::right << deal->get_amount().get_human_representation_without_currency();
		std::cout << std::setw(3) << std::left << deal->get_amount().get_currency().get_code();
		std::cout << " | ";

		std::cout << std::setw(7) << "[TODO] ";
		std::cout << " | ";

		enum SettlementStatus settlement_status_for_value_date = NOT_SETTLED; // TODO
		std::cout << " ? ";
		std::cout << " | ";

		std::cout << std::setw(24) << deal->get_source().get_name();
		std::cout << " | ";
		std::cout << std::setw(24) << deal->get_destination().get_name();
		std::cout << " | ";

		std::cout << std::endl;
	}
}
