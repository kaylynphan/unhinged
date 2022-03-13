#ifndef _ATTRIBUTE_TRANSLATOR_H_
#define _ATTRIBUTE_TRANSLATOR_H_

#include "provided.h"
#include <map>
#include <vector>

using namespace std;

class AttributeTranslator {
    public:
        AttributeTranslator();
        ~AttributeTranslator();
        bool Load(string filename);
        vector<AttValPair> FindCompatibleAttValPairs(const AttValPair& source) const;
        // MOVE TO PRIVATE WHEN DONE USING
        std::string toString();
    private:
        map<AttValPair, vector<AttValPair>> m_translations;
};

#endif