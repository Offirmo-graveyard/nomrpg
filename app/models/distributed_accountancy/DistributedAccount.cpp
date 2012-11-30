#include "DistributedAccount.hpp"

const std::string DistributedAccount::cm_default_denomination = "Anonymous account";
const std::string DistributedAccount::cm_default_currency_code = "EUR";

DistributedAccount::DistributedAccount(const std::string& p_denomination, const std::string& currency_code):
	NamedPersistableObject(p_denomination),
	m_currency(currency_code)
{

}
DistributedAccount::DistributedAccount(const RealUser& ru, const std::string& p_denomination, const std::string& currency_code):
	NamedPersistableObject(p_denomination),
	TiedToRealUser(ru),
	m_currency(currency_code)
{

}
