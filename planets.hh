#ifndef _GSB_PLANETS_HH_
#define _GSB_PLANETS_HH_

#include "beings.hh"
#include <exception>
#include <map>
#include <vector>


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
class SimplePlanetMixin : Planet {
protected:
	std::map<id_type, BeingT*> registered_beings;
	id_type counter = 0;
public:
	virtual ~SimplePlanetMixin() {
		for(auto p : registered_beings) {
			delete p.second;
		}
	}

	virtual BeingT & registerCitizen(std::string name) {
		auto id = counter++;
		return *(registered_beings[id] = new BeingT(id, name));
	}

	virtual BeingT & findCitizen(id_type id) {
		auto it = registered_beings.find(id);
		if(it != registered_beings.end())
			return *(it->second);
		else
			throw beingNotFoundException();
	}
};

class Earth : public SimplePlanetMixin<Human> {
};

class Kronos : public SimplePlanetMixin<Klingon> {
};
class Bajnaus : public Planet {
protected:
	std::map<id_type, Binarius *> registered_beings;
	id_type counter = 0;
public:
	~Bajnaus() {
		for(auto p : registered_beings) {
			delete p.second;
		}
	}

	virtual Binarius & registerCitizen(std::string name) {
		auto id = counter++;
		return *(registered_beings[id] = new SingleBinarius(id, name));
	}

	virtual Binarius & registerCitizen(Binarius & b0, Binarius & b1) {
		auto id = counter++;
		return *(registered_beings[id] = new CompositeBinarius(id, b0, b1));
	}

	virtual Binarius & findCitizen(id_type id) {
		auto it = registered_beings.find(id);
		if(it != registered_beings.end())
			return *(it->second);
		else
			throw beingNotFoundException();
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
