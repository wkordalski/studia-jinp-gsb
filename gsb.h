#ifndef _GSB_GSB_HH_
#define _GSB_GSB_HH_

#include <cstddef>
#include <exception>
#include <map>
#include <ostream>
#include <string>
#include <iomanip>
#include <vector>
#include "interstellarclock.h"

/*
===========================================================
        LIST OF ALL ACCOUNTS
===========================================================
*/

class Account;
class Bank;

std::vector<std::vector<Account *>> &all_accounts_ever() {
  static std::vector<std::vector<Account *>> instance;
  return instance;
}

std::vector<Bank *> &all_banks_ever() {
  static std::vector<Bank *> instance;
  return instance;
}

/*
* I know this is poor style, but we can't have all accounts in
* GKB, because GKB and Account are implementationwise dependent
* on each other and we aren't allowed to submit compiled code,
* just this header.
*/

/*
===========================================================
        EXCEPTIONS
===========================================================
*/

class beingNotFoundException : std::exception {
  virtual const char *what() { return "Being not found"; }
};

class BusinessException : std::exception {
private:
  const char *_msg;

public:
  BusinessException(const char *s) : _msg(s) {}
  virtual const char *what() const noexcept { return _msg; }
};

/*
===========================================================
        ID CLASS
===========================================================
*/
class ID {
private:
  bool _valid = false;
  std::size_t _value = 0;

public:
  ID() = default;
  ID(std::size_t v) : _valid(true), _value(v) {}
  ID(const ID &) = default;

  template <typename T> ID(const T &t) : _valid(false) {}
  /*
          ID constructed from any other type than
          std::size_t is not valid
  */
  bool valid() const { return _valid; }
  std::size_t value() const { return _value; }

  bool operator==(const ID &other) const {
    if (!_valid && !other._valid)
      return true;
    if (!_valid || !other._valid)
      return false;
    return _value == other._value;
  }

  bool operator<(const ID &other) const {
    if (!other._valid)
      return false;
    if (!_valid && other._valid)
      return true;
    return _value < other._value;
  }

  operator std::size_t() { return _value; }
};

/*
* AccountID has two coordinates: one for the bank,
* another for the account
*/

class AccountID {
private:
  bool _valid = false;
  std::size_t _value1 = 0;
  std::size_t _value2 = 0;

public:
  AccountID() = default;
  AccountID(std::size_t v1, std::size_t v2)
      : _valid(true), _value1(v1), _value2(v2) {}
  AccountID(const AccountID &) = default;

  template <typename T> AccountID(const T &t) : _valid(false) {}

  bool valid() const { return _valid; }
  std::size_t value1() const { return _value1; }
  std::size_t value2() const { return _value2; }

  bool operator==(const AccountID &other) const {
    if (!_valid && !other._valid)
      return true;
    if (!_valid || !other._valid)
      return false;
    return (_value1 == other._value1 && _value2 == other._value2);
  }

  bool operator<(const AccountID &other) const {
    if (!other._valid)
      return false;
    if (!_valid && other._valid)
      return true;
    return _value1 < other._value1 ||
           (_value1 == other._value1 && _value2 < other._value2);
  }

  operator std::size_t() { return _value2; }
};

std::ostream &operator<<(std::ostream &os, const ID &id) {
  os << std::fixed << std::setprecision(2) << id.value();
  return os;
}

std::ostream &operator<<(std::ostream &os, const AccountID &id) {
  os << std::fixed << std::setprecision(2) << "(" << id.value1() << ", "
     << id.value2() << ")";
  return os;
}

using id_type = ID;
using acc_id_type = AccountID;

/*
===========================================================
        BEINGS
===========================================================
*/

class Being {
protected:
  id_type _id;

public:
  Being(id_type id) : _id(id) {}
  virtual ~Being() {}
  id_type id() { return _id.value(); }
  virtual std::string name() const = 0;
};

