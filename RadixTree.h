#ifndef _RADIX_TREE_H_
#define _RADIX_TREE_H_

#include <map>
#include <iostream>

using namespace std;

template <typename ValueType>
class RadixTree {
public:
	RadixTree();
	~RadixTree();
	void insert(string key, const ValueType& value);
	ValueType* search(string key) const;
	map<string, ValueType*> getMap();
	
private:
	map<string, ValueType*> m_map;
};

template <typename ValueType>
RadixTree<ValueType>::RadixTree()
	: m_map(map<string, ValueType*>())
{}

template <typename ValueType>
RadixTree<ValueType>::~RadixTree()
{
	for (auto it = m_map.begin(); it != m_map.end(); it++) {
		delete it->second;
	}
}

template <typename ValueType>
void RadixTree<ValueType>::insert(string key, const ValueType& value)
{
	auto it = m_map.find(key);
	if (it != m_map.end()) {
		delete it->second;
	}
	ValueType* val = new ValueType(value);
	m_map[key] = val;
}

template <typename ValueType>
ValueType* RadixTree<ValueType>::search(string key) const
{
	auto it = m_map.find(key);
	if (it != m_map.end()) {
		//return const_cast<ValueType*>(it->second);
		return it->second;
	}
	return nullptr;
}


// FOR DEBUGGING, MUST REMOVE
template <typename ValueType>
map<string, ValueType*> RadixTree<ValueType>::getMap() {
	return m_map;
}

#endif
