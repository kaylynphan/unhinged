#ifndef _PERSON_PROFILE_H_
#define _PERSON_PROFILE_H_

#include "provided.h"
#include "RadixTree.h"
#include <vector>
#include <set>

using namespace std;

class PersonProfile {
    public:
        PersonProfile(string name, string email, int numAttValPairs);
        ~PersonProfile();
        string GetName() const;
        string GetEmail() const;
        void AddAttValPair(const AttValPair& attval);
        int GetNumAttValPairs() const;
        bool GetAttVal(int attribute_num, AttValPair& attval) const;
        //MOVE THIS TO PRIVATE WHEN DONE USING
        const string toString();
    private:
        string m_name;
        string m_email;
        int m_numAttValPairs;
        // maps strings (attributes) to sets of strings (sets of values)
        RadixTree<set<string> >* m_attValRadixTree;
        // used solely for indexing
        vector<AttValPair> m_attValVec;
};

#endif