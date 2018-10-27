#ifndef TRIENODE_H_INCLUDED
#define TRIENODE_H_INCLUDED
#include <string.h>
#include <ostream>
using namespace std;
const int num = 28;
const int keynum = 10;
struct Key
{
    char str[keynum];
    Key(char s[])
    {
        for (unsigned i=0; i <= strlen(s) && i<= keynum; i++)
            str[i] = s[i];
    }
    char key_letter(int position)
    {
        if (position < int(strlen(str)))
            return str[position];
        else
            return '\0';
    }
};

struct TrieNode
{
    Key *data;
    TrieNode* branch[num];
    TrieNode()
    {
        data = NULL;
        for (int i=0; i<num; i++)
            branch[i] = NULL;
    }
};

ostream & operator <<(ostream &output,Key &x)
{
    output<<x.str;
    output<<" ";
    return output;
}

int alphabetic(char c)
{
    if (c == ' ' || c=='\0') return 0;
    if ('a'<=c && c<='z') return c-'a'+1;
    if ('A'<=c && c<='Z') return c-'A'+1;
    return 27;
}

int TrieSearch(Key &target,TrieNode *T)
{
    int position = 0;
    char next_char = target.key_letter(position);
    TrieNode *location = T;
    while (location != NULL && next_char != '\0' )
    {
        location = location->branch[alphabetic(next_char)];
        next_char = target.key_letter(++position);
    }
    if (location != NULL && location->data !=NULL)
    {
        cout<<"found "<<*(location->data)<<endl;
        return 1;
    }
    else
    {
        cout<<"not found!"<<endl;
        return 0;
    }
}

int TrieInsert(Key &target,TrieNode *&T)
{
    if (T == NULL)
        T = new TrieNode();
    int position = 0;
    char next_char = target.key_letter(position);
    TrieNode *location = T;
    while (location != NULL && next_char != '\0')
    {
        if (location->branch[alphabetic(next_char)] == NULL)
        {
            location->branch[alphabetic(next_char)] = new TrieNode();
        }
        location = location->branch[alphabetic(next_char)];
        next_char = target.key_letter(++position);
    }
    if (location->data != NULL)
    {
        cout<<"duplicate error!"<<endl;
        return 0;
    }
    else
    {
        location->data = new Key(target.str);
        cout<<"insert success!"<<target<<endl;
        return 1;
    }
}

int TrieDelete(Key &target,TrieNode *&T)
{
    if (T == NULL)
        return 0;
    int position = 0;
    char next_char = target.key_letter(position);
    TrieNode *location = T;
    TrieNode *prelocation = T;
    while (location != NULL && next_char != '\0')
    {
        prelocation = location;
        location = location->branch[alphabetic(next_char)];
        next_char = target.key_letter(++position);
    }
    if (location == NULL)
    {
        cout<<"not found!"<<endl;
         return 0;
    }

    else
    {
        delete location->data;
        delete location;
        prelocation->branch[alphabetic(target.key_letter(--position))] = NULL;
        cout<<"success delete"<<endl;
        return 1;
    }

}

#endif // TRIENODE_H_INCLUDED
