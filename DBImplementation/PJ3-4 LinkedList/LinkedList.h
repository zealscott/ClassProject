#ifndef LINKEDLIST_H // include guard
#define LINKEDLIST_H

#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <fstream>

using namespace std;

class Node
{
  public:
    Node *next_;
    mutex mutex_;
    int key_;
    int value_;
    Node() : next_(nullptr){};
    Node(int key, int value) : next_(nullptr), key_(key), value_(value){};
};

void Add(Node *head, Node *node)
{
    if (node == nullptr || head == nullptr)
        return;
    printf("Inserting %d ....\n", node->key_);
    // need to lock first
    head->mutex_.lock();
    Node *prev = head;
    Node *current = head->next_;
    while (current != nullptr)
    {
        current->mutex_.lock();
        if (node->key_ < current->key_)
        {
            prev->next_ = node;
            node->next_ = current;
            printf("Interted %d success!\n", node->key_);
            current->mutex_.unlock();
            prev->mutex_.unlock();
            return;
        }
        else if (node->key_ == current->key_)
        {
            printf("Inserting %d failed! duplicated key\n", node->key_);
            current->mutex_.unlock();
            prev->mutex_.unlock();
            return;
        }
        else
        {
            prev->mutex_.unlock();
            prev = current;
            current = current->next_;
        }
    }
    // insert to the last
    printf("Interted %d success to the last!\n", node->key_);
    prev->next_ = node;
    prev->mutex_.unlock();
}



void Remove(Node *head, int key)
{
    if (head == nullptr)
        return;
    printf("Romoving %d ....\n", key);
    head->mutex_.lock();
    Node *prev = head;
    Node *current = head->next_;
    while (current != nullptr && current->key_ < key)
    {
        current->mutex_.lock();
        prev->mutex_.unlock();
        prev = current;
        current = current->next_;
    }
    if (current != nullptr && current->key_ == key)
    {
        // its important to lock current node to ensure delete safely
        current->mutex_.lock();
        prev->next_ = current->next_;
        delete current;
        prev->mutex_.unlock();
        printf("Romove %d success!\n", key);
    }
    else
    {
        prev->mutex_.unlock();
        printf("Romove %d failed! Key not exited!\n", key);
    }

    return;
}

bool Contain(Node *head, int key)
{
    if (head == nullptr)
        return false;
    Node *prev = head;
    Node *current = head->next_;
    while (current != nullptr)
    {
        // prev->mutex_.lock();
        // current->mutex_.lock();
        if (current->key_ == key)
        {
            // current->mutex_.unlock();
            // prev->mutex_.lock();
            return true;
        }
        else if (current->key_ < key)
        {
            // prev->mutex_.unlock();
            // current->mutex_.unlock();
            prev = current;
            current = current->next_;
        }
        else
            return false;
    }
    return false;
}

void Print(Node *head)
{
    head->mutex_.lock();
    ofstream fout("result.txt");
    fout << "----------PRINT----------" << endl;
    for (Node *current = head->next_; current != nullptr; current = current->next_)
    {
        fout << current->key_ << ":" << current->value_ << endl;
    }
    head->mutex_.unlock();
    fout << endl;
}

#endif
