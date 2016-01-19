/*accounts.hh*/

#ifndef _GSB_ACCOUNTS_H
#define _GSB_ACCOUNTS_H

class Account {
	private:
		AccountSettings acc_settings;
		//TODO informacje o właścicielu konta?
};

class CheckingAccount : public Account {

};

class SavingAccount : public Account {
	
};

class CurrencyAccount : public Account {

};

#endif /* ! _GCB_ACCOUNTS_GG_ */
