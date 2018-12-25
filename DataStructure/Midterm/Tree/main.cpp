#include <iostream>
#include "BinaryTree.h"
#include "BST.h"
using namespace std;

int main()
{
    SearchTree<float> MyTree;
    SearchTree<float> MyTree2;
    for (float i=10;i>0;i--)
        MyTree.Insert(i);
    for (float i=10;i>0;i--)
        MyTree2.Insert(i);
    MyTree.PreOrder();
    cout<<endl;
    MyTree.InOrder();
    cout<<endl;
    MyTree.PostOrder();
    cout<<endl;
    cout<<MyTree.Height()<<endl;
    cout<<(MyTree2==MyTree);
    return 0;
}
