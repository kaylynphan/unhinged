#include "MemberDatabase.h"
#include "provided.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

MemberDatabase::MemberDatabase()
	: m_profilesRadixTree(new RadixTree<PersonProfile*>), m_attValToEmailRadixTree(new RadixTree<vector<string>*>),
	m_emailSet(new set<string>), m_attvalSet(new set<string>)
{}

MemberDatabase::~MemberDatabase()
{
	for (auto it = m_emailSet->begin(); it != m_emailSet->end(); it++) {
		PersonProfile** pp = m_profilesRadixTree->search(*it);
		if (pp != nullptr) { //always true
			delete (*pp);
		}
	}
	for (auto it = m_attvalSet->begin(); it != m_attvalSet->end(); it++) {
		vector<string>** emails = m_attValToEmailRadixTree->search(*it);
		if (emails != nullptr) { // always true
			delete (*emails);
		}
	}
	delete m_profilesRadixTree;
	delete m_attValToEmailRadixTree;
	delete m_emailSet;
	delete m_attvalSet;
}

bool MemberDatabase::LoadDatabase(string filename)
{
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
					m_attvalSet->insert(attValKey);
				}
                // because we process each profile once, we can guarantee there will be no duplicate emails here
                // however, duplicates may appear when a member is matched twice to another member because they are compatible in more than one way
                // this case will be handled in MatchMaker.cpp
				(*emailsVector)->push_back(email);
			}
			m_profilesRadixTree->insert(email, toAdd);
			getline(infile, emptyLine);
		}
	}
	return true;
}

vector<string> MemberDatabase::FindMatchingMembers(const AttValPair& input) const
{
	string attValKey = input.attribute + input.value;
	vector<string>** matches = m_attValToEmailRadixTree->search(attValKey);
	if (matches != nullptr) {
		return **matches;
	}
	return vector<string>();
}

const PersonProfile* MemberDatabase::GetMemberByEmail(string email) const
{
	PersonProfile** profile = m_profilesRadixTree->search(email);
	if (profile != nullptr) {
		return *profile;
	}
	return nullptr;
}

// FOR DEBUGGING
/*
const string MemberDatabase::toString() {
	string result = "";
    map<string, PersonProfile**> mp = m_profilesRadixTree->getMap();
    for (auto it = mp.begin(); it != mp.end(); it++) {
        result += it->first + '\n';
        result += (*(it->second))->toString();
    }
    return result;
}
*/