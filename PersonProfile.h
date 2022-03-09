#ifndef _PERSON_PROFILE_H_
#define _PERSON_PROFILE_H_

#include "provided.h"
#include <vector>
using namespace std;

class PersonProfile {
    public:
        PersonProfile(string name, string email);
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
        vector<AttValPair> m_attValPairs;
};

#endif