class Human : public Being {
private:
  std::string _name;

public:
  Human(id_type id, std::string name) : Being(id), _name(name) {}
  virtual std::string name() const { return _name; }
};

class Klingon : public Being {
private:
  std::string _name;

public:
  Klingon(id_type id, std::string name) : Being(id), _name(name) {}
  virtual std::string name() const { return _name; }
};

class Binarius : public Being { // component
public:
  Binarius(id_type id) : Being(id) {}
};

class CompositeBinarius : public Binarius { // composite
private:
  Binarius &b0, &b1;

public:
  CompositeBinarius(id_type id, Binarius &b0, Binarius &b1)
      : Binarius(id), b0(b0), b1(b1) {}
  virtual std::string name() const { return b0.name() + "&" + b1.name(); }
};

class SingleBinarius : public Binarius { // leaf
public:
  std::string _name;

public:
  SingleBinarius(id_type id, std::string name) : Binarius(id), _name(name) {}
  virtual std::string name() const { return _name; }
};

/*
===========================================================
        PLANETS
===========================================================
*/

class Planet {
public:
  virtual ~Planet() {}

  virtual Being &registerCitizen(std::string name) = 0;
  virtual Being &findCitizen(std::string name) {
    auto it = id_by_name.find(name);
    if (it != id_by_name.end())
      return findCitizen(id_by_name[name]);
    else
      throw beingNotFoundException();
  }
  virtual Being &findCitizen(id_type id) = 0;

protected:
  std::map<std::string, id_type> id_by_name;
};

template <typename BeingT> class SimplePlanetMixin : Planet {
protected:
  std::map<id_type, BeingT *> registered_beings;
  std::size_t counter = 0;

public:
  virtual ~SimplePlanetMixin() {
    for (auto p : registered_beings) {
      delete p.second;
    }
  }

  virtual BeingT &registerCitizen(std::string name) {
    auto id = counter++;
    return *(registered_beings[id] = new BeingT(id, name));
  }

  virtual BeingT &findCitizen(id_type id) {
    auto it = registered_beings.find(id);
    if (id.valid() && it != registered_beings.end())
      return *(it->second);
    else
      throw beingNotFoundException();
  }
};

class Earth : public SimplePlanetMixin<Human> {};

class Kronos : public SimplePlanetMixin<Klingon> {};

class Bajnaus : public Planet {
protected:
  std::map<id_type, Binarius *> registered_beings;
  std::size_t counter = 0;

public:
  virtual ~Bajnaus() {
    for (auto p : registered_beings) {
      delete p.second;
    }
  }

  virtual Binarius &registerCitizen(std::string name) {
    auto id = counter++;
    return *(registered_beings[id] = new SingleBinarius(id, name));
  }

  virtual Binarius &registerCitizen(Binarius &b0, Binarius &b1) {
    auto id = counter++;
    return *(registered_beings[id] = new CompositeBinarius(id, b0, b1));
  }

  virtual Binarius &findCitizen(id_type id) {
    auto it = registered_beings.find(id);
    if (id.valid() && it != registered_beings.end())
      return *(it->second);
    else
      throw beingNotFoundException();
  }
};

Earth &earth() {
  static Earth instance;
  return instance;
}

Kronos &qonos() {
  static Kronos instance;
  return instance;
}

Bajnaus &bynaus() {
  static Bajnaus instance;
  return instance;
}

/*
===========================================================
        CURRENCY & MONEY
===========================================================
*/

enum class Currency { ENC = 0, BIC = 1, DIL = 2, LIT = 3 };

std::ostream &operator<<(std::ostream &os, const Currency &curr) {
  switch (curr) {
  case Currency::ENC:
    return os << std::fixed << std::setprecision(2) << "ENC";
  case Currency::BIC:
    return os << std::fixed << std::setprecision(2) << "BIC";
  case Currency::DIL:
    return os << std::fixed << std::setprecision(2) << "DIL";
  case Currency::LIT:
    return os << std::fixed << std::setprecision(2) << "LIT";
  default:
    return os;
  }
}

