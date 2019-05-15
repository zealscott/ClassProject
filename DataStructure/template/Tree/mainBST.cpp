#include <iostream>
#include "BinaryTree.h"
#include "BinarySearchTree.h"

using namespace std;

int main()
{
    Search_tree<int> mytree;
    mytree.Insert(2);
    mytree.Insert(4);
    mytree.Insert(1);
    mytree.Insert(3);
    cout<<"the tree size is "<<mytree.Size()<<endl;
    cout<<"the tree height is "<<mytree.Height()<<endl;
    cout<<"preorder:"<<endl;
    mytree.PreOrder(print);
    cout<<endl;
    cout<<"inorder:"<<endl;
    mytree.InOrder(print);
    cout<<endl;
    cout<<"postorder:"<<endl;
    mytree.PostOrder(print);
    mytree.Remove(4);
    cout<<"the tree size is "<<mytree.Size()<<endl;
    cout<<"the tree height is "<<mytree.Height()<<endl;
    cout<<"preorder:"<<endl;
    mytree.PreOrder(print);
    cout<<endl;
    cout<<"inorder:"<<endl;
    mytree.InOrder(print);
    cout<<endl;
    cout<<"postorder:"<<endl;
    mytree.PostOrder(print);
    return 0;
}
