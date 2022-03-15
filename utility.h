#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <functional>
#include "provided.h"

class AVPairHash {
public:
	unsigned int operator()(const AttValPair& av) const;
};

bool comp(const EmailCount& ec1, const EmailCount& ec2);

#endif
