/*accounts.hh*/

#ifndef _GSB_ACCOUNTS_H
#define _GSB_ACCOUNTS_H

#include "beings.hh"
#include "bank.hh"
#include "business_error.hh"
#include "money.hh"
#include "operation.hh"
#include <ostream>

class Bank;

class AccountSettings {
	double monthly_charge = 0.;
	double transfer_charge = 0.;
	double interest_rate = 0.;

public:
	AccountSettings(double monthlyCharge, double transferCharge, double interestRate) :
		monthly_charge(monthlyCharge), transfer_charge(transferCharge), interest_rate(interestRate) {

		// TODO: checkinf if they are positive?
	}

	double monthlyCharge() const { return monthly_charge; }
	double transferCharge() const { return transfer_charge; }
	double interestRate() const { return interest_rate; }
};

class Account {
	private:
	protected:
		id_type _id;											// Account ID
		double _balance;									// Balance of the account
		id_type bank_id;									// ID of the bank
		History _history;									// History of operations on this account
		const AccountSettings &settings;	// Account parameters

		Account(id_type account_id, id_type bank_id, const AccountSettings &settings) :
			_id(account_id),
			_balance(0.0),
			bank_id(bank_id),
			settings(settings) {}

	public:
		virtual ~Account() = 0;
		virtual const History & history() { return _history; }
		virtual double balance() const { return _balance; }
		virtual id_type id() const { return _id; }
		virtual void on_month_change() {
			double interests = _balance * settings.interestRate() / 100;
			double charges = settings.monthlyCharge();

			_balance += interests;
			_balance -= charges;

			// TODO: Wpis(y) do historii
		}
};

std::ostream &operator<<(std::ostream &os, const Account &acc) {
	os << acc.balance();
	return os;
}

class TransferAccount : public virtual Account {
public:
	TransferAccount(id_type id, id_type bank_id, const AccountSettings &settings) :
		Account(id, bank_id, settings) {}

	void transfer(Money money, id_type target_id, std::string msg = "") {
		if(money.amount() < 0) throw business_error("Transfering negative amount of money!");		// TODO: some subclassing
		// TODO
	}
	void transfer(double amount, id_type target_id, std::string msg = "") {
		this->transfer(Money(amount, Currency::ENC), target_id, msg);
	}
};

class WithdrawAccount : public virtual Account {
protected:
	Currency _currency;
public:
	WithdrawAccount(id_type id, id_type bank_id, Currency currency, const AccountSettings &settings) :
		Account(id, bank_id, settings), _currency(currency) {}

	void withdraw(Money money) {
		if(money.amount() < 0) throw business_error("Withdrawing negative amount of money!");	// TODO: some subclassing
		// TODO
	}

	void withdraw(double amount) {
		this->withdraw({amount, _currency});
	}
};

class CheckingAccount : public TransferAccount, public WithdrawAccount {
	public:
		CheckingAccount(id_type id, id_type bank_id, const AccountSettings &settings) :
			Account(id, bank_id, settings), TransferAccount(id, bank_id, settings), WithdrawAccount(id, bank_id, Currency::ENC, settings) {}

		void deposit(Money money) {
			if(money.amount() < 0) throw business_error("Depositing negative number of money!");	// TODO: some subclassing
			// TODO
		}
		void deposit(double amount) {
			this->deposit({amount, Currency::ENC});
		}
};

class SavingAccount : public TransferAccount {
	public:
		SavingAccount(id_type id, id_type bank_id, const AccountSettings &settings) :
			Account(id, bank_id, settings), TransferAccount(id, bank_id, settings) {}
};

class CurrencyAccount : public WithdrawAccount {
	public:
		CurrencyAccount(id_type id, id_type bank_id, Currency currency, const AccountSettings &settings) :
			Account(id, bank_id, settings), WithdrawAccount(id, bank_id, currency, settings) {}
};

#endif /* ! _GCB_ACCOUNTS_GG_ */
