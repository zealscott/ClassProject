#include <iostream>
#include "AVLTree.h"


using namespace std;

int main4()
{
    AVL_tree<int> mytree;
    for (int i=9;i>0;i--)
        mytree.Insert(i);
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
    mytree.Remove(8);
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
