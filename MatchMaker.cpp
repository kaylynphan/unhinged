#include "provided.h"
#include "MatchMaker.h"
#include <vector>

using namespace std;

MatchMaker::MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at) {
    m_mdb = mdb;
    m_at = at;
}

MatchMaker::~MatchMaker() {
    // I suspect there will be nothing in here
}

vector<EmailCount> MatchMaker::IdentifyRankedMatches(string email, int threshold) const {
    // to do
}