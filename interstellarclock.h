#ifndef __gsb__interstellar_clock__
#define __gsb__interstellar_clock__

#include <functional>
#include <set>

using Date = unsigned long long;
using Time = unsigned short;

class InterstellarClockObserver {
public:
    virtual void on_month_change() = 0;
};

// Czas międzyplanetarny liczymy z dokładnością do godziny.
// Nowoutworzony zegar międzyplanetarny będzie wskazywał czas 0 i dzień 0.
class InterstellarClock {
    using observer = std::function<void()>;
public:
    using observer_registration = std::set<observer>::iterator;

    // Doba międzyplanetarna ma 20 godzin (0-19).
    static const unsigned HOURS_IN_DAY = 20;

    // Miesiąc międzyplanetarny ma 30 dni.
    static const unsigned DAYS_IN_MONTH = 30;

    virtual ~InterstellarClock() {}

    // Zwraca liczbę godzin od początku założenia ZFP.
    unsigned long long hours() const;

    // Zwraca bieżącą godzinę (0-19).
    Time time() const;

    // Zwraca bieżący datę, tj. dzień od początku założenia ZFP.
    Date date() const;

    // Przesuwa zegar o godzinę.
    virtual InterstellarClock& nextHour();

    // Przesuwa zegar o dobę.
    virtual InterstellarClock& nextDay();

    // Przesuwa zegar o miesiąc (miesiąc ma 30 dni).
    virtual InterstellarClock& nextMonth();

    void registerMonthChangeObserver(InterstellarClockObserver *observer);

private:
    // liczba godzin od początku założenia ZFP
    unsigned long long _hours;

    // List of observers (we store function list we have to call on month change)
    std::set<InterstellarClockObserver*> _on_month_change;
};

InterstellarClock & interstellarClock();

#endif /* defined(__gsb__interstellar_clock__) */
