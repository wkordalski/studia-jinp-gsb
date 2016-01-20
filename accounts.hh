/*accounts.hh*/

#ifndef _GSB_ACCOUNTS_H
#define _GSB_ACCOUNTS_H

#include "beings.hh"
#include "bank.hh"
#include <ostream>

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
		AccountSettings acc_settings;
		//TODO informacje o właścicielu konta?
		id_type _id;
	protected:
		double balance;
	public:
		double giveBalance() const { return balance; }
		id_type id() { return _id; }
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
		void transfer(double amount /*, currency_type curr = default currency*/) {
			/*
			* Jak sprawić, żeby Transfer zmieniał stan innego konta?
			* Może każde konto powinno pamiętać swój bank?
			*/
		}
};

class WithdrawAccount : public Account {
	public:
		void withdraw(double amount /*, currency_type curr = default currency*/) {
			if (amount > balance)
				;/*throw wherewithalNotEnoughException()*/
			else
				balance -= amount;/* razy przelicznik waluty */
		}
};

class CheckingAccount : public TransferAccount, public WithdrawAccount {
	public:
		void deposit(double amount /*, currency_type curr = default currency*/) {
			// TODO
		}
};

class SavingAccount : public WithdrawAccount {};

class CurrencyAccount : public TransferAccount {};

/*
class CheckingAccount : public Account {
	public:
		//TODO napisać, co właściwie robią te funkcje
		void deposit(double amount, Currency curr = Currency::ENC ) {

		}
		void withdraw(double amount, Currency curr = Currency::ENC) {

		}
		
		void transfer(double amount, id_type where_to , const std::string &msg = "") {
			
		}
};

class SavingAccount : public Account {
	public:
		void transfer(double amount, id_type where_to, const std::string &msg = "") {

		}
};

class CurrencyAccount : public Account {
	public:
		void withdraw(double amount)
};
*/
#endif /* ! _GCB_ACCOUNTS_GG_ */
