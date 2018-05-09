#ifndef AVLTREE_H_INCLUDED
#define AVLTREE_H_INCLUDED
#include "BinarySearchTree.h"
#include "AVLtreeNode.h"


template<class Record>
class AVL_tree:public Search_tree<Record>
{
public:
    Error_code Insert(const Record &new_data);
    Error_code Remove(const Record &old_data);
private:
//  Insert
    Error_code avl_insert(
        Binary_node<Record>* &sub_root,const Record &new_data,bool &taller);
    void rotate_left(Binary_node<Record>* &sub_root);
    void rotate_right(Binary_node<Record>* &sub_root);
    void right_balance(Binary_node<Record>* &sub_root);
    void left_balance(Binary_node<Record>* &sub_root);
//  Remove
    Error_code avl_remove(
        Binary_node<Record>* &sub_root,const Record &old_data,bool &shorter);
    bool right_balance2(Binary_node<Record>* &sub_root);
    bool left_balance2(Binary_node<Record>* &sub_root);
};

template<class Record>
Error_code AVL_tree<Record>::Insert(const Record &new_data)
{
    bool taller;
    Error_code result = avl_insert(this->root,new_data,taller);
    if (result == success) this->Count++;
    return result;
}


template<class Record>
Error_code AVL_tree<Record>::avl_insert(
    Binary_node<Record>* &sub_root,const Record &new_data,bool &taller)
{
    Error_code result = success;
    // if the root is empty
    if (sub_root == NULL)
    {
        sub_root = new AVL_node<Record>(new_data);
        taller = true;
    }
    else if (new_data == sub_root->data)
    {
        result = duplicate_error;
        cout<<"duplicate_error!"<<endl;
        taller = false;
    }
    else if (new_data < sub_root->data)
    {
        //insert in left subtree
        result = avl_insert(sub_root->left,new_data,taller);
        if (taller == true)
        {
            switch (sub_root->get_balance())
            {
            case left_higher:
                left_balance(sub_root);
                taller = false;
                break;
            case equal_height:
                sub_root->set_balance(left_higher);
                break;
            case right_higher:
                sub_root->set_balance(equal_height);
                taller = false;
                break;
            }
        }
    }
    else
    {
        //insert in right subtree
        result = avl_insert(sub_root->right,new_data,taller);
        if (taller == true)
        {
            switch (sub_root->get_balance())
            {
            case left_higher:
                sub_root->set_balance(equal_height);
                taller = false;
                break;
            case equal_height:
                sub_root->set_balance(right_higher);
                break;
            case right_higher:
                right_balance(sub_root);
                taller = false;
                break;
            }
        }
    }
    return result;
}

template<class Record>
void AVL_tree<Record>::rotate_right(Binary_node<Record>* &sub_root)
{
    if (sub_root == NULL || sub_root->left == NULL)
        cout<<"WARNING: program error detected in rotate right!"<<endl;
    else
    {
        Binary_node<Record>*left_tree = sub_root->left;
        sub_root->left = left_tree->right;
        left_tree->right = sub_root;
        sub_root = left_tree;
    }
}

template<class Record>
void AVL_tree<Record>::rotate_left(Binary_node<Record>* &sub_root)
{
    if (sub_root == NULL || sub_root->right == NULL)
        cout<<"WARNING: program error detected in rotate left!"<<endl;
    else
    {
        Binary_node<Record>*right_tree = sub_root->right;
        sub_root->right = right_tree->left;
        right_tree->left = sub_root;
        sub_root = right_tree;
    }
}

template<class Record>
void AVL_tree<Record>::right_balance(Binary_node<Record>* &sub_root)
{
    Binary_node<Record>* &right_tree = sub_root->right;
    switch (right_tree->get_balance())
    {
    case right_higher:
        sub_root->set_balance(equal_height);
        right_tree->set_balance(equal_height);
        rotate_left(sub_root);
        break;
    case equal_height:
        cout<<"WARNING:program error in right balance"<<endl;
        break;
    case left_higher:
        Binary_node<Record>* sub_tree = right_tree->left;
        //  prepare to rotate
        switch(sub_tree->get_balance())
        {
        case equal_height:
            sub_root->set_balance(equal_height);
            right_tree->set_balance(equal_height);
            break;
        case left_higher:
            sub_root->set_balance(equal_height);
            right_tree->set_balance(right_higher);
            break;
        case right_higher:
            sub_root->set_balance(left_higher);
            right_tree->set_balance(equal_height);
            break;
        }
        sub_tree->set_balance(equal_height);
        rotate_right(right_tree);
        rotate_left(sub_root);
        break;
    }
}


template<class Record>
void AVL_tree<Record>::left_balance(Binary_node<Record>* &sub_root)
{
    Binary_node<Record>* &left_tree = sub_root->left;
    switch (left_tree->get_balance())
    {
    case left_higher:
        sub_root->set_balance(equal_height);
        left_tree->set_balance(equal_height);
        rotate_right(sub_root);
        break;
    case equal_height:
        cout<<"WARNING:program error in right balance"<<endl;
        break;
    case right_higher:
        Binary_node<Record>* sub_tree = left_tree->right;
        //  prepare to rotate
        switch(sub_tree->get_balance())
        {
        case equal_height:
            sub_root->set_balance(equal_height);
            left_tree->set_balance(equal_height);
            break;
        case right_higher:
            sub_root->set_balance(equal_height);
            left_tree->set_balance(left_higher);
            break;
        case left_higher:
            sub_root->set_balance(right_higher);
            left_tree->set_balance(equal_height);
            break;
        }
        sub_tree->set_balance(equal_height);
        rotate_left(left_tree);
        rotate_right(sub_root);
        break;
    }
}

