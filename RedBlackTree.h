enum Color {
  RED,
  BLACK
};

struct Node {
  int data;
  Node* left = nullptr;
  Node* right = nullptr;
  Node* parent = nullptr;
  Color color = RED;
};


class RedBlackTree {
 public:
  RedBlackTree();

 private:
};
