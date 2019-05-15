#ifndef BINARYNODE_H_INCLUDED
#define BINARYNODE_H_INCLUDED

struct BinaryNode
{
    int data;
    BinaryNode* left;
    BinaryNode* right;
    BinaryNode* parent;
    BinaryNode();
    BinaryNode(int &x);
};

BinaryNode::BinaryNode()
{
    data = 0;
    left = NULL;
    right = NULL;
}

BinaryNode::BinaryNode(int &x)
{
    data = x;
    left = NULL;
    right = NULL;
}

#endif // BINARYNODE_H_INCLUDED
