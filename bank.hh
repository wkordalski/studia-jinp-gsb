#ifndef _GSB_BANK_HH_
#define _GSB_BANK_HH_

#include "accounts.hh"
#include "exchange_table.hh"

class Bank : public InterstellarClockObserver {
protected:
	std::string _name;
	std::vector<Account *> accounts;

	id_type _id;
	AccountSettings checking_settings;
	AccountSettings saving_settings;
	AccountSettings currency_settings;

	ExchangeTable exchange_table;
public:


	Bank(std::string name, id_type id,
		AccountSettings checking, AccountSettings saving, AccountSettings currency) :
		_name(name), _id(id),
		checking_settings(checking), saving_settings(saving), currency_settings(currency) {

		interstellarClock().registerMonthChangeObserver(this);
	}


	~Bank() {
		for (auto acc : accounts)
			delete acc;
		interstellarClock().unregisterMonthChangeObserver(this);
	}

	ExchangeTable & exchangeTable() {
		return exchange_table;
	}

	CheckingAccount & openCheckingAccount(const Being &who) {
		auto *ptr = new CheckingAccount(accounts.size(), _id, checking_settings);
		accounts.push_back(ptr);
		return *ptr;
	}

	SavingAccount & openSavingAccount(const Being &who) {
		auto *ptr = new SavingAccount(accounts.size(), _id, saving_settings);
		accounts.push_back(ptr);
		return *ptr;
	}

	CurrencyAccount & openCurrencyAccount(const Being &who, Currency currency) {
		auto *ptr = new CurrencyAccount(accounts.size(), _id, currency, currency_settings);
		accounts.push_back(ptr);
		return *ptr;
	}

	void on_month_change() {
		for (auto acc : accounts)
			acc->on_month_change();
	}
};


#endif /* ! _GSB_BANK_HH_ */
