#ifndef _MATCHMAKER_H_
#define _MATCHMAKER_H_

#include "provided.h"
#include <vector>
class MemberDatabase;
class AttributeTranslator;
using namespace std;

class MatchMaker {
    public:
        MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at);
        ~MatchMaker();
        vector<EmailCount> IdentifyRankedMatches(string email, int threshold) const;
    private:
        MemberDatabase m_mdb;
        AttributeTranslator m_at;

};