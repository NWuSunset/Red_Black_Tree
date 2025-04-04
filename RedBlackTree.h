enum Color {
  RED,
  BLACK
};

struct Node {
  int data;
  Node* left = nullptr;
  Node* right = nullptr;
  Node* parent = nullptr;
  Color color;
};


class RedBlackTree {
 public:
  RedBlackTree();

  void insert(Node* pos, int data);
  void remove(Node* rem);
  void print(Node* pos);
  Node* getNode(Node* pos, int data);

  ~RedBlackTree();

 //private:
  Node* root = nullptr;
};
