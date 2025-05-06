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

  explicit Node(const int data) {
    this->data = data;
  }
  
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
  void swapNodes(Node *n1, Node *n2);

  direction nodeDirection(Node* node); //helper function, get's node's direction relative to its parent
  void transplant(Node *u, Node *v);

  void insert(Node* &pos, Node* prev, int data, direction dir = right); //for outside calling of the function (goes to end of tree)
  void insertBalance(Node* node, direction dir); //for inside class calling (actually does most of the logic)

  void remove(Node *toRemove);
  void removeBalance(Node* node);
  void print(const Node* pos , int depth = 0, bool isRight = false);
  Node* getNode(Node* pos, int data);

  ~RedBlackTree();

 
  Node* root = nullptr;
};

#endif
