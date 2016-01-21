#ifndef _GSB_OPERATION_HH
#define _GSB_OPERATION_HH

#include <ostream>
#include "beings.hh"
#include "interstellarclock.h"


class Operation {	
	protected:
		Operation(Date date, double amount, Currency curr) :
			_date(date), _amount(amount), _curr(curr) {}
		Date _date;
		double _amount;
		Currency _curr;
	public:
		Date date() const { return _date; }
		double amount() const { return _amount; }
		Currency curr() const { return _curr; }
};

std::ostream &operator<<(std::ostream &os, const Operation &op) {
	return os;
}

class Transfer : public Operation {
	private:
		id_type _from;
		id_type _to;
		std::string _msg;
	public:
		Transfer(Date date, double amount, Currency curr,
			id_type from, id_type to, std::string msg) :
				Operation(date, amount, curr),
				_from(from), _to(to), _msg(msg) {};
		id_type from() const { return _from; }
		id_type to() const { return _to; }
		std::string msg() const { return _msg; }
};

std::ostream &operator<<(std::ostream &os, const Transfer &tr) {
	os << tr.date() << " "
		<< tr.amount() << tr.curr()
		<< " TRANSFER (" << tr.msg() << ") FROM:"
		<< tr.from() << " TO: " << tr.to();
	return os;
}

class Deposit : public Operation {
	public:
		Deposit(Date date, double amount, Currency curr) :
			Operation(date, amount, curr) {}
};

std::ostream &operator<<(std::ostream &os, const Deposit &dep) {
	os << dep.date() << " "
		<< dep.amount() << dep.curr()
		<< " DEPOSIT";
}


class Withdrawal : public Operation {
	public:
		Withdrawal(Date date, double amount, Currency curr) :
			Operation(date, amount, curr) {}
};

std::ostream &operator<<(std::ostream &os, const Withdrawal &wit) {
	os << wit.date() << " "
		<< wit.amount() << wit.curr()
		<< " WITHDRAWAL";
}

class History {
	private:
		std::vector<Operation *> _operations;
	public:
		History() {}
		std::vector<Operation *> operations() const { return _operations; }
		void addOperation(Operation *p) {
			_operations.push_back(p);
		}
		~History() {
			for (auto op : _operations)
				delete op;
		}
};

std::ostream &operator<<(std::ostream &os, const History &h) {
	for (auto op : h.operations())
		os << *op << std::endl;
}

#endif
