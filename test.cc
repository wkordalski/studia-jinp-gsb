/*test.cc*/

#include "planets.hh"
#include "bank_application.hh"

#include <iostream>

using namespace std;

int main() {
	auto & captain = earth().registerCitizen("My name");
	cout << captain.name() << captain.id() << endl;
}
