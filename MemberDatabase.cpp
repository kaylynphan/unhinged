#include "provided.h"
#include "MemberDatabase.h"
#include <map> //get rid of this later
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

MemberDatabase::MemberDatabase() {
    // we may have to do nothing here
}

MemberDatabase::~MemberDatabase() {
    // we may have to do nothing here
}

bool MemberDatabase::LoadDatabase(string filename) {
    ifstream infile(filename);
    if (!infile) {
        return false;
    }
    bool keepReading = true;
    while (keepReading) {
        string name;
        getline(infile, name);
        string email;
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
        }
        m_database.insert(pair<string, PersonProfile*>(email, toAdd));
        string emptyLine;
        keepReading = getline(infile, emptyLine);
    }
    return true;
}

vector<string> MemberDatabase::FindMatchingMembers(const AttValPair& input) const {
    vector<string> result;
    return result;
}

const PersonProfile* MemberDatabase::GetMemberByEmail(string email) const {
    return (m_database.find(email))->second;
}

//prints the whole database
const string MemberDatabase::toString() {
    map<string, PersonProfile*>::iterator it = m_database.begin();
    string result = "";
    while (it != m_database.end()) {
        result += "Key: " + it->first + '\n';
        result += "Value:\n";
        // print the PersonProfile
        result += (it->second)->toString();
        it++;
    }
    return result;
}