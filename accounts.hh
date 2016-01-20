/*accounts.hh*/

#ifndef _GSB_ACCOUNTS_H
#define _GSB_ACCOUNTS_H

#include "beings.hh"
#include "bank.hh"
#include <ostream>

class Bank;

class AccountSettings {
	double monthly_charge = 0.;
	double transfer_charge = 0.;
	double interest_rate = 1.;
	
public:
	AccountSettings(double monthlyCharge, double transferCharge, double interestRate) :
		monthly_charge(monthlyCharge), transfer_charge(transferCharge), interest_rate(interestRate) {
	}
};

class Account {
	private:
		//AccountSettings acc_settings;
		//TODO informacje o właścicielu konta?
	protected:
		id_type _id;
		double _balance;
		const Bank &_bank;
	public:
		Account(id_type id, const Bank &bank) : 
			id_type(id),
			_balance(0.0),
			_bank(bank) {}
	
		double giveBalance() const { return _balance; }
		id_type id() const { return _id; }
};

	/*
	* Pytanie: może zastosować tu podwójne dziedziczenie?
	* W końcu CheckingAccount i CurrencyAccount mają metody withdraw,
	* a CheckingAccount i SavingAccount mają metody transfer. Wyglądałoby to
	* jakoś tak jak rozgałęziony diament nienawiści:
	*
	* 				Account
	*			/			\
	*		Withdrawable	Transferable
	*		/		\		/		\
	*	 Currency	Checking		Saving 
	*
	*
	* Edit: tak zrobię. Stara wersja w komentarzu
	*/

std::ostream &operator<<(std::ostream &os, const Account &acc) {
	os << acc.giveBalance();
	return os;
}

class TransferAccount : public Account {
	public:
		TransferAccount(id_type id, const Bank &bank) :
			Account(id, bank) {}
		
		void transfer(double amount /*, currency_type curr = default currency*/) {
			/*
			* Jak sprawić, żeby Transfer zmieniał stan innego konta?
			* Może każde konto powinno pamiętać swój bank?
			*/
		}
};

class WithdrawAccount : public Account {
	public:
		TransferAccount(id_type id, const Bank &bank) :
			Account(id, bank) {}

		void withdraw(double amount /*, currency_type curr = default currency*/) {
			if (amount > balance)
				;/*throw wherewithalNotEnoughException()*/
			else
				balance -= amount;/* razy przelicznik waluty */
		}
};

class CheckingAccount : public TransferAccount, public WithdrawAccount {
	public:
		CheckingAccount(id_type id, const Bank &bank) :
			TransferAccount(id, bank) {}

		void deposit(double amount /*, currency_type curr = default currency*/) {
};

class SavingAccount : public WithdrawAccount {
	public:
		SavingAccount(id_type id, const Bank &bank) :
			WithdrawAccount(id, bank) {}
};

class CurrencyAccount : public TransferAccount {
	public:
		CurrencyAccount(id_type id, const Bank &bank) :
			TransferAccount(id, bank) {}
};

#endif /* ! _GCB_ACCOUNTS_GG_ */
