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
public:
	virtual Being & registerCitizen(std::string name) = 0;
	virtual Being & findCitizen(std::string name) {
		auto it = id_by_name.find(name);
		if (it != id_by_name.end())
			return findCitizen(id_by_name[name]);
		else
			throw beingNotFoundException();
	}
	virtual Being & findCitizen(id_type id) = 0;
	
protected:
	std::map<std::string, id_type> id_by_name;
};

template<typename BeingT>
class PlanetMixin : Planet {
protected:
	std::map<id_type, BeingT> registered_beings;
	id_type counter = 0;
public:
	virtual BeingT & registerCitizen(std::string name) {
		auto id = counter++;
		return registered_beings[id] = BeingT(id, name);
	}
	
	virtual BeingT & findCitizen(id_type id) {
		auto it = registered_beings.find(id);
		if(it != registered_beings.end())
			return it->second;
		else
			throw beingNotFoundException();
	}
};

class Earth : public PlanetMixin<Human> {
};

class Kronos : public PlanetMixin<Klingon> {
};
class Bajnaus : public PlanetMixin<Binarius> {
public:
	virtual Binarius & registerCitizen(std::string name) {
		auto id = counter++;
		return registered_beings[id] = SingleBinarius(id, name);
	}
	
	virtual Binarius & registerCitizen(Binarius & b0, Binarius & b1) {
		auto id = counter++;
		return registered_beings[id] = BinariusComposite(id, b0, b1);
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