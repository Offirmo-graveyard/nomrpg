#include "TiedToDistributedAccount.hpp"

#include "DistributedAccount.hpp"


TiedToDistributedAccount::TiedToDistributedAccount(const std::string& da_id):
	m_distributed_account_id(da_id)
{
}
TiedToDistributedAccount::TiedToDistributedAccount(const DistributedAccount& da):
	m_distributed_account_id(da.get_name())
{
}

TiedToDistributedAccount::~TiedToDistributedAccount()
{
	// TODO Auto-generated destructor stub
}
