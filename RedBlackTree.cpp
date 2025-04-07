#include "RedBlackTree.h"

// Corrected constructor name
RedBlackTree::RedBlackTree() = default;


//https://en.wikipedia.org/wiki/Tree_rotation visualization
//node passed in is the root of the subtree (perhaps rename later
Node* RedBlackTree::rotateSubTree(Node* subRoot, const direction dir) {
  //Will rotate left or right.
  Node* parent = subRoot->parent; //save original root parent

  //New root is now the subRoot's opposite child
  //(of the direction passed in, so if we want to rotate left this would be the right child)
  Node* newRoot = subRoot->child(1 - dir);

  //swap the dir child of the subRoot and newRoot
  Node* newChild = newRoot->child(dir);
  subRoot->setChild(1 - dir, newChild);

  //update parents
  if (newChild != nullptr) { //for newChild
    newChild->parent = subRoot;
  }
  newRoot->setChild(dir, parent); //for newRoot
  subRoot->parent = newRoot; //for original subRoot

  //update the children for parent
  if (parent != nullptr) {
    //if subRoot == parent->right, it evaluates to 1 which is the right direction (since right and left are basically booleans)
    parent->setChild(subRoot == parent->right, newRoot);
  } else {
    root = newRoot;
  }

  return newRoot;
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