# Binary Search Tree

## 特性

当节点$x$拥有左子树$y$和右子树$z$，二叉搜索树总是满足条件$y.key <= x.key <= z.key$

高度为$h$的二叉树最多拥有$2^h-1$个节点

如果节点$x$的右子树为空且有前继$y$，则$x$的最小祖宗是$y$的左子树

最小节点没有左子树，最大节点没有右子树

## 有序遍历方式

中序遍历 (inorder tree walk)

## 操作时间复杂度

| Operator | Times |
|----------|-------|
| Minimum  | O(h)  |
| Maximum  | O(h)  |
| Successor| O(h)  |
| Precessor| O(h)  |
