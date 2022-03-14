#ifndef _MEMBER_DATABASE_H_
#define _MEMBER_DATABASE_H_

#include "provided.h"
#include "RadixTree.h"
#include "PersonProfile.h"
#include <vector>

using namespace std;


class MemberDatabase {
    public:
        MemberDatabase();
        ~MemberDatabase();
        bool LoadDatabase(string filename);
        vector<string> FindMatchingMembers(const AttValPair& input) const;
        const PersonProfile* GetMemberByEmail(string email) const;
        // MOVE THIS TO PRIVATE WHEN DONE
        const string toString();
    private:
        // maps emails (string) to Profiles
        RadixTree<PersonProfile*>* m_userDatabase;
        // for TRAIT: maps value (string) to emails ()
        RadixTree<set<string>*>* m_usersWithAttValPair;
        void addToAttributeSet(AttValPair attValPair, string email);
};

#endif