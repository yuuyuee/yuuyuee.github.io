# 内存一致性模型

## 顺序一致性（Sequential Consistency）

也称为强一致性，也就是访问内存顺序与程序执行顺序一致，不会出现内存重排序。

MIPS R10000

## 完全存储排序（TSO）

允许STORE-LOAD内存访问重排序。

X86_64

## 宽松一致性（Relaxed Memory Consistency）

允许LOAD-LOAD，LOAD-STORE，STORE-LOAD，STORE-STORE内存访问重排序。

ARM，POWER，DEC ALPHA