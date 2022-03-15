#ifndef _MEMBER_DATABASE_H_
#define _MEMBER_DATABASE_H_

#include <vector>
#include <set>
#include "PersonProfile.h"
#include "RadixTree.h"

using namespace std;

struct AttValPair;

class MemberDatabase {
public:
	MemberDatabase();
	~MemberDatabase();
	bool LoadDatabase(string filename);
	vector<string> FindMatchingMembers(const AttValPair& input) const;
	const PersonProfile* GetMemberByEmail(string email) const;
	const string toString();
private:
	RadixTree<PersonProfile*>* m_rtreeEmailToProfile;
	RadixTree<vector<string>*>* m_rtreeAttValToEmails;
	// had trouble deleting from the RadixTrees, a set helped
	set<string>* m_emailSet;
	set<string>* m_attvalSet;
};


#endif