#ifndef _GSB_BANK_HH_
#define _GSB_BANK_HH_

#include "accounts.hh"

class Bank {
protected:
	std::string _name;
	std::vector<Account *> accounts;

	id_type _id;
	AccountSettings checking;
	AccountSettings saving;
	AccountSettings currency;
public:
	void update_monthly() {
		for (auto acc : accounts)
			acc->update_monthly();
	}

	//Bank() : _name(), accounts(), checking(), saving(), currency() {}
	Bank(std::string name, id_type id,
		AccountSettings checking, AccountSettings saving, AccountSettings currency) :
		_name(name), _id(id), 
		checking(checking), saving(saving), currency(currency) {}

	//TODO dopisać klasę currency
	CheckingAccount openCheckingAccount(const Being &who) {
		accounts.push_back(new CheckingAccount(accounts.size(), _id));
	} 
	
	SavingAccount openSavingAccount(const Being &who) {
		accounts.push_back(new SavingAccount(accounts.size(), _id));
	}

	CurrencyAccount openCurrencyAccount(const Being &who) {
		accounts.push_back(new CurrencyAccount(accounts.size(), _id));
	}

	~Bank() {
		for (auto acc : accounts)
			delete acc;
	}
};


#endif /* ! _GSB_BANK_HH_ */
