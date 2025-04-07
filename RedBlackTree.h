#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H


enum Color {
  RED,
  BLACK
};

//Treat left as == 0, and right as == 1
enum direction {
  left = 0,
  right = 1
};



struct Node {
  int data{};
  Node* left = nullptr; //left will be index 0
  Node* right = nullptr; //right will be index 1
  Node* parent = nullptr;
  Color color = RED; //nodes start as color red

  Node* child(const int index) const {
    return index == 0 ? left : right; //Returns left if index is 0
  }

  //set's left or right child to value
  void setChild(const int index, Node* node) {
    if (index == 0) {
      left = node;
    } else {
      right = node;
    }
  }
};


class RedBlackTree {
 public:
  RedBlackTree();

  Node* rotateSubTree(Node* subRoot, direction dir); //rotates the tree in a direction
  void insert(Node* pos, int data);
  void balance(Node* Node);
  void remove(Node* rem);
  void print(Node* pos);
  Node* getNode(Node* pos, int data);

  ~RedBlackTree();

 //private:
  Node* root = nullptr;
};

#endif
