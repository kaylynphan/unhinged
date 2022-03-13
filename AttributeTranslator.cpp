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
            map<AttValPair, vector<AttValPair>>::iterator it = m_translations.find(avPair1);
            if (it != m_translations.end()) {
                it->second.push_back(avPair2);
            } else {
                vector<AttValPair> newvector;
                newvector.push_back(avPair2);
                m_translations.insert(pair<AttValPair, vector<AttValPair>>(avPair1, newvector));
            }
        } else {
            cout << "Incomplete line" << endl;
            break;
        }
    }
    return true;
}

vector<AttValPair> AttributeTranslator::FindCompatibleAttValPairs(const AttValPair& source) const {
    return m_translations.find(source)->second;
}

string AttributeTranslator::toString() {
    string result = "";
    map<AttValPair, vector<AttValPair>>::iterator it = m_translations.begin();
    while (it != m_translations.end()) {
        result += "Source:\n" + it->first.attribute + ": " + it->first.value + "\n";
        result += "Matches\n";
        vector<AttValPair>::iterator match = it->second.begin();
        while (match != it->second.end()) {
            result += match->attribute + ": " + match->value + "\n";
            match++;
        }
        result += "\n";
        it++;
    }
    return result;
    
}