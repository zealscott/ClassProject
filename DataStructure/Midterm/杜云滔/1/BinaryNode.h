#ifndef BINARYNODE_H_INCLUDED
#define BINARYNODE_H_INCLUDED

struct BinaryNode
{
    int data;
    BinaryNode* left;
    BinaryNode* right;
    int status;
    BinaryNode();
    BinaryNode(int &x);
};

BinaryNode::BinaryNode()
{
    data = 0;
    status = 0;
    left = NULL;
    right = NULL;
}

BinaryNode::BinaryNode(int &x)
{
    data = x;
    status = 0;
    left = NULL;
    right = NULL;
}

#endif // BINARYNODE_H_INCLUDED
