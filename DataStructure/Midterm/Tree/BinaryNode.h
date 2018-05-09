#ifndef BINARYNODE_H_INCLUDED
#define BINARYNODE_H_INCLUDED

template<class Record>
struct BinaryNode
{
    Record data;
    BinaryNode *left;
    BinaryNode *right;

    BinaryNode();
    BinaryNode(const Record &x);
};

template<class Record>
BinaryNode<Record>::BinaryNode()
{
    left = NULL;
    right = NULL;
}


template<class Record>
BinaryNode<Record>::BinaryNode(const Record &x)
{
    data = x;
    left = NULL;
    right = NULL;
}






#endif // BINARYNODE_H_INCLUDED
