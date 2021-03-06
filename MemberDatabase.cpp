#include "MemberDatabase.h"
#include "provided.h"
#include <fstream>
#include <iostream>

using namespace std;

MemberDatabase::MemberDatabase()
	: m_profilesRadixTree(new RadixTree<PersonProfile*>), m_attValToEmailRadixTree(new RadixTree<vector<string>*>),
	m_emailSet(new set<string>), m_attValStringSet(new set<string>) {}

MemberDatabase::~MemberDatabase() {
	for (set<string>::iterator it = m_emailSet->begin(); it != m_emailSet->end(); it++) {
		PersonProfile** pp = m_profilesRadixTree->search(*it);
		if (pp != nullptr) { //always true
			delete (*pp);
		}
	}
	for (set<string>::iterator it = m_attValStringSet->begin(); it != m_attValStringSet->end(); it++) {
		vector<string>** emails = m_attValToEmailRadixTree->search(*it);
		if (emails != nullptr) { // always true
			delete (*emails);
		}
	}
	delete m_profilesRadixTree;
	delete m_attValToEmailRadixTree;
	delete m_emailSet;
	delete m_attValStringSet;
}

bool MemberDatabase::LoadDatabase(string filename) {
	ifstream infile(filename);
	if (!infile) {
		return false;
	}
	if (infile.is_open()) {
		string name;
		string email; 
		string emptyLine;
		int numAVPairs = 0;
		while (infile.good()) {
			getline(infile, name);
			getline(infile, email);
			// do i need to check for duplicate emails?
			if (m_profilesRadixTree->search(email) != nullptr) {
				return false;
			}
			m_emailSet->insert(email);
			PersonProfile* toAdd = new PersonProfile(name, email);
			infile >> numAVPairs;
			getline(infile, emptyLine);
			for (int i = 0; i != numAVPairs; i++) {
				string att;
				string val;
				getline(infile, att, ',');
				getline(infile, val);
				toAdd->AddAttValPair(AttValPair(att, val));

                // now insert the email into vectors, where each vector is mapped to by each AVPair
				string attValKey = att + val;
				vector<string>** emailsVector = m_attValToEmailRadixTree->search(attValKey);
                // if such AVPair does not yet have a vector, create a new vector
				if (emailsVector == nullptr) {
					vector<string>* newVector = new vector<string>();
					m_attValToEmailRadixTree->insert(attValKey, newVector);
					emailsVector = m_attValToEmailRadixTree->search(attValKey);
					m_attValStringSet->insert(attValKey);
				}
                // because we process each profile once, we can guarantee there will be no duplicate emails here
				(*emailsVector)->push_back(email);
			}
			m_profilesRadixTree->insert(email, toAdd);
			getline(infile, emptyLine);
		}
	}
	return true;
}

vector<string> MemberDatabase::FindMatchingMembers(const AttValPair& input) const {
	string attValKey = input.attribute + input.value;
	vector<string>** matches = m_attValToEmailRadixTree->search(attValKey);
	if (matches != nullptr) {
		return **matches;
	}
	return vector<string>();
}

const PersonProfile* MemberDatabase::GetMemberByEmail(string email) const {
	PersonProfile** profile = m_profilesRadixTree->search(email);
	if (profile != nullptr) {
		return *profile;
	}
	return nullptr;
}

