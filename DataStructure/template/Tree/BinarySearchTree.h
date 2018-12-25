#ifndef BINARYSEARCHTREE_H_INCLUDED
#define BINARYSEARCHTREE_H_INCLUDED
#include "BinaryTree.h"
using namespace std;

enum Error_code {success,not_present,duplicate_error};

//class Record
//{
//public:
//    int key_size;
//    string key;
//    char key_letter(int position)const;
//    Record(int i);
//    Record();
//    friend ostream& operator<<(ostream & os,const Record &In);
//};


template<class Record>
class Search_tree:public Binary_tree<Record>
{
public:
    Error_code Insert(const Record &new_data);
    Error_code Remove(const Record &target);
    Error_code tree_search(const Record &target);
private:
    Binary_node<Record>* search_for_node(
        Binary_node<Record>* sub_root,const Record &target)const;
    Error_code search_and_insert(
        Binary_node<Record>* &sub_root, const Record &new_data);
    Error_code remove_root(Binary_node<Record>* &sub_root);
    Error_code search_and_destroy(
        Binary_node<Record>* &sub_root,const Record &target);
};


//--------------------complement------------//
template <class Record>
Binary_node<Record>* Search_tree<Record>::search_for_node(
    Binary_node<Record>* sub_root,const Record &target)const
{
    while(sub_root != NULL && sub_root->data !=target)
    {
        if (sub_root->data < target)
            sub_root = sub_root->right;
        else
            sub_root = sub_root->left;
    }
    return sub_root;
}

template<class Record>
Error_code Search_tree<Record>::tree_search(const Record &target)
{
    Error_code result = success;
    Binary_node<Record> *found = search_for_node(this->root,target);
    if (found == NULL)
        result = not_present;
    else
        target = found->data;
    return result;
}

template<class Record>
Error_code Search_tree<Record>::search_and_insert(
    Binary_node<Record>* &sub_root, const Record &new_data)
{
    if (sub_root == NULL)
    {
        sub_root = new Binary_node<Record>(new_data);
        return success;
    }
    else if (new_data < sub_root ->data)
        return search_and_insert(sub_root->left,new_data);
    else if (new_data > sub_root ->data)
        return search_and_insert(sub_root->right,new_data);
    else
        return duplicate_error;

}

template<class Record>
Error_code Search_tree<Record>::Insert(const Record &new_data)
{
    Error_code result = search_and_insert(this->root,new_data);
    if (result == success) this->Count++;
    return result;
}
template<class Record>
Error_code Search_tree<Record>::remove_root(Binary_node<Record>* &sub_root)
{
    if (sub_root == NULL) return not_present;
    Binary_node<Record> *to_delete = sub_root;
    if (sub_root->right == NULL) sub_root = sub_root->left;
    else if (sub_root->left == NULL) sub_root = sub_root->right;
    else
    {
        //Neither subtree is empty
        to_delete = sub_root->left;
        //find the predecessor
        Binary_node<Record> *parent = sub_root;
        while (to_delete->right != NULL)
        {
            parent = to_delete;
            to_delete = to_delete->right;
        }
        sub_root->data = to_delete->data;
        //move from to_delete to root
        if (parent == sub_root)
            sub_root->left = to_delete->left;
        else parent->right = to_delete->left;
    }
    delete to_delete;
    return success;
}

template<class Record>
Error_code Search_tree<Record>::search_and_destroy(
    Binary_node<Record>* &sub_root,const Record &target)
{
    if (sub_root ==NULL || sub_root->data ==target)
    {
        return remove_root(sub_root);
    }
    else if (target < sub_root->data)
        return search_and_destroy(sub_root->left,target);
    else
        return search_and_destroy(sub_root->right,target);
}

template<class Record>
Error_code Search_tree<Record>::Remove(const Record &target)
{
    Error_code result = search_and_destroy(this->root,target);
    if (result == success)this->Count--;
    return result;
}


#endif // BINARYSEARCHTREE_H_INCLUDED
