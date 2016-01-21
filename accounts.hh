/*accounts.hh*/

#ifndef _GSB_ACCOUNTS_H
#define _GSB_ACCOUNTS_H

#include "currency.hh"
#include "beings.hh"
#include "bank.hh"
#include <ostream>

class Bank;

class AccountSettings {
	double monthly_charge = 0.;
	double transfer_charge = 0.;
	double interest_rate = 0.;
	
public:
	AccountSettings(double monthlyCharge, double transferCharge, double interestRate) :
		monthly_charge(monthlyCharge), transfer_charge(transferCharge), interest_rate(interestRate) {
	}
};

class Account {
	private:
		//AccountSettings acc_settings;
		//TODO informacje o właścicielu konta?
		//Edit: chyba są niepotrzebne
	protected:
		id_type _id;
		double _balance;
		id_type bank_id;
		//Account() : _id(), _balance(), bank_id() {}
		Account(id_type id, id_type bank_id) : 
			_id(id),
			_balance(0.0),
			bank_id(bank_id) {}
	public:
	
		virtual double giveBalance() const { return _balance; }
		virtual id_type id() const { return _id; }
		virtual void update_monthly() {
			// co ma się dziać co miesiąc?
		}
};

std::ostream &operator<<(std::ostream &os, const Account &acc) {
	os << acc.giveBalance();
	return os;
}

class TransferAccount : public virtual Account {
	public:
		TransferAccount(id_type id, id_type bank_id) :
			Account(id, bank_id) {}
		
		void transfer(double amount /*, currency_type curr = default currency*/) {
		}
};

class WithdrawAccount : public virtual Account {
	public:
		WithdrawAccount(id_type id, id_type bank_id) :
			Account(id, bank_id) {}

		void withdraw(double amount /*, currency_type curr = default currency*/) {
			if (amount > _balance)
				;/*throw wherewithalNotEnoughException()*/
			else
				_balance -= amount;/* razy przelicznik waluty */
		}
};

class CheckingAccount : public TransferAccount, public WithdrawAccount {
	public:
		CheckingAccount(id_type id, id_type bank_id) :
			Account(id, bank_id), TransferAccount(id, bank_id), WithdrawAccount(id, bank_id) {}

		void deposit(double amount /*, currency_type curr = default currency*/) {
			// TODO
		}
};

class SavingAccount : public WithdrawAccount {
	public:
		SavingAccount(id_type id, id_type bank_id) :
			Account(id, bank_id), WithdrawAccount(id, bank_id) {}
};

class CurrencyAccount : public TransferAccount {
	public:
		CurrencyAccount(id_type id, id_type bank_id) :
			Account(id, bank_id), TransferAccount(id, bank_id) {}
};

#endif /* ! _GCB_ACCOUNTS_GG_ */
