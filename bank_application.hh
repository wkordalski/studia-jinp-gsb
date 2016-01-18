/*
class AccountBuilder {
public:
	double monthly_charge;
	double transfer_charge;
	double interest_rate;
};

class BankApplication {
	std::string name;
	AccountBuilder cas(*this);
	AccountBuilder *current = nullptr;
	//...

	BankApplication & name(std::string name) {
		this->name = name;
		return *this;
	}

	BankApplication & checkingAccount() {
		this->current = &this->cas;
		return *this;
	}
	
	BankApplication & monthlyCharge(double value) {
		this->current.monthly_charge = value;
		return *this;
	}
};
*/
//////////////////
#ifndef _GSB_BANK_APPLICATION_HH_
#define _GSB_BANK_APPLICATION_HH_

#include "bank.hh"

#include <string>

class BankApplication;

class AccountBuilder {
protected:
	BankApplication &app;
	double monthly_charge = 0.;
	double transfer_charge = 0.;
	double interest_rate = 1.;
public:
	AccountBuilder(BankApplication &app) : app(app) {}
	
	AccountBuilder & monthlyCharge(double value) {
		monthly_charge = value;
		return *this;
	}
	
	AccountBuilder & transferCharge(double value) {
		transfer_charge = value;
		return *this;
	}
	
	AccountBuilder & interestRate(double value) {
		interest_rate = value;
		return *this;
	}
	
	BankApplication & name(std::string name);
	Bank createBank();
	AccountBuilder & checkingAccount();
	AccountBuilder & savingAccount();
	AccountBuilder & currencyAccount();
	
	
	AccountSettings createAccountSettings() {
		return { monthly_charge, transfer_charge, interest_rate };
	}
};


class BankApplication {
protected:
	std::string _name = "";
	AccountBuilder checking_account_settings;
	AccountBuilder saving_account_settings;
	AccountBuilder currency_account_settings;
public:
	BankApplication() : 
		checking_account_settings(*this),
		saving_account_settings(*this),
		currency_account_settings(*this) {
	}
	
	BankApplication & name(std::string text) {
		this->_name = text;
		return *this;
	}
	
	Bank createBank() {
		return { _name, checking_account_settings.createAccountSettings(),
			saving_account_settings.createAccountSettings(),
			currency_account_settings.createAccountSettings()
		};
	}

	AccountBuilder & checkingAccount() {
		return checking_account_settings;
	}
	
	AccountBuilder & savingAccount() {
		return saving_account_settings;
	}
	
	AccountBuilder & currencyAccount() {
		return currency_account_settings;
	}
};


class GKB {
	BankApplication bankApplication() { return BankApplication(); }
};

GKB & gkb() {
	static GKB instance;
	return instance;
}



BankApplication & AccountBuilder::name(std::string name) { return app.name(name); }
Bank AccountBuilder::createBank() { return app.createBank(); }
AccountBuilder & AccountBuilder::checkingAccount() { return app.checkingAccount(); }
AccountBuilder & AccountBuilder::savingAccount() { return app.savingAccount(); }
AccountBuilder & AccountBuilder::currencyAccount() { return app.currencyAccount(); }

#endif /* ! _GSB_BANK_APPLICATION_HH_ */