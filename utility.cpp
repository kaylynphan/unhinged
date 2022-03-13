
#include "utility.h"
#include "provided.h"

bool operator<(const AttValPair& av1, const AttValPair & av2) {
    return (av1.attribute < av2.attribute  
        ||  ((av1.attribute == av2.attribute) & (av1.value < av2.value)));
}