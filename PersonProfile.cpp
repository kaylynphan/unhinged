#include "PersonProfile.h"
#include "provided.h"

using namespace std;

PersonProfile::PersonProfile(string name, string email)
	: m_name(name), m_email(email), m_numAVPairs(0), m_attValRadixTree(new RadixTree<set<string> >), m_attValVector(vector<AttValPair*>()) {}

PersonProfile::~PersonProfile() {
	// use the vector to 'iterate' through the RadixTree
	for (vector<AttValPair*>::iterator it = m_attValVector.begin(); it != m_attValVector.end(); it++) {
		set<string>* attSet = m_attValRadixTree->search((*it)->value);
		if (attSet != nullptr) { 
			delete attSet;
		}
		delete (*it);
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
	set<string>* attSet = m_attValRadixTree->search(attval.attribute);
	if (attSet == nullptr) {
		set<string> newSet;
		m_attValRadixTree->insert(attval.attribute, newSet);
		attSet = m_attValRadixTree->search(attval.attribute);
	}
	// at this point, the attribute must be in the Radix Tree
	// set::find() returns set::end() if the item is not found
	if (attSet->find(attval.value) == attSet->end()) {
		set<string>* newSet = new set<string>(*attSet);
		newSet->insert(attval.value);
		// replace the set in the radix tree via insertion
		m_attValRadixTree->insert(attval.attribute, *newSet);
		m_attValVector.push_back(new AttValPair(attval));
		m_numAVPairs++;
		delete newSet;
	}
}

int PersonProfile::GetNumAttValPairs() const
{
	return m_numAVPairs;
}

bool PersonProfile::GetAttVal(int attribute_num, AttValPair& attval) const
{
	if (attribute_num < 0 || attribute_num >= GetNumAttValPairs()) {
		return false;
	}
	attval = *m_attValVector[attribute_num];
	return true;
}

// FOR DEBUGGING
/*
const string PersonProfile::toString() {
    string result = m_name + '\n';
    auto it = m_attValVector.begin();
    while (it != m_attValVector.end()) {
        result += "Attribute: " + (*it)->attribute;
        result += " Value: " + (*it)->value + '\n';
        it++;
    }
    return result;
}
*/