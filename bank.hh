#ifndef _GSB_BANK_HH_
#define _GSB_BANK_HH_

class AccountSettings {
	double monthly_charge = 0.;
	double transfer_charge = 0.;
	double interest_rate = 1.;
	
public:
	AccountSettings(double monthlyCharge, double transferCharge, double interestRate) :
		monthly_charge(monthlyCharge), transfer_charge(transferCharge), interest_rate(interestRate) {
	}
};

class Bank {
protected:
	std::string _name;
	AccountSettings checking;
	AccountSettings saving;
	AccountSettings currency;
public:
	Bank(std::string name, AccountSettings checking, AccountSettings saving, AccountSettings currency) :
		_name(name), checking(checking), saving(saving), currency(currency) {}
};


#endif /* ! _GSB_BANK_HH_ */