/*currency.hh*/

#ifndef _GSB_MONEY_HH_
#define _GSB_MONEY_HH_
#include <ostream>

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
		Money(double amount, Currency curr) :
			_amount(amount), _curr(curr) {}
		double amount() const { return _amount; }
		Currency currency() const { return _curr; }
};

std::ostream & operator<<(std::ostream &os, const Money &money) {
	return os << money.amount() << money.currency();
}

#endif
