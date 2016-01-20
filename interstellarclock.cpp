#include "interstellarclock.h"

unsigned long long InterstellarClock::hours() const {
    return _hours;
}

Time InterstellarClock::time() const {
    return _hours % HOURS_IN_DAY;
}

Date InterstellarClock::date() const {
    return _hours / HOURS_IN_DAY;
}

InterstellarClock& InterstellarClock::nextHour() {
    _hours++;
    if(_hours % (HOURS_IN_DAY * DAYS_IN_MONTH) == 0) {
      for(auto f : _on_month_change) f(*this);
    }
    return *this;
}

InterstellarClock& InterstellarClock::nextDay() {
    for (Time i = 0; i < HOURS_IN_DAY; i++)
        nextHour();
    return *this;
}

InterstellarClock& InterstellarClock::nextMonth() {
    for (Date i = 0; i < DAYS_IN_MONTH; i++)
        nextDay();
    return *this;
}

void InterstellarClock::registerMonthChangeObserver(std::function<void(InterstellarClock&)> observer) {
  _on_month_change.push_back(observer);
}
