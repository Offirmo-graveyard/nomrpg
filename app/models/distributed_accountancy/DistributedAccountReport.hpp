/*
#include "DistributedAccountReport.hpp"
 */

#ifndef DISTRIBUTEDACCOUNTREPORT_HPP_
#define DISTRIBUTEDACCOUNTREPORT_HPP_


#include <vector>
#include <map>

#include "MoneyAmount.hpp"
#include "DistributedAccount.hpp"
#include "DistributedSubAccount.hpp"
#include "Deal.hpp"


/** the account itself doesn't do computations,
 * it only stores things.
 * Ths report actually do computations,
 * it contains temporary copies of data.
 */
typedef std::string DistributedAccountReportInternalIDType;

class DistributedAccountReport
{
public:
	enum SettlementStatus
	{
		SETTLED,
		NOT_SETTLED
	};
	DistributedAccountReport();
	DistributedAccountReport(const DistributedAccount& da);
	virtual ~DistributedAccountReport();

	// note : source and dest of this deal
	// will be automatically registered as sub-accounts
	void register_deal(const Deal& d);

	// usually not needed
	// since deal registration automatically register sub accounts
	//void register_sub_account(const DistributedSubAccount& dsa);
	//bool is_sub_account_registered(const DistributedSubAccount& dsa) const;

	MoneyAmount theoretical_total_available(); // TODO make it const

	MoneyAmount theoretical_balance_of_sub_account(const DistributedSubAccount& dsa);
	MoneyAmount theoretical_input_from_sub_account(const DistributedSubAccount& dsa);
	MoneyAmount theoretical_output_to_sub_account(const DistributedSubAccount& dsa);

	void print_recap();

protected:

	void ensure_up_to_date_results();
	void recompute_all();
	void reset_results(); // TODO use C++11 const don't care
	void process_all_deals();

	DistributedAccount m_account;
	//std::vector<DistributedSubAccount> m_sub_accounts;
	std::vector<Deal> m_deals;

	bool m_need_compute;

	boost::posix_time::ptime m_value_date; // date for which we must compute results

	// results (cache)
	MoneyAmount m_theoretical_total_available;
	MoneyAmount m_settled_total_available;

	template<class T>
	DistributedAccountReportInternalIDType get_internal_id_for(const T& elem) const;

	std::map<DistributedAccountReportInternalIDType, MoneyAmount> m_theoretical_balances;
	std::map<DistributedAccountReportInternalIDType, MoneyAmount> m_theoretical_inputs;
	std::map<DistributedAccountReportInternalIDType, MoneyAmount> m_theoretical_outputs;

	void debug_generic(const DistributedSubAccount& dsa, std::string descr, const MoneyAmount& amount);
	void debug_balance(const DistributedSubAccount& dsa);
	void debug_output(const DistributedSubAccount& dsa);
	void debug_input(const DistributedSubAccount& dsa);
};

#endif /* DISTRIBUTEDACCOUNTREPORT_HPP_ */
