#define _GSB_OBSERVER_HH_
#ifndef _GSB_OBSERVER_HH_

template<typename Subject>
class IObserver {
public:
	virtual void notify(Subject &s) = 0;
};

template<typename Subject, typename EventTypeType>
class ObservableMixin {
protected:
	std::map<EventTypeType, std::set<IObserver<Subject>&>> observers;
	
public:
	void register_observer(IObserver &observer, EventTypeType type) {
		observers.at(type).insert(observer);
	}
	
	void notify_observers(EventTypeType type) {
		auto &obs = observers.at(type);
		for(auto & ob : obs)
			ob.notify(*this);
	}
};

#endif /* ! _GSB_OBSERVER_HH_ */