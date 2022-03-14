#include "provided.h"
#include "RadixTree.h"
#include "MemberDatabase.h"
#include <set> 
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

MemberDatabase::MemberDatabase() {
    m_userDatabase = new RadixTree<PersonProfile*>();
    m_usersWithAttValPair = new RadixTree<set<string>*>;
}

MemberDatabase::~MemberDatabase() {
    delete m_userDatabase;
    delete m_usersWithAttValPair;
}

void MemberDatabase::addToAttributeSet(AttValPair attValPair, string email) {
    //string key will be "attribute, value"
    string key = attValPair.attribute + ", " + attValPair.value;
    set<string>** emailSet = m_usersWithAttValPair->search(key);
    if (emailSet == nullptr) {
        m_usersWithAttValPair->insert(key, new set<string>());
        emailSet = m_usersWithAttValPair->search(key);
    }
    (*emailSet)->insert(email);
}

bool MemberDatabase::LoadDatabase(string filename) {
    ifstream infile(filename);
    if (!infile) {
        return false;
    }
    while (true) {
        string name;
        getline(infile, name);
        string email;
        getline(infile, email);
        int numAVPairs = 0;
        string line;
        getline(infile, line);
        istringstream iss(line);
        PersonProfile* toAdd = new PersonProfile(name, email, numAVPairs);
        iss >> numAVPairs;
        iss.ignore(1000, '\n');
        for (int i = 0; i < numAVPairs; i++) {
            getline(infile, line);
            istringstream iss(line);
            string att;
            string val;
            getline(iss, att, ',');
            getline(iss, val);
            AttValPair avpair(att, val);
            toAdd->AddAttValPair(avpair);
            addToAttributeSet(avpair, email);
        }
        // add <email, Profile> to database
        m_userDatabase->insert(email, toAdd);
        string emptyLine;
        if (!getline(infile, emptyLine)) {
            break;
        }
    }
    return true;
}

vector<string> MemberDatabase::FindMatchingMembers(const AttValPair& input) const {
    vector<string> result;
    string key = input.attribute + ", " + input.value;
    set<string>** emailSet = m_usersWithAttValPair->search(key);
    if (emailSet != nullptr) {
        for (auto it = (**emailSet).begin(); it != (**emailSet).end(); it++) {
            result.push_back(*it);
        }
    } 
    return result;
}

const PersonProfile* MemberDatabase::GetMemberByEmail(string email) const {
    return *(m_userDatabase->search(email));
}

//prints the whole database
const string MemberDatabase::toString() {
    string result = "";
    map<string, PersonProfile**> mp = m_userDatabase->getMap();
    for (auto it = mp.begin(); it != mp.end(); it++) {
        result += it->first + '\n';
        result += (*(it->second))->toString();
    }
    return result;
}