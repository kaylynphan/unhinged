#include "AttributeTranslator.h"
#include <fstream>
#include <iostream>

using namespace std;

AttributeTranslator::AttributeTranslator()
	: m_translationsRadixTree(new RadixTree<vector<AttValPair>*>), m_sources(new set<string>)
{}

AttributeTranslator::~AttributeTranslator() {
	for (set<string>::iterator it = m_sources->begin(); it != m_sources->end(); it++) {
		vector<AttValPair>** compatiblePairsVector = m_translationsRadixTree->search(*it);
		if (compatiblePairsVector != nullptr) { // always true
			delete (*compatiblePairsVector);
		}
	}
	delete m_translationsRadixTree;
	delete m_sources;
}

bool AttributeTranslator::Load(string filename) {
	ifstream infile(filename);
	if (!infile) {
		return false;
	}

	if (infile.is_open()) {
		string att1, val1, att2, val2, skip;
		while (infile.good()) {
			getline(infile, att1, ',');
			getline(infile, val1, ','); 
			getline(infile, att2, ',');
			getline(infile, val2);

            // reminder: Radix Tree m_translationsRadixTree uses [source attribute][source value] (concatenated string) as a key
            string key = att1 + val1;
            // see if the source pair is already in the translations RadixTree
			vector<AttValPair>** compatiblePairsVector = m_translationsRadixTree->search(key);
			if (compatiblePairsVector == nullptr) {
				vector<AttValPair>* toInsert = new vector<AttValPair>;
				m_translationsRadixTree->insert(key, toInsert);
				compatiblePairsVector = m_translationsRadixTree->search(key);

                //new unique source, insert into m_sources
				m_sources->insert(key);
			}
            // at this point, the key must be in the Radix Tree
            AttValPair compatiblePair(att2, val2);
			(*compatiblePairsVector)->push_back(compatiblePair);
		}
	}
	return true;
}

vector<AttValPair> AttributeTranslator::FindCompatibleAttValPairs(const AttValPair& source) const {
	// reminder: keys are [source attribute][source value] (two concatenated strings)
	string key = source.attribute + source.value;
	vector<AttValPair>** compatibleAttValPairs = m_translationsRadixTree->search(key);
	if (compatibleAttValPairs != nullptr) {
		return **compatibleAttValPairs;
	}
	return vector<AttValPair>();
}

// FOR DEBUGGING
/*
string AttributeTranslator::toString() {
    string result = "";
    map<string, vector<AttValPair>**> mp = m_translationsRadixTree->getMap();
    for (auto it = mp.begin(); it != mp.end(); it++) {
        result += "Source: " + it->first + '\n';
		result += "Compatible:\n";
        vector<AttValPair> compatible = **(it->second);
		for (int i = 0; i < compatible.size(); i++) {
			result += compatible[i].attribute + compatible[i].value + "\n";
		}
    }
    return result;  
}
*/
