#include "RedBlackTree.h"
#include <iostream>

// Corrected constructor name
RedBlackTree::RedBlackTree() = default;


//https://en.wikipedia.org/wiki/Tree_rotation visualization
//node passed in is the root of the subtree (perhaps rename later
//direction is the direction to rotate in
Node* RedBlackTree::rotateSubTree(Node* subRoot, const direction dir) {
    //Will rotate left or right.
    Node* parent = subRoot->parent; //save original root parent

    //New root is now the subRoot's opposite child
    //(of the direction passed in, so if we want to rotate left this would be the right child)
    Node* newRoot = subRoot->child(1 - dir);

    //swap the dir child of the subRoot and newRoot (swap rotation direction child over (the 'inner' child))
    Node* newChild = newRoot->child(dir);
    subRoot->setChild(1 - dir, newChild);
    
    newRoot->setChild(dir, subRoot); //set subRoot as the newRoot's child
    
    //update parents
    if (newChild != nullptr) {
        //for newChild
        newChild->parent = subRoot;
    }
    
    newRoot->parent = parent; //for new root 
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


//Gets the direction of the node passed in relatvie to it's parent
direction RedBlackTree::nodeDirection(Node* node) {
  if (node == node->parent->right) {
    return right;
  } else {
    return left;
  }
}

//u is the node to be swapped, and v is the node to swap with
void RedBlackTree::transplant(Node* u, Node* v) {
    const direction dir = nodeDirection(u); //u's direction relative to parent

    if (u->parent == nullptr) {
        //u is root
        root = v;
    } else if (dir == right) {
        u->parent->setChild(dir, v);
    } else {
        u->parent->setChild(1 - dir, v);
    }

    if (v != nullptr) {
        v->parent = u->parent;
    }
}

//position initially is the root, prev is initially nullptr

//insert function broken (misses 3 when addding?)
void RedBlackTree::insert(Node* & pos, Node* prev, const int data, const direction dir) {
    if (pos == nullptr) {
        pos = new Node(data); //this will be the base case of the recursion (will insert once the path has ended)
        pos->parent = prev; //set parent node
        insertBalance(pos, dir); //do the actual insert
        return;
    }


    //if number is already in the tree
    if (pos->data == data) {
        std::cout << "Already in the tree" << std::endl;
        return;
    }

    //if data being inserted is greater than node then go right
    if (pos->data < data) {
        insert(pos->right, pos, data, right);
    } else if (pos->data > data) {
        //If data being inserted is less than node go left
        insert(pos->left, pos, data, left);
    }
}

// Corrected method definitions
void RedBlackTree::insertBalance(Node* node, direction dir) {
  Node* parent = node->parent;
  
  if (parent == nullptr) {
    node->color = BLACK;
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
     */


    //recursion or iteration....
    do {
      grandparent = parent->parent;
      //Case 1 (requirements are good)
        if (parent->color == BLACK) {
            return; //requirements hold
        }

        //case 4: Parent is red and the root, but the passed in node is also red (default color)
        if (parent->color == RED && grandparent == nullptr) {
            parent->color = BLACK; //switch root color
            return;
        }

        //direction now becomes the parent's direction relative to the grandparent (to get uncle and check for inner and outer children)

	//dir = (parent == grandparent->right) ? right : left; //direction of parent (this was working, changed to function for ease of use)
	dir = nodeDirection(parent);
	uncle = grandparent->child(1 - dir); //opposite parent direction

        //Case 5: If parent is red but uncle is black, then parent would have a red child (but we can't just swap colors like in case 2)
        if ((uncle == nullptr || uncle->color == BLACK) && parent->color == RED) {
            //Check if inner child
            if (node == parent->child(1 - dir)) {
                rotateSubTree(parent, dir);
                //rotate parent to grandparent position (dir is the direction parent is in relative to grandparent).

                //update the node names (since we use pointers, the one at the top should be the parent now)
                node = parent;
                parent = grandparent->child(dir);
            }

            //Case 6 (will come after case 5 since this is what fixes requirement 3, since parent and node could both be red after doing case 5)
            rotateSubTree(grandparent, static_cast<direction>(1 - dir));
            //rotate opposite direction at grandparent (parent is now grandparent)
            parent->color = BLACK;
            grandparent->color = RED;
            return;
        }


        //case 2: If both uncle and parent are red then requirement 4 would be violated when inserting a new node
        if (uncle != nullptr && (parent->color == RED && uncle->color == RED)) {
            parent->color = BLACK;
            uncle->color = BLACK;
            grandparent->color = RED; //this may violate rule 3 if grandparent's parent is red
            node = grandparent; //sets node to grandparent so we can continue checking validity upwards through the tree

	    //grandparent = node->parent->parent;
        }
    }
    while ((parent = node->parent)); //loop while parent is not null (and go up the tree)

    //case 3 (if case 2 has happened enough for height to increase) then every requirement is met (we can basically exit function)

    root->color = BLACK; //make sure root is black (wasn't always set befoer)
}

//remove traversal
void RedBlackTree::remove(Node* toRemove) {
    //Simple casses:
    Node* y = toRemove;

    Color y_color_original = y->color;

    Node* N = nullptr;


    //Node has 1 null child, then swap with it's child and color it black
    //Then the single child must be red, and the dleeted node must be black.
    if (toRemove->right == nullptr) {
        //swap wi
        N = toRemove->right;
        transplant(toRemove, toRemove->right);
    } else if (toRemove->left == nullptr) {
    //swap with left
        N = toRemove->left;
        transplant(toRemove, toRemove->left);
    } else {
        //IF 2 children , swap value with in order sucesssor, and dleete the sucessor instead.
        y = tree_min(toRemove->right);
    }

    //Complex case: if to delete node is black and has no actual children (null). In the first iteration, N will be replaced by null
    if (y_original_color == BLACK && N != NULL) {
        remove(N); // Call to the Wikipedia rebalancing function
    }


    }


}

struct Node* tree_min(struct Node *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}


void RedBlackTree::removeBalance(Node* node) {
  // Implementation for removing a node

  Node* parent = node->parent;
  
  //Initialize then needed nodes
  Node* sibling = nullptr;
  Node* close_nephew = nullptr;
  Node* far_nephew = nullptr;
  direction dir = right; //default direction to right
  
  if (parent != nullptr) {
    dir = nodeDirection(node);
    parent->setChild(dir, nullptr); //set the parents child pointer to null now.
  }
  //now do the balancing
  /* Loop Follows:
     -At the start of each iteration the height of "node" in the tree represent the iteration number (minus 1) (so the first iteration is zero)
     -The number of black nodes on tree paths through "node" is one less than before deletion, but this is unchanged for other paths. This means the parent of "node" has a violation of requirement 4 if other paths exist.
  */
  
  do {
    //case 1:
    if (parent == nullptr) {
      return; //exit if this is the root (tree should be balaced)
    }
    
    //update parameters after every iteration (parent is updated in the while loop):
    dir = nodeDirection(node);
    sibling = parent->child(1 - dir);
    close_nephew = sibling->child(dir); //close = same direction
    far_nephew = sibling->child(1 - dir); //far = opposite direction as node is to it's parent

    //Note: recal that null can also be black (optimize for this, without having such long if statements maybe...)
    
    //Case 3: (sibling is red, so nephews and parent must be black). This rotates the tree so that it can be fixed in cases 4,5, or 6
    if (sibling->color == RED) {
      //(if node is left of parent rotate left, if it's right, do the opposite)
      rotateSubTree(parent, dir); //rotate so that the sibling becomes the new parent (or node's grandparent now) 
      parent->color = RED;
      sibling->color = BLACK;
      sibling = close_nephew; //update sibling pointer (since original sibling isn't actually the sibling anymore)
    } //check if all pointers updated correctly !!!!!!!!!

    //Case 4: Sibling and it's children are black, but parent is red
    if (sibling->color == BLACK && sibling->right->color == BLACK && sibling->left->color == BLACK && parent->color == RED) {
      parent->color = BLACK; //switching the colors will add one to the # of black nodes through 'node's' path (since it's parent will now be black), which makes up for the deleted black nodes without altering the # of black nodes through sibling's path.
      sibling->color = RED;

      return; //after case 4 the tree should be balanced
    }

    //Case 5: Silbing is black, the close child is red, the far child is black.
    if (sibling->color == RED && close_nephew->color == RED && far_nephew->color == BLACK) {
      //Rotate sibling opposite of node's direction (relative to parent). Now the close child takes sibling's place.
      rotateSubTree(sibling, static_cast<direction>(1 - dir));

      //Swap the close child and sibling's colors and update pointer positions
      sibling->color = RED;
      close_nephew->color = BLACK;
      far_nephew = sibling; //sibling would now be in the postiion of node's far nephew
      sibling = close_nephew; //close_nephew is now the actual sibling
    }
    //Now case 5 would move to case 6 to fix the tree

    //Case 6: sibling is black, and it's far child (node's far nephew) is red
    if (sibling->color == BLACK && far_nephew->color == RED) {
      rotateSubTree(parent, dir); //rotate parent so that the sibling is now in its original position

      //swap the parent and sibling colors (only happens if parent would be red)
      sibling->color = parent->color;
      parent->color = BLACK;
      
      far_nephew->color = BLACK; //set the far_nephew color to black

      //Now the subtree still has a root of the same color so requirement 3 is held.
      //The tree paths not passing through "node" also pass through the same number of black nodes as before, but "node now has an extra black ancestor (since parent was shifted to black, or sibling is added as a black grandparent). This means that paths passing through "node" have an additional black node, which fixes requirement 4 from being violated. 
      
      return; //exit loop as the tree should now be balanced after case 6 
    }
    
    //case 2: (if parent, sibling, and sibling's children are black) (req 4 violated)
    if (parent->color == BLACK &&
        sibling->color == BLACK &&
        sibling->left->color == BLACK &&
        sibling->right->color == BLACK) {
      sibling->color = RED; //set sibling color to red
      node = parent; //update the node pointer to become the parent (move up tree by one iteration)
    }

    
  } while (parent = node->parent); //update parent every iteration, will stop once parent is null
}

void RedBlackTree::print(const Node* pos , const int depth, bool isRight) {
    //Take top then right

    if (pos == nullptr) {
        return; //base case
    }

    //Take the right first
    print(pos->right, depth + 1, true);

    //put spaces for the depth (more depth = more spaces)
    for (int i = 0; i < depth; i++) {
        std::cout << "    ";
    }


    //Print a Γ for a right node and L for a left node (helps show the connections of the tree better)
    if (depth > 0 ) {
        std::cout << (isRight ? "Γ" : "L") << " ";
    }
    std::cout << pos->data << " (" << (pos->color == RED ? "R" : "B") << ")" << std::endl; //print out ada and it's color

    print(pos->left, depth + 1, false); //print out left of tree
}

Node* RedBlackTree::getNode(Node* pos, int data) {
  // Implementation for getting a node
  
  if (pos->data == data) {
    return pos;
  }
  
  //if data is greater than node then go right
  if (pos->data < data) {
    return getNode(pos->right, data);
    } else if (pos->data > data) {
    //If data is less than node go left
    return getNode(pos->left, data);
  }
  return nullptr;
}

// Corrected destructor
RedBlackTree::~RedBlackTree() = default;
