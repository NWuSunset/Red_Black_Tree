#include "RedBlackTree.h"

// Corrected constructor name
RedBlackTree::RedBlackTree() = default;

void RotateDir(Node* node, direction dir) {
  //Will rotate left or right.
} 

// Corrected method definitions
void RedBlackTree::insert(Node* node, Node* parent, direction dir) {
  // Implementation for inserting a node
  Node* grandparent = nullptr;
  Node* uncle = nullptr;

  if (parent = nullptr) {
    root = node; //if there aren't any other nodes in the tree then this node is root
    return;
  }
  //Else, check which side this node is on
  if (parent->data < node->data) {
    parent->right = node;
  } else {
    parent->left = node;
  }



  //Do the actual condition logic
  /*
    6 conditions
    1: 
  */

  balanceInsert(node, dir);
  

}

//goes through the insertion conditions to balance the tree
void balanceInsert(Node* Node) {


}
void RedBlackTree::remove(Node* rem) {
  // Implementation for removing a node
}

void RedBlackTree::print(Node* pos) {
  // Implementation for printing the tree
}

Node* RedBlackTree::getNode(Node* pos, int data) {
  // Implementation for getting a node
  return nullptr; // Placeholder
}

// Corrected destructor
RedBlackTree::~RedBlackTree() = default;