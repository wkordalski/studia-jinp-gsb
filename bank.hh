#ifndef _GSB_BANK_HH_
#define _GSB_BANK_HH_

#include "accounts.hh"

class Bank {
protected:
	std::string _name;
	AccountSettings checking;
	AccountSettings saving;
	AccountSettings currency;
public:
	Bank(std::string name, AccountSettings checking, AccountSettings saving, AccountSettings currency) :
		_name(name), checking(checking), saving(saving), currency(currency) {}

	//TODO dopisać klasę currency
	CheckingAccount openCheckingAccount(const Being &who) {

	} 
	
	SavingAccount openSavingAccount(const Being &who) {

	}

	CurrencyAccount openCurrencyAccount(const Being &who) {

	}
};


#endif /* ! _GSB_BANK_HH_ */
