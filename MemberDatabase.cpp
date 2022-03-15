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
	/*
	ifstream infile(filename);
    if (!infile) {
        return false;
    }
    bool keepReading = true;
    while (keepReading) {
        string name;
        getline(infile, name);
        string email;

		if (m_rtreeEmailToProfile->search(email) != nullptr) {
			return false;
		}
		m_emailSet->insert(email);

        getline(infile, email);
        int numAVPairs = 0;
        string line;
        getline(infile, line);
        istringstream iss(line);
        PersonProfile* toAdd = new PersonProfile(name, email);
        iss >> numAVPairs;
        iss.ignore(1000, '\n');
        for (int i = 0; i < numAVPairs; i++) {
            getline(infile, line);
            istringstream iss(line);
            string att;
            string val;
            getline(iss, att, ',');
            getline(iss, val, ',');
			toAdd->AddAttValPair(AttValPair(att, val));
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
        m_rtreeEmailToProfile->insert(email, toAdd);
        string emptyLine;
        keepReading = static_cast<bool>(getline(infile, emptyLine));
    }
	*/

	
	ifstream infile(filename);
	if (!infile) {
		return false;
	}
	if (infile.is_open()) {
		string name, email, att, val, skip;
		int attvalCount = 0;
		while (infile.good()) {
			getline(infile, name);
			getline(infile, email);
			if (m_rtreeEmailToProfile->search(email) != nullptr) {
				return false;
			}
			m_emailSet->insert(email);
			PersonProfile* ppToAdd = new PersonProfile(name, email);
			infile >> attvalCount;
			getline(infile, skip);
			for (int i = 0; i != attvalCount; i++) {
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
			getline(infile, skip);
			m_rtreeEmailToProfile->insert(email, ppToAdd);
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