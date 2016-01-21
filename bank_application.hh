#ifndef _GSB_BANK_APPLICATION_HH_
#define _GSB_BANK_APPLICATION_HH_

#include "bank.hh"

#include <string>

class BankApplication;

struct AccountSettingsBuilder {
	double monthly_charge = 0.;
	double transfer_charge = 0.;
	double interest_rate = 0.;

	AccountSettings createAccountSettings() {
		return {monthly_charge, transfer_charge, interest_rate};
	}
};


class BankApplication {
protected:
	std::string _name = "";
	AccountSettingsBuilder checking_account_settings;
	AccountSettingsBuilder saving_account_settings;
	AccountSettingsBuilder currency_account_settings;
	AccountSettingsBuilder *current = nullptr;
public:

	BankApplication & name(std::string text) {
		this->_name = text;
		// TODO: this->current = nullptr?
		return *this;
	}

	Bank createBank();// {
//		return { _name, std::vector<Account>(), gkb().next_id(),
//			checking_account_settings.createAccountSettings(),
//			saving_account_settings.createAccountSettings(),
//			currency_account_settings.createAccountSettings()
//		};
//	}

	BankApplication & checkingAccount() {
		this->current = &this->checking_account_settings;
		return *this;
	}

	BankApplication & savingAccount() {
		this->current = &this->saving_account_settings;
		return *this;
	}

	BankApplication & currencyAccount() {
		this->current = &this->currency_account_settings;
		return *this;
	}

	BankApplication & monthlyCharge(double value) {
		if(this->current == nullptr) throw std::logic_error("You have to select account type to set its properties");
		this->current->monthly_charge = value;
		return *this;
	}

	BankApplication & transferCharge(double value) {
		if(this->current == nullptr) throw std::logic_error("You have to select account type to set its properties");
		this->current->transfer_charge = value;
		return *this;
	}

	BankApplication & interestRate(double value) {
		if(this->current == nullptr) throw std::logic_error("You have to select account type to set its properties");
		this->current->interest_rate = value;
		return *this;
	}
};

	/*
	* czy poniższe edity są w porządku?
	*/
class GKB {
private:
	GKB() = default;
	std::vector<Bank> banks;
public:
	id_type next_id() { return banks.size(); }
	const Bank  &add_bank(const Bank &bank) {
		banks.push_back(bank);
		return bank;
	}
	BankApplication bankApplication() { return BankApplication(); }
	friend GKB & gkb();
};

GKB & gkb() {
	static GKB instance;
	return instance;
}


Bank BankApplication::createBank() {
	return gkb().add_bank({ _name, gkb().next_id(),
		checking_account_settings.createAccountSettings(),
		saving_account_settings.createAccountSettings(),
		currency_account_settings.createAccountSettings()
	});
}
#endif /* ! _GSB_BANK_APPLICATION_HH_ */
