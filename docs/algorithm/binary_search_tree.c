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

// O(h)
struct node* node_predecessor(struct node* node) {
    assert(node);

    if (node->left)
        return node_maximum(node->left);
    return NULL;
}

// O(h)
struct node* node_successor(struct node* node) {
    assert(node);

    if (node->right)
        return node_minimum(node->right);

    struct node* ptr = node->parent;
    while (ptr && ptr->right != node) {
        node = ptr;
        ptr = node->parent;
    }
    return ptr;
}

struct node* node_alloc(int value) {
    struct node* node = (struct node*) malloc(sizeof(*node));
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->value = value;
    return node;
}

void node_free(struct node* node) {
    while (node) {
        node_free(node->left);
        struct node* ptr = node;
        node = node->right;
        free(ptr);
    }
}

void node_traverse(const struct node* node, void (*visitor)(int)) {
    while (node) {
        node_traverse(node->left, visitor);
        visitor(node->value);
        node = node->right;
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

    while (cur) {
        if (cur->left) {
            // find the inorder predecessor of current
            pre = cur->left;
            while (pre->right && pre->right != cur)
                pre = pre->right;

            if (pre->right) {
                // revert the changes mode in the `if' part to restore the
                // original tree i.e., fix the right child of predecessor
                pre->right = NULL;
                visitor(cur->value);
                cur = cur->right;
            } else {
                // make sure as the right child of its inorder predecessor
                pre->right = cur;
                cur = cur->left;
            }
        } else {
            visitor(cur->value);
            cur = cur->right;
        }
    }
}

struct stack {
    const void** base;
    int top;
    int size;
};

void stack_init(struct stack* stack) {
    stack->base = NULL;
    stack->top = 0;
    stack->size = 0;
}

void stack_free(struct stack* stack) {
    if (stack->base)
        free(stack->base);
}

void stack_extend(struct stack* stack, int size) {
    assert(stack->size < size);
    stack->base = stack->base
            ? realloc(stack->base, size * sizeof(*stack->base))
            : malloc(size * sizeof(*stack->base));
    stack->size = size;
}

void stack_push(struct stack* stack, const void* value) {
    if (stack->top == stack->size) {
        int new_size = stack->size == 0 ? 64 : stack->size * 2;
        stack_extend(stack, new_size);
    }
    stack->base[stack->top++] = value;
}

const void* stack_pop(struct stack* stack) {
    assert(stack->top >= 0);
    return stack->base[--stack->top];
}

int stack_empty(const struct stack* stack) {
    return stack->top == 0;
}

void node_traverse3(const struct node* node, void (*visitor)(int)) {
    struct stack stack;

    stack_init(&stack);
    while (node || !stack_empty(&stack)) {
        while (node) {
            stack_push(&stack, node);
            node = node->left;
        }
        node = (const struct node*) stack_pop(&stack);
        visitor(node->value);
        node = node->right;
    }
    stack_free(&stack);
}

// O(h)
void node_insert(struct node* node, int value) {
    assert(node);
    struct node* parent = NULL;
    struct node* new_node = node_alloc(value);

    while (node) {
        parent = node;
        node = node->value > value ? node->left : node->right;
    }
    new_node->parent = parent;
    if (parent->value > value) parent->left = new_node;
    else parent->right = new_node;
}

// O(h)
struct node* node_search(struct node* node, int value) {
    assert(node);
    while (node && node->value != value)
        node = node->value > value ? node->left : node->right;
    return node;
}

void node_delete(struct node* node, int value) {

}

struct bst {
    struct node* root;
    size_t size;
};

void bst_init(struct bst* tree) {
    tree->root = NULL;
    tree->size = 0;
}

void bst_free(struct bst* tree) {
    node_free(tree->root);
    tree->size = 0;
}

int bst_empty(const struct bst* tree) {
    return tree->size == 0;
}

// Inorder traverse the binary tree with recursion
void bst_traverse(const struct bst* tree, void (*visitor)(int)) {
    node_traverse2(tree->root, visitor);
}

void bst_traverse2(const struct bst* tree, void (*visitor)(int)) {
    node_traverse2(tree->root, visitor);
}

void bst_traverse3(const struct bst* tree, void (*visitor)(int)) {
    node_traverse3(tree->root, visitor);
}

void bst_insert(struct bst* tree, int value) {
    if (bst_empty(tree))
        tree->root = node_alloc(value);
    else
        node_insert(tree->root, value);
    ++tree->size;
}

int* bst_search(struct bst* tree, int value) {
    if (bst_empty(tree))
        return NULL;
    struct node* node = node_search(tree->root, value);
    return node ? &node->value : NULL;
}

void bst_delete(struct bst* tree, int value) {
    if (!bst_empty(tree)) {
        node_delte(tree->root, value);
        --tree->size;
    }
}

void visitor(int v) { printf("%d ", v); }

int main() {
    struct bst tree;
    bst_init(&tree);

    bst_insert(&tree, 10);
    bst_insert(&tree, 3);
    bst_insert(&tree, 1);
    bst_insert(&tree, 5);
    bst_insert(&tree, 0);
    bst_insert(&tree, 2);
    bst_insert(&tree, 4);
    bst_insert(&tree, 6);
    bst_insert(&tree, 17);
    bst_insert(&tree, 15);
    bst_insert(&tree, 21);
    bst_insert(&tree, 16);
    bst_insert(&tree, 18);
    bst_insert(&tree, 22);

    bst_traverse(&tree, visitor);
    printf("\n");

    bst_traverse2(&tree, visitor);
    printf("\n");

    bst_traverse3(&tree, visitor);
    printf("\n");

    bst_free(&tree);
    return 0;
}
