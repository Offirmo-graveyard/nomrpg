/*
#include "TiedToDistributedAccount.hpp"
 */

#ifndef TIED_TO_DISTRIBUTED_ACCOUNT_HPP_
#define TIED_TO_DISTRIBUTED_ACCOUNT_HPP_


#include <string>

class DistributedAccount;


class TiedToDistributedAccount
{
public:
	TiedToDistributedAccount(const std::string& da_id = "");
	TiedToDistributedAccount(const DistributedAccount& da);

	virtual ~TiedToDistributedAccount(); // just for declaring it virtual

	std::string get_distributed_account_name() const { return m_distributed_account_id; }

protected:
	std::string m_distributed_account_id;
};

#endif /* TIED_TO_DISTRIBUTED_ACCOUNT_HPP_ */
