#ifndef _GSB_OPERATION_HH
#define _GSB_OPERATION_HH

#include <ostream>
#include "beings.hh"
#include "interstellarclock.h"
#include "money.hh"


class Operation {
	protected:
		Date _date;

	public:
		Operation(Date date) : _date(date) {}
		virtual ~Operation() {}
		Date date() const { return _date; }

		virtual void print(std::ostream &os) const = 0;
};

class Transfer : public Operation {
protected:
	Money _money;
	id_type _from;
	id_type _to;
	std::string _msg;
public:
	Transfer(Date date, Money money, id_type from, id_type to, std::string msg) :
			Operation(date), _money(money), _from(from), _to(to), _msg(msg) {};

	Money money() const { return _money; }
	id_type from() const { return _from; }
	id_type to() const { return _to; }
	std::string msg() const { return _msg; }

	virtual void print(std::ostream &os) const {
		os << date() << " " << money() << " TRANSFER ";
		if(msg() != "") os << "(" << msg() << ")";
		os << " FROM:" << from() << " TO: " << to();
	}
};


class Deposit : public Operation {
protected:
	Money _money;
public:
	Deposit(Date date, Money money) :
			Operation(date), _money(money) {}

	Money money() const { return _money; }

	virtual void print(std::ostream &os) const {
		os << date() << " " << money() << " DEPOSIT";
	}
};

class Withdrawal : public Operation {
		Money _money;
	public:
		Withdrawal(Date date, Money money) :
			Operation(date), _money(money) {}

		Money money() const { return _money; }

		virtual void print(std::ostream &os) const {
			os << date() << " " << money() << " WITHDRAWAL";
		}
};


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

std::ostream & operator<<(std::ostream &os, const Operation &op) {
	op.print(os);
	return os;
}

std::ostream & operator<<(std::ostream &os, const History &h) {
	for (auto op : h.operations())
		os << *op << std::endl;
  return os;
}

#endif
