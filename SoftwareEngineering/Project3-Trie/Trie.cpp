//
// Created by Du Scott on 2018/10/27.
//

#include <iostream>
#include "Trie.h"



TrieNode::TrieNode(string str) {
    count = 1;
    isEnd = false;
    val = str;
}

TrieNode::TrieNode() {
    count = 1;
    isEnd = false;
    val = "";
}


Trie::Trie() // 初始化字典树
{
    root = new TrieNode();
}


/***
 * @method insert
 * @param str
 */
void Trie::insert(string str) {
    if (str.empty() || str.length() < CHARSIZE) {
        return;
    }
    TrieNode *node = root;

    unsigned long len = str.length(),pos;

    for (pos= 0; pos < len;) {
        string subStr = str.substr(pos, CHARSIZE);

//        if the word not exist
        if (node->childMap.find(subStr) == node->childMap.end()) {
//            cout<<"insert :" << subStr << " "
            TrieNode* tmp = new TrieNode(subStr);
            node->childMap.insert(pair<string,TrieNode*>(subStr,tmp));
            node = tmp;
        }
//        if exsit
        else {
            node->count++;
            node = node->childMap.find(subStr)->second;
        }
        pos += CHARSIZE;
    }
    node->isEnd = true;
}


void Trie::print() {
    map<string, TrieNode*>::reverse_iterator iter;

    for(iter = root->childMap.rbegin(); iter != root->childMap.rend(); iter++)

        cout<<iter->first<<"  "<<iter->second<<endl;
}

bool Trie::isExist(string words) {
    if (words.empty() || words.length() < CHARSIZE) {
        return false;
    }
    TrieNode *node = root;

    unsigned long len = words.length(),pos;

    for (pos= 0; pos < len;) {
        string subStr = words.substr(pos, CHARSIZE);

//        if the word not exist
        if (node->childMap.find(subStr) == node->childMap.end()) {
//            cout <<"words : " << subStr << "not exist" <<endl;
            return false;
        }
//        if exsit
        else {
//            cout <<"words : " << subStr << "exist" <<endl;
            node = node->childMap[subStr];
        }
        pos += CHARSIZE;
    }

    return node->isEnd;
}