class Money {
private:
  double _amount;
  Currency _curr;

public:
  Money(double amount, Currency curr) : _amount(amount), _curr(curr) {}
  double amount() const { return _amount; }
  Currency currency() const { return _curr; }
};

std::ostream &operator<<(std::ostream &os, const Money &money) {
  return os << std::fixed << std::setprecision(2) << money.amount()
            << money.currency();
}

Money operator-(Money money) { return {-money.amount(), money.currency()}; }

/*
===========================================================
        OPERATION & HISTORY
===========================================================
*/

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
  acc_id_type _from;
  acc_id_type _to;
  std::string _msg;

public:
  Transfer(Date date, Money money, acc_id_type from, acc_id_type to,
           std::string msg)
      : Operation(date), _money(money), _from(from), _to(to), _msg(msg){};

  Money money() const { return _money; }
  acc_id_type from() const { return _from; }
  acc_id_type to() const { return _to; }
  std::string msg() const { return _msg; }

  virtual void print(std::ostream &os) const {
    os << std::fixed << std::setprecision(2) << date() << " " << money()
       << " TRANSFER "
       << "(" << msg() << ")"
       << " FROM: " << from() << " TO: " << to();
  }
};

/*
* OperationMixin stands for withdrawals, charges and interests
*/
class SimpleOperationMixin : public Operation {
protected:
  Money _money;
  std::string _oper; // name of the operation
public:
  SimpleOperationMixin(Date date, Money money, std::string oper)
      : Operation(date), _money(money), _oper(oper) {}

  Money money() const { return _money; }

  virtual void print(std::ostream &os) const {
    os << std::fixed << std::setprecision(2) << date() << " " << money() << " "
       << _oper;
  }
};

class Withdrawal : public SimpleOperationMixin {
public:
  Withdrawal(Date date, Money money)
      : SimpleOperationMixin(date, money, "WITHDRAWAL") {}
};

class Charge : public SimpleOperationMixin {
public:
  Charge(Date date, Money money)
      : SimpleOperationMixin(date, money, "CHARGE") {}
};
class Interest : public SimpleOperationMixin {
public:
  Interest(Date date, Money money)
      : SimpleOperationMixin(date, money, "INTEREST") {}
};
class Deposit : public SimpleOperationMixin {
public:
  Deposit(Date date, Money money)
      : SimpleOperationMixin(date, money, "DEPOSIT") {}
};

class History {
private:
  std::vector<Operation *> _operations;

public:
  History() {}
  std::vector<Operation *> operations() const { return _operations; }
  void add(Operation *p) { _operations.push_back(p); }
  ~History() {
    for (auto op : _operations)
      delete op;
  }
};

std::ostream &operator<<(std::ostream &os, const Operation &op) {
  op.print(os);
  return os;
}

std::ostream &operator<<(std::ostream &os, const History &h) {
  for (auto op : h.operations())
    os << std::fixed << std::setprecision(2) << *op << "\n";
  return os;
}

/*
===========================================================
        EXCHANGE TABLE
===========================================================
*/

class ExchangeTable {
protected:
  struct TableEntry {
    double buy = 1.;
    double sell = 1.;
  };

protected:
  std::map<Currency, TableEntry> _map;
  TableEntry *_current_entry = nullptr;

public:
  ExchangeTable &exchangeRate(Currency currency) {
    _current_entry = &_map[currency];
    return *this;
  }

  ExchangeTable &buyingRate(double value) {
    if (_current_entry)
      _current_entry->buy = value;
    else
      throw std::logic_error("Set currency before setting rates.");
    return *this;
  }
  ExchangeTable &sellingRate(double value) {
    if (_current_entry)
      _current_entry->sell = value;
    else
      throw std::logic_error("Set currency before setting rates.");
    return *this;
  }

