#ifndef _MATCHMAKER_H_
#define _MATCHMAKER_H_

#include "MemberDatabase.h"
#include "AttributeTranslator.h"

using namespace std;

class MatchMaker {
public:
	MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at);
	~MatchMaker();
	vector<EmailCount> IdentifyRankedMatches(string email, int threshold) const;
private:
	const MemberDatabase* m_mdb;
	const AttributeTranslator* m_at;
};


#endif