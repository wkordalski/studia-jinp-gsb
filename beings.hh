#ifndef _GSB_BEINGS_HH_
#define _GSB_BEINGS_HH_

#include <string>
#include <vector>

using id_type = std::vector<int>::size_type;

class Being {
	protected:
		id_type _id;
		std::string _name;
	public:
		Being(id_type id, std::string name) : _id(id), _name(name) {}
		id_type id() { return _id; }
		std::string name() { return _name; }

};

class Human : public Being {
public:
	Human(id_type id, std::string name) : Being(id, name) {}
};

class Klingon : public Being {
public:
	Klingon(id_type id, std::string name) : Being(id, name) {}
};

class Binarius : public Being { //component
public:
	Binarius(id_type id, std::string name) : Being(id, name) {}
};

class BinariusComposite : public Binarius { //composite
	public:
		BinariusComposite(id_type id, Binarius &b1, Binarius &b2) :
			Binarius(id, b1.name() + "&" + b2.name()) {}
			
};

class BinariusLeaf : public Binarius { //leaf
};

#endif /* ! _GSB_BEINGS_HH_ */