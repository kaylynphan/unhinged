#ifndef _MEMBER_DATABASE_H_
#define _MEMBER_DATABASE_H_

#include "provided.h"
#include "PersonProfile.h"
#include <vector>
#include <map> //get rid of this later

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
        map<string, PersonProfile*> m_database;
        // maps emails to PersonProfiles
};

#endif