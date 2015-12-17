//
//  main.cpp
//  HW5
//
//  Created by Connor Kenny on 3/8/15.
//  Copyright (c) 2015 Connor Kenny. All rights reserved.
//

#include <iostream>

struct BinaryTreeNode
{
    BinaryTreeNode* m_parent;
    BinaryTreeNode* m_left;
    BinaryTreeNode* m_right;
    int m_data;
};

void insert(Node*& root, int data) {
    if (!root)
        root = new Node(data);
    else if (data < root->data)
        insert(root->left, data);
    else if (data > root->data)
        insert(root->right, data);
}


Node* current = headOfBinaryTree;
Node* previous = NULL;
while the current pointer is not null:
set the previous pointer to the current pointer;
if the value of the node to be inserted is greater than the value of current:
set current to the right pointer in the node pointed by current;
else:
set current to the left pointer;
set the left and right pointers in toBeInserted to NULL;
set the parent pointer in the toBeInserted node to previous;
if the value of toBeInserted is greater than the value of previous:
set the left pointer of the node pointed by previous to toBeInserted;
else:
set the right pointer to previous;