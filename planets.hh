#ifndef _GSB_PLANETS_HH_
#define _GSB_PLANETS_HH_

#include "beings.hh"
#include <exception>
#include <map>
#include <vector>

class beingNotFoundException : public std::exception {
	virtual const char * what() { return "Begin not found"; }
};

class Planet {
	virtual Being & registerCitizen(std::string name) = 0;
	virtual Being & findCitizen(std::string name) {
		// TODO: sprawdzanie czy istnieje
		auto it = id_by_name.find(name);
		if (it != id_by_name.end())
			return findCitizen(id_by_name[name]);
		else
			throw beingNotFoundException();
	}
	virtual Being & findCitizen(id_type id) = 0;
	
	std::map<std::string, id_type> id_by_name;
};

template<typename BeingT>
class PlanetMixin : Planet {
public:
	std::vector<BeingT> registered_beings;
	virtual BeingT & registerCitizen(std::string name) {
		auto id = registered_beings.size();
		registered_beings.push_back(BeingT(id, name));
		return registered_beings[id];
	}
	
	virtual BeingT & findCitizen(id_type id) {
		if(id > registered_beings.size()) throw 0;	// TODO
		return registered_beings[id];
	}
};

class Earth : public PlanetMixin<Human> {
};

class Kronos : public PlanetMixin<Klingon> {
};
class Bajnaus : public PlanetMixin<Binarius> {
	virtual Binarius & registerCitizen(Binarius & b0, Binarius & b1) {
		auto id = registered_beings.size();
		registered_beings.push_back(BinariusComposite(id, b0, b1));
		return registered_beings[id];
	}
};

Earth & earth() {
	static Earth instance;
	return instance;
}

Kronos & qonos() {
	static Kronos instance;
	return instance;
}

Bajnaus & bynaus() {
	static Bajnaus instance;
	return instance;
}

#endif /* ! _GSB_PLANETS_HH_ */