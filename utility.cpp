#include "utility.h"

using namespace std;

unsigned int AVPairHash::operator()(const AttValPair& attval) const {
	hash<string> str_hash;
	return str_hash(attval.attribute + attval.value);
}


bool comp(const EmailCount& ec1, const EmailCount& ec2) {
	return (ec1.count > ec2.count || ((ec1.count == ec2.count) && (ec1.email < ec2.email)));
}