#include "provided.h"
#include "PersonProfile.h"
#include "RadixTree.h"
#include <iostream>
#include <set>
#include <vector>

using namespace std;

PersonProfile::PersonProfile(string name, string email, int numAttValPairs) {
    m_name = name;
    m_email = email;
    m_numAttValPairs = numAttValPairs;
    m_attValRadixTree = new RadixTree<set<string> >();
    m_attValVec = vector<AttValPair>();
}

PersonProfile::~PersonProfile() {
    cout << "Calling PersonProfile destructor" << endl;
    for (auto it = m_attValVec.begin(); it != m_attValVec.end(); it++) {
		set<string>* attSet = m_attValRadixTree->search((*it).value);
		if (attSet != nullptr) { // should always be the case
			delete attSet;
		}
		//delete (*it);
	}
	delete m_attValRadixTree;
}

string PersonProfile::GetName() const {
    return m_name;
}

string PersonProfile::GetEmail() const {
    return m_email;
}

void PersonProfile::AddAttValPair(const AttValPair& attval) {
    // Add to RadixTree
    set<string>* foundSet = m_attValRadixTree->search(attval.attribute);
    if (foundSet == nullptr) {
        m_attValRadixTree->insert(attval.attribute, set<string>());
        foundSet = m_attValRadixTree->search(attval.attribute);
    }
    // at this point, there is a set to insert into
    foundSet->insert(attval.value);
    // Add to Vector
    m_attValVec.push_back(attval); // MALLOC
}

int PersonProfile::GetNumAttValPairs() const {
    return m_numAttValPairs;
}

bool PersonProfile::GetAttVal(int attribute_num, AttValPair& attval) const {
    if (attribute_num < m_numAttValPairs && attribute_num >= 0) {
        attval = m_attValVec[attribute_num];
        return true;
    }
    return false;
    //not sure if this is right
}

const string PersonProfile::toString() {
    string result = m_name + '\n';
    vector<AttValPair>::iterator it = m_attValVec.begin();
    while (it != m_attValVec.end()) {
        result += "Attribute: " + (*it).attribute;
        result += " Value: " + (*it).value + '\n';
        it++;
    }
    return result;
}