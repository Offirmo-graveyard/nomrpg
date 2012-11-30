#include "DistributedSubAccount.hpp"


const std::string DistributedSubAccount::cm_main_subaccount_name = "main";


DistributedSubAccount::DistributedSubAccount(const std::string& denomination):
	NamedPersistableObject(denomination)
	//m_relation_to_main_account(DistributedSubAccount::IS_EXTERNAL)
{
}
DistributedSubAccount::DistributedSubAccount(const DistributedAccount& da, const std::string& denomination): //, enum ToMainAccountRelation ma_rel):
			NamedPersistableObject(denomination),
			TiedToDistributedAccount(da)
			//m_relation_to_main_account(ma_rel)
{
}

DistributedSubAccount::~DistributedSubAccount()
{
}

