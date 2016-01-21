#ifndef _GSB_EXCHANGE_TABLE_HH_
#define _GSB_EXCHANGE_TABLE_HH_

#include "money.hh"

#include <exception>
#include <map>

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
  ExchangeTable & exchangeRate(Currency currency) {
    _current_entry = &_map[currency];
    return *this;
  }

  ExchangeTable & buyingRate(double value) {
    if(_current_entry) _current_entry->buy = value;
    else throw std::logic_error("Set currency before setting rates.");
    return *this;
  }
  ExchangeTable & sellingRate(double value) {
    if(_current_entry) _current_entry->sell = value;
    else throw std::logic_error("Set currency before setting rates.");
    return *this;
  }

  // Changes money to different currency using the ExchangeTable.
  Money change(Money money, Currency currency) const {
    // No change needed...
    if(money.currency() == currency) return money;

    auto entry = [this](Currency c) {
      auto it = _map.find(c);
      if(it == _map.end()) return TableEntry();
      else return it->second;
    };

    Money enc = (money.currency() == Currency::ENC)?money:
      Money( money.amount()*entry(money.currency()).sell, Currency::ENC );
    return (currency == Currency::ENC)?enc:
      Money( enc.amount()*entry(currency).buy, currency);
  }
};

#endif /* end of include guard: _GSB_EXCHANGE_TABLE_HH_ */
