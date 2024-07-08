#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct node {
    struct node* left;
    struct node* right;
    struct node* parent;
    int value;
};

// O(h)
struct node* node_minimum(struct node* node) {
    while (node)
        node = node->left;
    return node;
}

// O(h)
struct node* node_maximum(struct node* node) {
    while (node)
        node = node->right;
    return node;
}

struct node* node_alloc(int value) {
    struct node* node = (struct node*) malloc(sizeof(*node));
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->value = value;
    return node;
}

// O(h)
void node_free(struct node* node) {
    while (node) {
        node_free(node->right);
        struct node* ptr = node->left;
        free(node);
        node = ptr;
    }
}

void node_traverse(const struct node* node, void (*visitor)(int)) {
    if (node) {
        node_traverse(node->left, visitor);
        visitor(node->value);
        node_traverse(node->right, visitor);
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
void node_traverse2(const struct node* node, void (*visitor)(int)) {
  struct node* pre;
  struct node* cur = (struct node*) node;

  int i = 0;
  while (cur) {
    if (cur->left) {
      // find the inorder predecessor of current
      pre = cur->left;
      while (pre->right && pre->right != cur)
        pre = pre->right;

      // make sure as the right child of its inorder predecessor
      if (pre->right) {
        // revert the changes mode in the `if' part to restore the
        // original tree i.e., fix the right child of predecessor
        pre->right = NULL;
        visitor(cur->value);
        cur = cur->right;
      } else {
        pre->right = cur;
        cur = cur->left;
      }
    } else {
      visitor(cur->value);
      cur = cur->right;
    }
  }
}

void stack_push(const void*** base, int* top, int* max, const void* value) {
    if (*top >= *max) {
        int new_max = *max == 0 ? 10 : *max * 2;
        *base = realloc(*base, sizeof(**base) * new_max);
        assert(*base && "realloc failed");
        *max = new_max;
    }

    (*base)[((*top)++)] = value;
}

const void* stack_pop(const void** base, int* top) {
    assert(*top >= 0);
    return base[(*top)--];
}

void node_traverse3(const struct node* node, void (*visitor)(int)) {
    const void** base;
    int top = 0, max = 0;

    while (node || top > 0) {
        while (node) {
            stack_push(&base, &top, &max, node);
            node = node->left;
        }

        node = (const struct node*) stack_pop(base, &top);
        visitor(node->value);
        node = node->right;
    }
}

struct binary_search_tree {
    struct node* root;
    size_t size;
};

void bst_init(struct binary_search_tree* tree) {
    tree->root = NULL;
    tree->size = 0;
}


void bst_free(struct binary_search_tree* tree) {
    node_free(tree->root);
    tree->size = 0;
}

int bst_empty(const struct binary_search_tree* tree) {
  return tree->size == 0;
}

// Inorder traverse the binary tree with recursion
void bst_traverse(const struct binary_search_tree* tree, void (*visitor)(int)) {
    node_traverse(tree->root, visitor);
}


void NodeInsert(Node** node, int value) {
  if (NodeEmpty(*node)) {
    NodeInit(node, value);
  } else if ((*node)->value > value) {
    NodeInsert(&(*node)->left, value);
  } else {
    NodeInsert(&(*node)->right, value);
  }

  {
    Node* p = *node;
    while (p) {
      if (p->value > value)
        p = p->left;
      else
        p = p->right;
    }
  }
}

// O(h)
const Node* NodeSearch(const Node* node, int value) {
  while (node) {
    if (node->value == value)
      return node;
    if (node->value > value)
      node = node->left;
    else
      node = node->right;
  }
  return node;
}



int main() {
  Node* tree;
  NodeInit2(&tree);

  NodeInsert(&tree, 10);
  NodeInsert(&tree, 3);
  NodeInsert(&tree, 1);
  NodeInsert(&tree, 5);
  NodeInsert(&tree, 0);
  NodeInsert(&tree, 2);
  NodeInsert(&tree, 4);
  NodeInsert(&tree, 6);
  NodeInsert(&tree, 17);
  NodeInsert(&tree, 15);
  NodeInsert(&tree, 21);
  NodeInsert(&tree, 16);
  NodeInsert(&tree, 18);
  NodeInsert(&tree, 22);

  NodeTraverse(tree, [](int value) { std::cout << value << " "; });
  std::cout << std::endl;

  std::cout << std::endl;
  NodeTraverse2(tree, [](int value) { std::cout << value << " "; });
  std::cout << std::endl;

  // NodeTraverse(tree, [](int value) { std::cout << value << " "; });
  // std::cout << std::endl;

  // NodeTraverse3(tree, [](int value) { std::cout << value << " "; });
  // std::cout << std::endl;

  std::cout << std::endl;
  NodeTraverse(tree, [](int value) { std::cout << value << " "; });
  std::cout << std::endl;

  NodeFree(tree);
  return 0;
}
