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

	void register_sub_account(const DistributedSubAccount& dsa);
	void register_deal(const Deal& d);

	MoneyAmount theoretical_total_available(); // TODO make it const

	MoneyAmount theoretical_balance_of_sub_account(const DistributedSubAccount& dsa);

protected:

	void update_results_if_needed();
	void recompute_all();
	void reset_results(); // TODO use C++11 const don't care
	void process_all_deals();

	DistributedAccount m_account;
	std::vector<DistributedSubAccount> m_sub_accounts;
	std::vector<Deal> m_deals;

	bool m_need_compute;

	boost::posix_time::ptime m_value_date; // date for which we must compute results

	// results (cache)
	MoneyAmount m_theoretical_total_available;
	MoneyAmount m_settled_total_available;

	template<class T>
	DistributedAccountReportInternalIDType get_internal_id_for(const T& elem) const;

	std::map<DistributedAccountReportInternalIDType, MoneyAmount> m_theoretical_sub_totals;

	void check_if_sub_account_is_known(const DistributedSubAccount& dsa) const;
};

#endif /* DISTRIBUTEDACCOUNTREPORT_HPP_ */
