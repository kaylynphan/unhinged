#ifndef _PERSON_PROFILE_H_
#define _PERSON_PROFILE_H_

#include <string>
#include "RadixTree.h"
#include <vector>
#include <set>

using namespace std;

struct AttValPair;

class PersonProfile {
public:
	PersonProfile(string name, string email);
	~PersonProfile();
	string GetName() const;
	string GetEmail() const;
	void AddAttValPair(const AttValPair& attval);
	int GetNumAttValPairs() const;
	bool GetAttVal(int attribute_num, AttValPair& attval) const;
	//FOR DEBUGGING
	//const string toString();
private:
	string m_name;
	string m_email;
	int m_numAVPairs;
	// maps attribute(string) to set of values(set<string>)
	RadixTree<set<string> >* m_attValRadixTree;
	// used solely for indexing
	vector<AttValPair*> m_attValVector; 
};

#endif