  // Changes money to different currency using the ExchangeTable.
  Money change(Money money, Currency currency) const {
    // No change needed...
    if (money.currency() == currency)
      return money;

    auto entry = [this](Currency c) {
      auto it = _map.find(c);
      if (it == _map.end())
        return TableEntry();
      else
        return it->second;
    };

    Money enc = (money.currency() == Currency::ENC)
                    ? money
                    : Money(money.amount() * entry(money.currency()).sell,
                            Currency::ENC);
    return (currency == Currency::ENC)
               ? enc
               : Money(enc.amount() * entry(currency).buy, currency);
  }
};

/*
===========================================================
        ACCOUNTS
===========================================================
*/

class Bank;

class AccountSettings {
  double monthly_charge = 0.;
  double transfer_charge = 0.;
  double interest_rate = 0.;

public:
  AccountSettings(double monthlyCharge, double transferCharge,
                  double interestRate)
      : monthly_charge(monthlyCharge), transfer_charge(transferCharge),
        interest_rate(interestRate) {
    if (monthlyCharge < .0 || transferCharge < .0 || interestRate < .0) {
      throw BusinessException("You can't charge negative amounts of money!");
    }
  }

  double monthlyCharge() const { return monthly_charge; }
  double transferCharge() const { return transfer_charge; }
  double interestRate() const { return interest_rate; }
};

class Account {
private:
protected:
  acc_id_type _id;                 // Account ID
  double _balance;                 // Balance of the account
  id_type bank_id;                 // ID of the bank
  History _history;                // History of operations on this account
  const AccountSettings &settings; // Account parameters
  Currency _currency;              // Currency of the account

  Account(acc_id_type account_id, id_type bank_id,
          const AccountSettings &settings, Currency currency = Currency::ENC)
      : _id(account_id), _balance(0.0), bank_id(bank_id), settings(settings),
        _currency(currency) {}

public:
  virtual ~Account() {}
  virtual const History &history() const { return _history; }
  virtual Money balance() const { return {_balance, _currency}; }
  virtual acc_id_type id() const { return _id; }
  virtual Currency currency() const { return _currency; }
  virtual void on_month_change() {
    double interests = _balance * settings.interestRate() / 100;
    double charges = settings.monthlyCharge();

    if (_balance > 0) {
      _balance += interests;
      _history.add(
          new Interest(interstellarClock().date(), {interests, _currency}));
    }

    _balance -= charges;
    _history.add(
        new Charge(interstellarClock().date(), {(-1.0) * charges, _currency}));
  }
  Bank *getBank() { return all_banks_ever()[bank_id]; }

protected:
  void receive_transfer(Money money, acc_id_type from, const char *msg = "");

public:
  void transfer(Money money, acc_id_type to, const char *msg = "");

  void transfer(double amount, acc_id_type to, const char *msg = "") {
    transfer({amount, currency()}, to, msg);
  }
};

std::ostream &operator<<(std::ostream &os, const Account &acc) {
  os << std::fixed << std::setprecision(2) << "Account: " << acc.id()
     << std::endl
     << "Balance: " << acc.balance() << std::endl
     << "History:\n" << acc.history();
  return os;
}

class CheckingAccount : public Account {
public:
  CheckingAccount(acc_id_type id, id_type bank_id,
                  const AccountSettings &settings)
      : Account(id, bank_id, settings) {}

  void deposit(Money money) {
    if (money.amount() < 0)
      throw BusinessException("Depositing negative number of money!");
    if (money.currency() != currency())
      throw BusinessException("Wrong currency");
    _balance += money.amount();
    _history.add(new Deposit(interstellarClock().date(), money));
  }
  void deposit(double amount) { this->deposit({amount, _currency}); }

