#ifndef _RADIX_TREE_H_
#define _RADIX_TREE_H_

using namespace std;

template<typename ValueType>

class RadixTree {
    public:
        RadixTree();
        ~RadixTree();
        void insert(string key, const ValueType& value);
        ValueType* search(string key) const;
    private:

};

#endif
