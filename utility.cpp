#include "utility.h"

using namespace std;

unsigned int AVPairHash::operator()(const AttValPair& av) const
{
	hash<string> str_hash;
	return str_hash(av.attribute + av.value);
}


bool comp(const EmailCount& ec1, const EmailCount& ec2) {
	return (ec1.count > ec2.count || ((ec1.count == ec2.count) && (ec1.email < ec2.email)));
}