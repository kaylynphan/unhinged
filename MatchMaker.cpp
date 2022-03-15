#include "MatchMaker.h"
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include "utility.h"

using namespace std;

MatchMaker::MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at)
	: m_mdb(&mdb), m_at(&at) {}

MatchMaker::~MatchMaker() {}

vector<EmailCount> MatchMaker::IdentifyRankedMatches(string email, int threshold) const {
	const PersonProfile* inputProfile = m_mdb->GetMemberByEmail(email);
	
	unordered_set<AttValPair, AVPairHash> allCompatiblePairsHashSet;
    // for each AttValPair in the profile
	for (int i = 0; i != inputProfile->GetNumAttValPairs(); i++) {
		AttValPair sourceAttVal;
		inputProfile->GetAttVal(i, sourceAttVal); // this passes sourceAttVal by reference
        // obtain a vector of compatible AttValPairs
		vector<AttValPair> compatiblesToThisSource = m_at->FindCompatibleAttValPairs(sourceAttVal);
        // compatiblesToThisSource is a vector with compatible AttValPairs obtained from translations
		for (vector<AttValPair>::iterator it = compatiblesToThisSource.begin(); it != compatiblesToThisSource.end(); it++) {
            // unordered_set<Object, Hash>::set() takes in an object, hashes it using the Hash, and places it in the right bucket
			allCompatiblePairsHashSet.insert(*it);
            // allCompatiblePairsHashSet contains all *UNIQUE* AttValPairs that are compatible with the user's
		}
	}

    // use a hashmap to map an email(string) to a number of matches(int)
	unordered_map<string, int> emailsToNumMatchesHashMap;
    // for each AttValPair to look through
	for (unordered_set<AttValPair, AVPairHash>::iterator it = allCompatiblePairsHashSet.begin(); it != allCompatiblePairsHashSet.end(); it++) {
		vector<string> emailsWithThisPair = m_mdb->FindMatchingMembers(*it);
        // for each email under this AttValPair
		for (vector<string>::iterator it1 = emailsWithThisPair.begin(); it1 != emailsWithThisPair.end(); it1++) {
            // make sure you don't match a member with themselves
            if (*it1 != inputProfile->GetEmail()) {
                // increase the number of matches
                emailsToNumMatchesHashMap[*it1]++;
            }
		}
	}

    // a set ensures that matches will be placed in order
    set<EmailCount> matches;
    for (unordered_map<string, int>::iterator it = emailsToNumMatchesHashMap.begin(); it != emailsToNumMatchesHashMap.end(); it++) {
        if (it->second >= threshold) {
            matches.insert(EmailCount(it->first, it->second));
        }
    }

    vector<EmailCount> rankedMatches;
    for (set<EmailCount>::iterator it = matches.begin(); it != matches.end(); it++) {
        rankedMatches.push_back(*it);
    }

    /*
	vector<EmailCount> rankedMatches;
	for (unordered_map<string, int>::iterator it = emailsToNumMatchesHashMap.begin(); it != emailsToNumMatchesHashMap.end(); it++) {
		if (it->second >= threshold) {
            rankedMatches.push_back(EmailCount(it->first, it->second));
        }
	}
	sort(rankedMatches.begin(), rankedMatches.end(), comp);
    */

	return rankedMatches;
}