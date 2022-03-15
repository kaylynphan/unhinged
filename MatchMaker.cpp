#include "MatchMaker.h"
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include "utility.h"

using namespace std;

MatchMaker::MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at)
	: m_mdb(&mdb), m_at(&at)
{}

MatchMaker::~MatchMaker()
{}

vector<EmailCount> MatchMaker::IdentifyRankedMatches(string email, int threshold) const
{
	//Find PersonProfile associated with Email
	const PersonProfile* ppOfInterest = m_mdb->GetMemberByEmail(email);
	
	//Find all compatible AttValPairs for each of PersonProfile's own, set ensures no duplicates
	unordered_set<AttValPair, AVPairHash> membersToLookFor;
	for (int i = 0; i != ppOfInterest->GetNumAttValPairs(); i++) {
		AttValPair avSource;
		ppOfInterest->GetAttVal(i, avSource);
		vector<AttValPair> compatiblesVec = m_at->FindCompatibleAttValPairs(avSource);
		for (vector<AttValPair>::iterator it = compatiblesVec.begin(); it != compatiblesVec.end(); it++) {
			membersToLookFor.insert(*it);
		}
	}

	unordered_map<string, int> emailToMatchCount;
	for (unordered_set<AttValPair, AVPairHash>::iterator it = membersToLookFor.begin(); it != membersToLookFor.end(); it++) {
		vector<string> emailsWithMatchingPair = m_mdb->FindMatchingMembers(*it);
		for (vector<string>::iterator it1 = emailsWithMatchingPair.begin(); it1 != emailsWithMatchingPair.end(); it1++) {
			if (*it1 != ppOfInterest->GetEmail()) emailToMatchCount[*it1]++;
		}
	}

	vector<EmailCount> rankedMatches;
	for (unordered_map<string, int>::iterator it = emailToMatchCount.begin(); it != emailToMatchCount.end(); it++) {
		if (it->second >= threshold) rankedMatches.push_back(EmailCount(it->first, it->second));
	}

	sort(rankedMatches.begin(), rankedMatches.end(), comp);

	return rankedMatches;
}