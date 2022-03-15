#ifndef _RADIX_TREE_H_
#define _RADIX_TREE_H_

#include <string>

using namespace std;

template <typename ValueType>
struct RadixN {
	bool endmark;
	RadixN<ValueType>* child[128];
	string child_s[128];
	ValueType val;
	RadixN() {
		for (int i = 0; i < 128; ++i) {
			child[i] = nullptr;
		}
	}
	~RadixN() {
		for (int i = 0; i < 128; ++i) {
			delete child[i];
		}
	}
};

template <typename ValueType>
class RadixTree {
	public:
		RadixTree();
		~RadixTree();
		void insert(string key, ValueType& value);
		ValueType* search(string key) const;
	private:
		RadixN<ValueType>* root;
		void Radix_Insert(RadixN<ValueType>* root, string s,  ValueType& value);
		RadixN<ValueType>* Radix_Query(RadixN<ValueType>* root, string s) const;
};

template <typename ValueType>
RadixTree<ValueType>::RadixTree()
{ 
	root = new RadixN<ValueType>();
	root->endmark = false;
}

template <typename ValueType>
RadixTree<ValueType>::~RadixTree()
{
	delete root;
}

template <typename ValueType>
void RadixTree<ValueType>::insert(string key,  ValueType& value)
{
	Radix_Insert(root, key, value);
}

template <typename ValueType>
void RadixTree<ValueType>::Radix_Insert(RadixN<ValueType>* root, string s, ValueType& value) {
	if (s.empty()) {
		root->endmark = true;
		root->val = value;
		return;
	}
	int cur_char = s[0];
	if (root->child[cur_char] == nullptr) {
		root->child[cur_char] = new RadixN<ValueType>;
		root->child_s[cur_char] = s;
		Radix_Insert(root->child[cur_char], "", value);
		return;
	}
	string transition = root->child_s[cur_char];
	int match_len = 0;
	while (match_len < min(transition.length(), s.length()) && transition[match_len] == s[match_len]) {
		++match_len;
	}
	if (match_len == transition.length()) {
		Radix_Insert(root->child[cur_char], s.substr(match_len), value);
	} else if (match_len == s.length()) {
		RadixN<ValueType>* newnode = new RadixN<ValueType>;
		newnode->child[transition[match_len]] = root->child[cur_char];
		newnode->child_s[transition[match_len]] = transition.substr(match_len);
		root->child[cur_char] = newnode;
		root->child_s[cur_char] = s;
		Radix_Insert(root->child[cur_char], "", value);
	} else {
		RadixN<ValueType>* newnode = new RadixN<ValueType>;
		newnode->child[transition[match_len]] = root->child[cur_char];
		newnode->child_s[transition[match_len]] = transition.substr(match_len);
		root->child[cur_char] = newnode;
		root->child_s[cur_char] = s.substr(0, match_len);
		Radix_Insert(root->child[cur_char], s.substr(match_len), value);
	}
}

template <typename ValueType>
ValueType* RadixTree<ValueType>::search(string key) const {
	RadixN<ValueType>* foundNode = Radix_Query(root, key);
	if(foundNode != nullptr) {
		 ValueType* ptr = &(foundNode->val);
		return ptr;
	} 
	return nullptr;
}

template <typename ValueType>
 RadixN<ValueType>* RadixTree<ValueType>::Radix_Query(RadixN<ValueType>* root, string s) const {
	if (s.empty()) {
		return (root->endmark) ? root : nullptr;
	}
	int cur_char = s[0];
	if (root == nullptr) {
		return nullptr;
	}
	if (root->child[cur_char] == nullptr) {
		return nullptr;
	}
	string transition = root->child_s[cur_char];
	int len = min(transition.length(), s.length());
	if (transition.substr(0, len) != s.substr(0, len)) {
		return nullptr;
	}
	if (transition.length() == len) {
		return Radix_Query(root->child[cur_char], s.substr(transition.length()));
	} else {
		return nullptr;
	}
}

// FOR DEBUGGING, MUST REMOVE
/*
template <typename ValueType>
map<string, ValueType*> RadixTree<ValueType>::getMap() {
	return m_map;
}
*/

#endif
