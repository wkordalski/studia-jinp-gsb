/*currency.hh*/

#ifndef _GSB_CURRENCY_HH
#define _GSBK_CURRENCY_HH

#define NUMBER_OF_CURRENCIES 4

enum class Currency {
	ENC = 0, BIC = 1, DIL = 2, LIT = 3
};

class Money {
	private:
		double _amount;
		Currency _curr;
	public:
		Money(double amount, Currency curr = Currency::ENC) : 
			_amount(amount), _curr(curr) {}
		double amount() { return _amount; }
		Currency curr() { return _curr; }	
};

#endif
