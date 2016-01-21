/*currency.hh*/

#ifndef _GSB_CURRENCY_HH
#define _GSBK_CURRENCY_HH
#include <ostream>

#define NUMBER_OF_CURRENCIES 4

enum class Currency {
	ENC = 0, BIC = 1, DIL = 2, LIT = 3
};

std::ostream &operator<<(std::ostream &os, const Currency &curr) {
	switch(curr) {
		case Currency::ENC:
			return os << "ENC";
		case Currency::BIC:
			return os << "BIC";
		case Currency::DIL:
			return os << "DIL";
		case Currency::LIT:
			return os << "LIT";
		default:
			return os;
	}
}

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