template<class Record>
Error_code AVL_tree<Record>::Remove(const Record &old_data)
{
    bool shorter;
    Error_code result = avl_remove(this->root,old_data,shorter);
    if (result == success) this->Count--;
    return result;
}

template<class Record>
Error_code AVL_tree<Record>::avl_remove(
    Binary_node<Record>* &sub_root, const Record &old_data,bool &shorter)
{
    Error_code result = success;
    if (sub_root == NULL)
    {
        shorter = false;
        return not_present;
    }
    else if (old_data == sub_root->data)
    {
        Binary_node<Record>* to_delete = sub_root;
        shorter = true;
        if (sub_root->right == NULL)
            sub_root = sub_root->left;
        else if (sub_root->left == NULL)
            sub_root = sub_root->right;
        else
        {
            // neither subtree is empty
            to_delete = sub_root->left;
            //move left to find predecessor
            Binary_node<Record>* parent = sub_root;
            while(to_delete->right != NULL)
            {
                parent = to_delete;
                to_delete = to_delete ->right;
            }
            //move from to_delete to root
            sub_root->data = to_delete->data;
            if (parent == sub_root)
                sub_root->left = to_delete->left;
            else parent->right = to_delete->left;
        }
        delete to_delete;
        return success;
    }
    else if (old_data < sub_root->data)
    {
        result = avl_remove(sub_root->left,old_data,shorter);
        if (shorter == true)
        {
            switch(sub_root->get_balance())
            {
            case left_higher:
                // 高度减小，要递归
                sub_root->set_balance(equal_height);
                break;
            case equal_height:
                sub_root->set_balance(right_higher);
                // 总高度不变，不需要递归
                shorter = false;
                break;
            case right_higher:
                shorter = right_balance2(sub_root);
                break;
            }
        }
        return result;
    }
    else
    {
        result = avl_remove(sub_root->right,old_data,shorter);
        if (shorter == true)
        {
            switch (sub_root->get_balance())
            {
            case left_higher:
                shorter = left_balance2(sub_root);
                break;
            case equal_height:
                sub_root->set_balance(left_higher);
                shorter = false;
                break;
            case right_higher:
                sub_root->set_balance(equal_height);
                break;
            }
        }
    }
    return result;
}

template<class Record>
bool AVL_tree<Record>::right_balance2(Binary_node<Record>* &sub_root)
{
    bool shorter;
    Binary_node<Record>* &right_tree = sub_root->right;
    switch(right_tree->get_balance())
    {
    case right_higher:
        sub_root->set_balance(equal_height);
        right_tree->set_balance(equal_height);
        rotate_left(sub_root);
        shorter = true;
        break;
    case equal_height:
        right_tree->set_balance(left_higher);
        rotate_left(sub_root);
        shorter = false;
        break;
    case left_higher:
        Binary_node<Record>* sub_tree = right_tree->left;
        switch(sub_tree->get_balance())
        {
        case equal_height:
            sub_root->set_balance(equal_height);
            right_tree->set_balance(equal_height);
            break;
        case left_higher:
            sub_root->set_balance(equal_height);
            right_tree->set_balance(right_higher);
            break;
        case right_higher:
            sub_root->set_balance(left_higher);
            right_tree->set_balance(equal_height);
            break;
        }
        sub_tree->set_balance(equal_height);
        rotate_right(right_tree);
        rotate_left(sub_root);
        shorter = true;
        break;
    }
    return shorter;
}


template<class Record>
bool AVL_tree<Record>::left_balance2(Binary_node<Record>* &sub_root)
{
    bool shorter;
    Binary_node<Record>* &left_tree = sub_root->left;
    switch (left_tree->get_balance())
    {
    case left_higher:
        sub_root->set_balance(equal_height);
        left_tree->set_balance(equal_height);
        rotate_right(sub_root);
        shorter = true;
        break;
    case equal_height:
        left_tree->set_balance(right_higher);
        rotate_right(sub_root);
        shorter = false;
        break;
    case right_higher:
        Binary_node<Record>* sub_tree = left_tree->right;
        //  prepare to rotate
        switch(sub_tree->get_balance())
        {
        case equal_height:
            sub_root->set_balance(equal_height);
            left_tree->set_balance(equal_height);
            break;
        case right_higher:
            sub_root->set_balance(equal_height);
            left_tree->set_balance(left_higher);
            break;
        case left_higher:
            sub_root->set_balance(right_higher);
            left_tree->set_balance(equal_height);
            break;
        }
        sub_tree->set_balance(equal_height);
        rotate_left(left_tree);
        rotate_right(sub_root);
        shorter = true;
        break;
    }
    return shorter;
}




#endif // AVLTREE_H_INCLUDED
