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
    int data{}; //data stored in node
    Node* left = nullptr; //left will be index 0
    Node* right = nullptr; //right will be index 1
    Node* parent = nullptr; //node's parent
    Color color = RED; //nodes start as the color red

    /**
    * @brief Constructs a node with the given value
    * @param data Value to store in the node
    */
    explicit Node(const int data) {
        this->data = data;
    }

    /**
 * @brief Retrieves a child node by direction (index) (0 for left, 1 for right)
 * @param index The index of the child to retrieve
 * @return Pointer to the child node
 */
    Node* child(const int index) const {
        return index == 0 ? left : right;
    }

    /**
    * @brief Sets a child node by index (0 for left, 1 for right)
    * @param index The index of the child to set
    * @param node The node to set as the child
    */
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

    /**
 * @brief Rotates a subtree around a pivot node in the specified direction
 * @param subRoot The root of the subtree to rotate
 * @param dir The direction to rotate (left or right)
 * @return Pointer to the new subtree root after rotation
 */
    Node* rotateSubTree(Node* subRoot, direction dir); //rotates the tree in a direction

    /**
 * @brief Determines whether a node is a left or right child of its parent
 * @param node The node whose direction is to be determined
 * @return The direction enum value (left or right)
 */
    static direction nodeDirection(const Node* node); //helper function, get's node's direction relative to its parent

    /**
     * @brief Gets the color of a node, treating nullptr as BLACK
     * @param node The node whose color is to be determined
     * @return The color enum value (RED or BLACK)
     */
    static Color getColor(const Node* node);

    /**
 * @brief Replaces one subtree with another
 * @param u The subtree to be replaced
 * @param v The subtree that replaces u
 */
    void transplant(Node *u, Node *v);

    /**
 * @brief Finds the minimum value (in order successor) node in a subtree
 * @param node The root of the subtree
 * @return Pointer to the node with minimum value (in order successor)
 */
    static Node* tree_min(Node* node);

    /**
 * @brief Inserts a new value into the Red-Black tree
 * @param pos Reference to the current position in the tree (initially root)
 * @param prev Parent node of the current position
 * @param data Value to be inserted
 * @param dir Direction from parent (defaults to right)
 */
    void insert(Node* & pos, Node* prev, int data, direction dir = right);

    /**
 * @brief Rebalances the tree after insertion to maintain Red-Black properties
 * @param node The newly inserted node
 * @param dir Direction of the node relative to its parent
 */
    void insertBalance(Node* node, direction dir);

    /**
 * @brief Removes a node from the Red-Black tree
 * @param toRemove The node to be removed
 */
    void remove(Node *toRemove);

    /**
 * @brief Rebalances the tree after removal to maintain Red-Black properties
 * @param node The node that was removed
 */
    void removeBalance(const Node* node);

    /**
 * @brief Prints the tree structure with indentation based on depth
 * @param pos The current node in traversal
 * @param depth The depth of the current node (for indentation)
 * @param isRight Whether the current node is a right child
 */
    void print(const Node* pos, int depth = 0, bool isRight = false);

    /**
 * @brief Searches for a node with the specified value
 * @param pos The current node in traversal (initially root)
 * @param data The value to search for
 * @return Pointer to the node if found, nullptr otherwise
 */
    static Node* getNode(Node* pos, int data);


    /**
     * @brief Recursively deletes all nodes in a subtree.
     * @param node The root of the subtree to delete.
     */
    void deleteSubtree(Node *node);

    ~RedBlackTree(); //destructor


    Node* root = nullptr; //root of tree
};

#endif
