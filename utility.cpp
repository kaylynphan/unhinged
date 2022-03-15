#include "utility.h"

using namespace std;

unsigned int AVPairHash::operator()(const AttValPair& attval) const {
	hash<string> str_hash;
	return str_hash(attval.attribute + attval.value);
}

// An EmailCount is "less" or should be earlier in order when
// Primarily: its count is higher
// Secondarily: its email is lexographically earlier
bool operator<(const EmailCount& a, const EmailCount& b) {
    return (a.count > b.count || ((a.count == b.count) && (a.email < b.email)));
} 

/*
bool comp(const EmailCount& a, const EmailCount& b) {
	return (a.count > b.count || ((a.count == b.count) && (a.email < b.email)));
}
*/