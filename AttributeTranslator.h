#ifndef _ATTRIBUTE_TRANSLATOR_H_
#define _ATTRIBUTE_TRANSLATOR_H_

#include <vector>
#include <set>
#include <string>
#include "provided.h"
#include "RadixTree.h"

using namespace std;

class AttributeTranslator {
public:
	AttributeTranslator();
	~AttributeTranslator();
	bool Load(string filename);
	vector<AttValPair> FindCompatibleAttValPairs(const AttValPair& source) const;
private:
	RadixTree<vector<AttValPair>*>* m_translationsRadixTree;
	set<string>* m_sources;
};

#endif