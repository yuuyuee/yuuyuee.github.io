#include <iostream>
#include <stack>
#include <functional>

struct Node {
  Node* left;
  Node* right;
  int value;
};

void NodeInit(Node** node, int value) {
  Node* tmp = new Node;
  tmp->left = tmp->right = nullptr;
  tmp->value = value;
  *node = tmp;
}

void NodeInit2(Node** node) {
  *node = nullptr;
}

int NodeEmpty(const Node* node) {
  return !node ? 1 : 0;
}

// Inorder traverse the binary tree with recursion
void NodeTraverse(const Node* node, const std::function<void(int)>& visitor) {
  if (!NodeEmpty(node)) {
    NodeTraverse(node->left, visitor);
    visitor(node->value);
    NodeTraverse(node->right, visitor);
  }
}

// Inorder traverse the binary tree without recursion and without stack
//
// 1. Initialize current as root
// 2. While current is not NULL
//    If the current does not have left child
//       a) Print current’s data
//       b) Go to the right, i.e., current = current->right
//    Else
//       a) Find rightmost node in current left subtree OR
//               node whose right child == current.
//          If we found right child == current
//              a) Update the right child as NULL of that node whose right child is current
//              b) Print current’s data
//              c) Go to the right, i.e. current = current->right
//          Else
//              a) Make current as the right child of that rightmost
//                 node we found; and
//              b) Go to this left child, i.e., current = current->left
void NodeTraverse2(Node* node, const std::function<void(int)>& visitor) {
  // if (NodeEmpty(node))
  //   return;
  Node* pre;
  Node* cur = node;

  while (cur != nullptr) {
    if (cur->left != nullptr) {
      // find the inorder predecessor of current
      pre = cur->left;
      while (pre->right != nullptr && pre->right != cur)
        pre = pre->right;

      // make sure as the right child of its inorder predecessor
      if (pre->right == nullptr) {
        pre->right = cur;
        cur = cur->left;
      } else {
        // revert the changes mode in the `if' part to restore the
        // original tree i.e., fix the right child of predecessor
        pre->right = nullptr;
        visitor(cur->value);
        cur = cur->right;
      }
    } else {
      visitor(cur->value);
      cur = cur->right;
    }
  }
}

void NodeTraverse3(Node* node, const std::function<void(int)>& visitor) {
  std::stack<Node*> stack;
  while (!NodeEmpty(node) || !stack.empty()) {
    while (!NodeEmpty(node)) {
      stack.push(node);
      node = node->left;
    }
    Node* tmp = stack.top();
    stack.pop();
    visitor(tmp->value);
    node = tmp->right;
  }
}

void NodeInsert(Node** node, int value) {
  if (NodeEmpty(*node)) {
    NodeInit(node, value);
  } else if ((*node)->value >= value) {
    NodeInsert(&(*node)->left, value);
  } else {
    NodeInsert(&(*node)->right, value);
  }
}

void NodeFree(Node* node) {
  if (!NodeEmpty(node)) {
    NodeFree(node->left);
    NodeFree(node->right);
    delete node;
  }
}

int main() {
  Node* tree;
  NodeInit2(&tree);

  NodeInsert(&tree, 10);
  NodeInsert(&tree, 3);
  NodeInsert(&tree, 1);
  NodeInsert(&tree, 5);
  NodeInsert(&tree, 2);
  NodeInsert(&tree, 4);
  NodeInsert(&tree, 6);
  NodeInsert(&tree, 17);
  NodeInsert(&tree, 16);
  NodeInsert(&tree, 21);

  NodeTraverse(tree, [](int value) { std::cout << value << " "; });
  std::cout << std::endl;

  NodeTraverse2(tree, [](int value) { std::cout << value << " "; });
  std::cout << std::endl;

  NodeTraverse(tree, [](int value) { std::cout << value << " "; });
  std::cout << std::endl;

  NodeTraverse3(tree, [](int value) { std::cout << value << " "; });
  std::cout << std::endl;

  NodeTraverse(tree, [](int value) { std::cout << value << " "; });
  std::cout << std::endl;

  NodeFree(tree);
  return 0;
}
