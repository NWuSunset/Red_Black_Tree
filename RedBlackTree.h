enum Color {
  RED,
  BLACK
};

enum direction {
  left,
  right
};



struct Node {
  int data;
  Node* left = nullptr;
  Node* right = nullptr;
  Node* parent = nullptr;
  Color color = RED; //nodes start as color red
};


class RedBlackTree {
 public:
  RedBlackTree();

  Node* RotateDir(Node* node, direction dir); //rotates the tree in a direction
  void insert(Node* pos, int data);
  void balance(Node* Node);
  void remove(Node* rem);
  void print(Node* pos);
  Node* getNode(Node* pos, int data);

  ~RedBlackTree();

 //private:
  Node* root = nullptr;
};
