#include "provided.h"
#include "PersonProfile.h"

using namespace std;

PersonProfile::PersonProfile(string name, string email) {
    m_name = name;
    m_email = email;
}

PersonProfile::~PersonProfile() {
    // I suspect that nothing has to be in here
}

string PersonProfile::GetName() const {
    return m_name;
}

string PersonProfile::GetEmail() const {
    return m_email;
}

void PersonProfile::AddAttValPair(const AttValPair& attval) {
    m_attValPairs.push_back(attval);
}

int PersonProfile::GetNumAttValPairs() const {
    return m_attValPairs.size();
}

bool PersonProfile::GetAttVal(int attribute_num, AttValPair& attval) const {
    attval = m_attValPairs[attribute_num];
    return true;
    //not sure if this is right
}

const string PersonProfile::toString() {
    string result = m_name + '\n';
    vector<AttValPair>::iterator it = m_attValPairs.begin();
    while (it != m_attValPairs.end()) {
        result += "Attribute: " + (*it).attribute;
        result += " Value: " + (*it).value + '\n';
        it++;
    }
    return result;
}