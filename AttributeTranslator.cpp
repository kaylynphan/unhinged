#include "AttributeTranslator.h"
#include <fstream>
#include <iostream>

using namespace std;

AttributeTranslator::AttributeTranslator()
	: m_rtreePairToPair(new RadixTree<vector<AttValPair>*>), m_sourceAttvalSet(new set<string>)
{}

AttributeTranslator::~AttributeTranslator()
{
	for (auto it = m_sourceAttvalSet->begin(); it != m_sourceAttvalSet->end(); it++) {
		vector<AttValPair>** compatibleVec = m_rtreePairToPair->search(*it);
		if (compatibleVec != nullptr) { // always true
			delete (*compatibleVec);
		}
	}
	delete m_rtreePairToPair;
	delete m_sourceAttvalSet;
}

bool AttributeTranslator::Load(string filename)
{
	ifstream translatorFile(filename);
	if (!translatorFile) {
		return false;
	}

	if (translatorFile.is_open()) {
		string attSRead, valSRead, attCRead, valCRead, skip;
		while (translatorFile.good()) {
			getline(translatorFile, attSRead, ',');
			getline(translatorFile, valSRead, ','); 
			getline(translatorFile, attCRead, ',');
			getline(translatorFile, valCRead);

			string sourcePair(attSRead + valSRead);
			AttValPair compatiblePair(attCRead, valCRead);
			vector<AttValPair>** compatibleVec = m_rtreePairToPair->search(sourcePair);
			if (compatibleVec == nullptr) {
				vector<AttValPair>* toInsert = new vector<AttValPair>;
				m_rtreePairToPair->insert(sourcePair, toInsert);
				compatibleVec = m_rtreePairToPair->search(sourcePair);
				m_sourceAttvalSet->insert(sourcePair);
			}
			(*compatibleVec)->push_back(compatiblePair);
		}
	}
	return true;
}

vector<AttValPair> AttributeTranslator::FindCompatibleAttValPairs(const AttValPair& source) const {
	string sourcePair = source.attribute + source.value;
	vector<AttValPair>** compatibleVec = m_rtreePairToPair->search(sourcePair);
	if (compatibleVec != nullptr) {
		return **compatibleVec;
	}
	return vector<AttValPair>();
}

// FOR DEBUGGING
/*
string AttributeTranslator::toString() {
    string result = "";
    map<string, vector<AttValPair>**> mp = m_rtreePairToPair->getMap();
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
