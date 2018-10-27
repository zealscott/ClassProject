//
// Created by Du Scott on 2018/10/27.
//

#ifndef TRIE_TRIE_H
#define TRIE_TRIE_H

#include <string>
#include <map>
#define CHARSIZE 3

using namespace std;

class TrieNode // 字典树节点
{
public:
    int count;// count word
    bool isEnd;// is the last node?
    string val;// the node value, if Chinese, 3 char needed
    map<string, TrieNode*> childMap;// children nodes hash map
    TrieNode(string str);
    TrieNode();
};

class Trie{
private:
    TrieNode* root;
public:
    Trie();
    void print();
    void insert(string str);
    bool isExist(string words);
};



#endif //TRIE_TRIE_H