  void withdraw(Money money) {
    if (money.currency() != currency())
      throw BusinessException("Wrong currency");
    if (money.amount() < 0)
      throw BusinessException("Withdrawing negative amount of money");
    if (money.amount() > _balance)
      throw BusinessException("Insufficient balance");
    _balance -= money.amount();
    _history.add(new Withdrawal(interstellarClock().date(), -money));
  }

  void withdraw(double amount) { withdraw({amount, currency()}); }
};

class SavingAccount : public Account {
public:
  SavingAccount(acc_id_type id, id_type bank_id,
                const AccountSettings &settings)
      : Account(id, bank_id, settings) {}
};

class CurrencyAccount : public Account {
public:
  CurrencyAccount(acc_id_type id, id_type bank_id, Currency currency,
                  const AccountSettings &settings)
      : Account(id, bank_id, settings, currency) {}

  void withdraw(Money money);
  void withdraw(double amount) { withdraw({amount, _currency}); }
};

/*
===========================================================
        BANKS
===========================================================
*/

class Bank : public InterstellarClockObserver {
protected:
  std::string _name;
  std::vector<Account *> accounts;

  id_type _id;
  AccountSettings checking_settings;
  AccountSettings saving_settings;
  AccountSettings currency_settings;

  ExchangeTable exchange_table;

public:
  Bank(Bank &&other)
      : _name(std::move(other._name)), accounts(std::move(other.accounts)),
        _id(std::move(other._id)),
        checking_settings(std::move(other.checking_settings)),
        saving_settings(std::move(other.saving_settings)),
        currency_settings(std::move(other.currency_settings)),
        exchange_table(std::move(other.exchange_table)) {
    interstellarClock().registerMonthChangeObserver(this);
  }

  Bank(std::string name, id_type id, AccountSettings checking,
       AccountSettings saving, AccountSettings currency)
      : _name(name), _id(id), checking_settings(checking),
        saving_settings(saving), currency_settings(currency) {
    interstellarClock().registerMonthChangeObserver(this);
  }

  virtual ~Bank() {
    for (auto acc : accounts)
      delete acc;
    // interstellarClock().unregisterMonthChangeObserver(this);
  }

  ExchangeTable &exchangeTable() { return exchange_table; }

  CheckingAccount &openCheckingAccount(const Being &who) {
    auto *ptr =
        new CheckingAccount({_id, accounts.size()}, _id, checking_settings);
    accounts.push_back(ptr);
    all_accounts_ever()[_id].push_back(ptr);
    return *ptr;
  }

  SavingAccount &openSavingAccount(const Being &who) {
    auto *ptr = new SavingAccount({_id, accounts.size()}, _id, saving_settings);
    all_accounts_ever()[_id].push_back(ptr);
    accounts.push_back(ptr);
    return *ptr;
  }

  CurrencyAccount &openCurrencyAccount(const Being &who, Currency currency) {
    auto *ptr = new CurrencyAccount({_id, accounts.size()}, _id, currency,
                                    currency_settings);
    all_accounts_ever()[_id].push_back(ptr);
    accounts.push_back(ptr);
    return *ptr;
  }

  void on_month_change() {
    for (auto acc : accounts)
      acc->on_month_change();
  }
};
/*
===========================================================
        BANK APPLICATIONS
===========================================================
*/

class BankApplication;

struct AccountSettingsBuilder {
  double monthly_charge = 0.;
  double transfer_charge = 0.;
  double interest_rate = 0.;

  AccountSettings createAccountSettings() {
    return {monthly_charge, transfer_charge, interest_rate};
  }
};

class BankApplication {
protected:
  std::string _name = "";
  AccountSettingsBuilder checking_account_settings;
  AccountSettingsBuilder saving_account_settings;
  AccountSettingsBuilder currency_account_settings;
  AccountSettingsBuilder *current = nullptr;

public:
  BankApplication &name(std::string text) {
    this->_name = text;
    return *this;
  }

  Bank &createBank();

  BankApplication &checkingAccount() {
    this->current = &this->checking_account_settings;
    return *this;
  }

