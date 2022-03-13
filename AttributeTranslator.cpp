#include "AttributeTranslator.h"
#include "utility.h"
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

AttributeTranslator::AttributeTranslator() {
    // we may have to do nothing here
}

AttributeTranslator::~AttributeTranslator() {
    // we may have to do nothing here
}

bool AttributeTranslator::Load(string filename) {
    ifstream infile(filename);
    if (!infile) {
        return false;
    }
    string line;
    while (getline(infile, line)) {
        istringstream iss(line);
        string att1;
        string val1;
        string att2;
        string val2;
        getline(iss, att1, ',');
        getline(iss, val1, ',');
        getline(iss, att2, ',');
        getline(iss, val2);
        if ((att1 != "") && (att2 != "") && (val1 != "") && (val2 != "")) {
            AttValPair avPair1(att1, val1);
            AttValPair avPair2(att2, val2);
            m_translations.insert(pair<AttValPair, AttValPair>(avPair1, avPair2));
        } else {
            cout << "Incomplete line" << endl;
            break;
        }
    }
    return true;
}

vector<AttValPair> AttributeTranslator::FindCompatibleAttValPairs(const AttValPair& source) const {
    vector<AttValPair> result;
    return result;
}

string AttributeTranslator::toString() {
    map<AttValPair, AttValPair>::iterator it = m_translations.begin();
    string result = "";
    while (it != m_translations.end()) {
        result += "Source: " + it->first.attribute + ": " + it->first.value + "\n";
        result += "Match: " + it->second.attribute + ": " + it->second.value + "\n";
        it++;
    }
    return result;
    
}