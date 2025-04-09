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

void RedBlackTree::insert(Node* &pos, Node* prev, const int data, direction dir) {
  if (pos == nullptr) {
    pos = new Node(data); //this will be the base case of the recursion (will insert once the path has ended)
    pos->parent = prev; //set parent node
    insert(pos, dir); //do the actual insert
  }
  
  
  //if number is already in the tree
  if (pos->data == data) {
    std::cout << "Already in the tree" << std::endl;
    return;
  } 
  
  //if data being inserted is greater than node then go right
  if (pos->data < data) {
    insert(pos->right, pos,  data, right);
  } else if (pos->data > data) { //If data being inserted is less than node go left
    insert(pos->left, pos, data, left); 
  }
}

// Corrected method definitions
void RedBlackTree::insert(Node* node, direction dir) {

  Node* parent = node->parent;

  if (parent == nullptr) {
    root = node; //if there aren't any other nodes in the tree then this node is root
    return;
  }


  Node* grandparent = parent->parent;
  Node* uncle = nullptr;

  parent->setChild(dir, node); //update the parent's child to passed in node

  //Now do the balancing

  /*
   * Requirements:
  *  1 Every node is either red or black
  *  2 All null nodes  are considered black.
  *  3 A red node does not have a red child.
  *  4 Every path from a given node to any of its leaf nodes goes through the same number of black nodes.
  *  (Conclusion) If a node N has exactly one child, the child must be red, because if it were black, its leaves would sit at a different black depth than N's child, violating requirement 4.
   * /

  //Do the actual condition logic (see images on wikipedia)
  /*
    6 conditions
    1: if the parent color is black, every requirement holds so we don't need to do anything
    2: if parent or uncle are red, both become black and grandparent becomes red (maintains requirement 4).
       since any path through parent/uncle passes through grandparent # of black nodes doesn't change.
       Grandparent now violates req 3 if it has a red parent, so one must set grandparent = passed in node
    3: Insert case 2 has happened enoguh times where the heigh of the tree is now 1 greater, t
       this makes passed in node the red root, meaning all requirements are valid (don't have to change anything)
    4. Parent is red and is root, passed in node is default red, but we can just change root color to have a valid tree
    5. Parent is Red, uncle is black. We want to rotate parent node to grandparent position.
       This won't work if passed in Node is inner grandchild of grandparent (N is left child of right child of grandparent or right child of left child)
       We need to rotate the parent of the passed in Node (by direction dir (which is the direction Node is passed in on) to switch Node and it's parent,
       both the Parent and Node will be red so req 4 is fine.
       Requirment 3 will need further correction (case 6)
    6. Passed in Node is now outer grandchild (left of left child or right of right child of grandparent).
       Now rotating grandparent by opposite direction of where the passed in Node went (dir) will make the Parent in the grandparent's position
       and make parent the parent of the passed in node AND the grandparent node. The grandparent node will be black and the Parent of it (it's former child) is red
       Then switching the colors of the Parent and Grandparent fixes requirement 3.
       Req 4 is still fine since all paths that went through the grandparent (which was black) now go through the parent (which is now black).
  */


  //recursion or iteration....
  do {
    //Case 1
    if (parent->color == black) {
      return; //requirements hold
    }



    dir = (parent == grandparent->right) ? right : left; //direction of parent
    uncle = grandparent->child(1 - dir); //opposite parent direction

    //since we assume null is black
    if (uncle != nullptr || uncle->color == BLACK) {
      
    }

    //i can move these cases down here for simplicity
    //!!check if parent is red
    //case 2 (if parent or uncle are red), if parent is red then 
    if (parent->color == RED || uncle->color == RED) {
      parent->color == BLACK;
      uncle->color == BLACK;
      grandparent->color = RED;
      node = grandparent; //fixes grandparent violating req 3 if it has a red parent
    }

    //case 3 (if case 2 has happened enough for height to increase)
    return; //then every requirement is met

    //case 4 (if red and root)
    if (parent->color == RED && grandparent == nullptr) {
      parent->color = BLACK; //switch root color
      return;
    }

    //Case 5
    if ((uncle == nullptr || uncle->color == BLACK) && parent->color == RED) {
      //Check if inner child
      if (node == parent->child(1-dir)) {

      rotateSubTree(parent, dir); //rotate parent to grandparent position (dir is the direction parent is in relative to grandparent).

      //then swap node and parent again
      node = parent; 
      parent = grandparent->child[dir];
      }
    }

    
    
  } while()
	

      


      

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