  BankApplication &savingAccount() {
    this->current = &this->saving_account_settings;
    return *this;
  }

  BankApplication &currencyAccount() {
    this->current = &this->currency_account_settings;
    return *this;
  }

  BankApplication &monthlyCharge(double value) {
    if (this->current == nullptr)
      throw std::logic_error(
          "You have to select account type to set its properties");
    this->current->monthly_charge = value;
    return *this;
  }

  BankApplication &transferCharge(double value) {
    if (this->current == nullptr)
      throw std::logic_error(
          "You have to select account type to set its properties");
    this->current->transfer_charge = value;
    return *this;
  }

  BankApplication &interestRate(double value) {
    if (this->current == nullptr)
      throw std::logic_error(
          "You have to select account type to set its properties");
    this->current->interest_rate = value;
    return *this;
  }
};

class GKB {
private:
  GKB() = default;
  std::vector<Bank *> banks;

public:
  ~GKB() {
    for (Bank *b : banks)
      delete b;
  }
  id_type next_id() { return banks.size(); }
  const std::vector<Bank *> &registered_banks() { return banks; }
  Bank &add_bank(Bank *bank) {
    banks.push_back(bank);
    return *bank;
  }
  BankApplication bankApplication() { return BankApplication(); }
  friend GKB &gkb();
};

GKB &gkb() {
  static GKB instance;
  return instance;
}

Bank &BankApplication::createBank() {
  all_accounts_ever().push_back(std::vector<Account *>());
  auto ptr = new Bank(_name, gkb().next_id(),
                      checking_account_settings.createAccountSettings(),
                      saving_account_settings.createAccountSettings(),
                      currency_account_settings.createAccountSettings());
  all_banks_ever().push_back(ptr);
  gkb().add_bank(ptr);
  return *ptr;
}

void Account::receive_transfer(Money money, acc_id_type from, const char *msg) {
  if (money.amount() < 0) {
    throw BusinessException("We don't receive negative transfers");
  }
  _balance +=
      this->getBank()->exchangeTable().change(money, _currency).amount();
  _history.add(new Transfer(interstellarClock().date(), money, from, _id, msg));
}

void Account::transfer(Money money, acc_id_type to, const char *msg) {
  if (money.amount() > _balance) {
    throw BusinessException("Insufficient balance");
  }

  if (!to.valid() || to.value1() > all_accounts_ever().size() ||
      to.value2() > all_accounts_ever()[to.value1()].size()) {
    throw BusinessException("Invalid identifier");
  }

  if (money.currency() != currency())
    throw BusinessException("Wrong currency.");

  Account *target_account = all_accounts_ever()[to.value1()][to.value2()];

  _balance -= money.amount();
  try {
    if (target_account->currency() == currency()) {
      // Wysyłamy w walucie konta
      target_account->receive_transfer(money, _id, msg);
    } else {
      // Wysyłamy w ENC
      auto in_enc = getBank()->exchangeTable().change(money, Currency::ENC);
      target_account->receive_transfer(in_enc, _id, msg);
    }

    _history.add(
        new Transfer(interstellarClock().date(), -money, _id, to, msg));
    _history.add(new Charge(interstellarClock().date(),
                            {(-1.0) * settings.transferCharge(), _currency}));

    _balance -= settings.transferCharge();
  } catch (...) {
    _balance += money.amount();
    throw;
  }
}

void CurrencyAccount::withdraw(Money money) {
  if (money.amount() < 0)
    throw BusinessException("Withdrawing negative amount of money");
  if (money.currency() != currency() && money.currency() != Currency::ENC)
    throw BusinessException("Wrong currency");
  auto local = getBank()->exchangeTable().change(money, currency());
  if (local.amount() > _balance)
    throw BusinessException("Insufficient balance");
  _balance -= local.amount();
  _history.add(new Withdrawal(interstellarClock().date(), -money));
}

#endif
