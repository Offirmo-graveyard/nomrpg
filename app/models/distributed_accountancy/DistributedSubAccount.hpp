/*
#include "DistributedSubAccount.hpp"
 */

#ifndef DISTRIBUTED_SUB_ACCOUNT_HPP_
#define DISTRIBUTED_SUB_ACCOUNT_HPP_


// ancestor classes
#include "NamedPersistableObject.hpp"
#include "TiedToDistributedAccount.hpp"


class DistributedSubAccount: public NamedPersistableObject, public TiedToDistributedAccount
{
public:
	static const std::string cm_main_subaccount_name;

/*	enum ToMainAccountRelation
	{
		IS_INTERNAL,
		IS_EXTERNAL
	};*/
	DistributedSubAccount(const std::string& denomination = "");
	DistributedSubAccount(const DistributedAccount& da, const std::string& denomination = ""); //, enum ToMainAccountRelation ma_rel = DistributedSubAccount::IS_EXTERNAL);

	virtual ~DistributedSubAccount();


	//////////////////////////////////// DBO ////////////////////////////////////
	// for STI (Single Type Inheritance). It MUST be equals to the class name.
	virtual const std::string get_class_name() const { return "DistributedSubAccount"; }
	//////////////////////////////////// DBO ////////////////////////////////////

protected:
	//enum ToMainAccountRelation m_relation_to_main_account;
};

#endif /* DISTRIBUTED_SUB_ACCOUNT_HPP_ */
