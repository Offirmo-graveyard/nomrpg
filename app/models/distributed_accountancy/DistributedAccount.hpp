/*
#include "DistributedAccount.hpp"
 */

#ifndef DISTRIBUTED_ACCOUNT_HPP_
#define DISTRIBUTED_ACCOUNT_HPP_


// ancestor classes
#include "NamedPersistableObject.hpp"
#include "TiedToRealUser.hpp"

#include "MoneyAmount.hpp"
#include "DistributedSubAccount.hpp"

class DistributedAccount : public NamedPersistableObject, public TiedToRealUser
{
public:
	static const std::string cm_default_denomination;
	static const std::string cm_default_currency_code;

	DistributedAccount(const std::string& p_denomination = DistributedAccount::cm_default_denomination, const std::string& currency_code = DistributedAccount::cm_default_currency_code);
	DistributedAccount(const RealUser& ru, const std::string& p_denomination = DistributedAccount::cm_default_denomination, const std::string& currency_code = DistributedAccount::cm_default_currency_code);

	//////////////////////////////////// DBO ////////////////////////////////////
	// for STI (Single Type Inheritance). It MUST be equals to the class name.
	virtual const std::string get_class_name() const { return "DistributedAccount"; }

#ifdef USE_WT
	template<class Action>
	void persist(Action& a)
	{
		//Wt::Dbo::field(a,     m_currency,           "currency");
		NamedPersistableObject::persist_virtual(a);
	}
#endif // USE_WT
	//////////////////////////////////// DBO ////////////////////////////////////

	Currency get_currency() const { return m_currency; }

protected:
	Currency m_currency;
};

#endif /* DISTRIBUTED_ACCOUNT_HPP_ */
