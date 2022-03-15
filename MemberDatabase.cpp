#include "MemberDatabase.h"
#include "provided.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

MemberDatabase::MemberDatabase()
	: m_rtreeEmailToProfile(new RadixTree<PersonProfile*>), m_rtreeAttValToEmails(new RadixTree<vector<string>*>),
	m_emailSet(new set<string>), m_attvalSet(new set<string>)
{}

MemberDatabase::~MemberDatabase()
{
	for (auto it = m_emailSet->begin(); it != m_emailSet->end(); it++) {
		PersonProfile** pp = m_rtreeEmailToProfile->search(*it);
		if (pp != nullptr) { //always true
			delete (*pp);
		}
	}
	for (auto it = m_attvalSet->begin(); it != m_attvalSet->end(); it++) {
		vector<string>** emails = m_rtreeAttValToEmails->search(*it);
		if (emails != nullptr) { // always true
			delete (*emails);
		}
	}
	delete m_rtreeEmailToProfile;
	delete m_rtreeAttValToEmails;
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
			if (m_rtreeEmailToProfile->search(email) != nullptr) {
				return false;
			}
			m_emailSet->insert(email);
			PersonProfile* ppToAdd = new PersonProfile(name, email);
			infile >> numAVPairs;
			getline(infile, emptyLine);
			for (int i = 0; i != numAVPairs; i++) {
				string att;
				string val;
				getline(infile, att, ',');
				getline(infile, val);
				ppToAdd->AddAttValPair(AttValPair(att, val));

				string attValKey = att + val;
				vector<string>** emailVec = m_rtreeAttValToEmails->search(attValKey);
				if (emailVec == nullptr) {
					vector<string>* newVector = new vector<string>();
					m_rtreeAttValToEmails->insert(attValKey, newVector);
					emailVec = m_rtreeAttValToEmails->search(attValKey);
					m_attvalSet->insert(attValKey);
				}
				(*emailVec)->push_back(email);
			}
			m_rtreeEmailToProfile->insert(email, ppToAdd);
			getline(infile, emptyLine);
		}
	}
	
	return true;
}

vector<string> MemberDatabase::FindMatchingMembers(const AttValPair& input) const
{
	string attValKey = input.attribute + input.value;
	vector<string>** matches = m_rtreeAttValToEmails->search(attValKey);
	if (matches != nullptr) {
		return **matches;
	}
	return vector<string>();
}

const PersonProfile* MemberDatabase::GetMemberByEmail(string email) const
{
	PersonProfile** profile = m_rtreeEmailToProfile->search(email);
	if (profile != nullptr) {
		return *profile;
	}
	return nullptr;
}

// FOR DEBUGGING
/*
const string MemberDatabase::toString() {
	string result = "";
    map<string, PersonProfile**> mp = m_rtreeEmailToProfile->getMap();
    for (auto it = mp.begin(); it != mp.end(); it++) {
        result += it->first + '\n';
        result += (*(it->second))->toString();
    }
    return result;
}
*/