#ifndef _GSB_BEINGS_HH_
#define _GSB_BEINGS_HH_

#include <cstddef>
#include <string>
#include <vector>

using id_type = size_t;

class Being {
	protected:
		id_type _id;
	public:
		Being(id_type id) : _id(id) {}
		virtual ~Being() {}
		id_type id() { return _id; }
		virtual std::string name() const = 0;

};

class Human : public Being {
	std::string _name;
public:
	Human(id_type id, std::string name) : Being(id), _name(name) {}

	virtual std::string name() const { return _name; }
};

class Klingon : public Being {
	std::string _name;
public:
	Klingon(id_type id, std::string name) : Being(id), _name(name) {}

	virtual std::string name() const { return _name; }
};

class Binarius : public Being { //component
public:
	Binarius(id_type id) : Being(id) {}
	~Binarius() {}
};

class CompositeBinarius : public Binarius { //composite
	Binarius &b0, &b1;
public:
	CompositeBinarius(id_type id, Binarius &b0, Binarius &b1) :
		Binarius(id), b0(b0), b1(b1) {}

	virtual std::string name() const { return b0.name() + "&" + b1.name(); }
};

class SingleBinarius : public Binarius { //leaf
	std::string _name;
public:
	SingleBinarius(id_type id, std::string name) : Binarius(id), _name(name) {}

	virtual std::string name() const { return _name; }
};

#endif /* ! _GSB_BEINGS_HH_ */
