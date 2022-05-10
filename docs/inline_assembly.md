# GCC Inline Assembly

## 概述

内联汇编就是一种编写为内联函数的汇编程序，它方便，快速并且在系统编程中非常有用。使用内联汇编需要用到asm关键字，内联汇编之所以重要，主要是因为它能够操作并输出到C变量上，由于这种能力使得asm汇编指令作为接口连接C语言与汇编。

## GCC汇编语法

GCC使用AT&T/UNIX汇编语法，以下描述了AT&T汇编语法与Intel汇编语法的差别（binutils 2.10版本后可以通过.intel_syntax指令来支持Intel汇编语法）。

1. 源-目的操作数顺序

   Intel汇编语法第一个操作数目的操作数，第二个操作数是源操作数，AT&T汇编语法则刚好相反。

   Intel汇编 ：```mov eax, 1```

   AT&T汇编：```movl $1, %eax```

2. 寄存器名称

   AT&T汇编语法中寄存器名有**%**前缀，而Intel则没有。

   Intel汇编 ：```mov eax, 1```

   AT&T汇编：```movl $1, %eax```

3. 立即数操作

   AT&T汇编语法中直接立即数有**$**前缀，其中紧跟着**0x**和立即数表示十六进制；而Intel汇编语法中立即数没有前缀，其中以立即数加**h**后缀表示十六进制。

   Intel汇编 ：10 （十进制）10h（十六进制）

   AT&T汇编：$10 （十进制） $0x10（十六进制）

4. 操作数大小

   AT&T汇编语法中使用操作码后紧跟**b（8-bit）w（16-bit）l（32-bit）**表示内存操作大小；而Intel则通过内存操作前缀**byte ptr，word ptr，dword ptr**来表示。

   Intel汇编 ：```mov a1, byte ptr foo```

   AT&T汇编：```movb foo, %a1```

5. 内存操作

   AT&T汇编语法中基址寄存器（base register）是封闭在（）中的；而Intel汇编语法中则是封闭在[]中的。

   AT&T汇编语法中间接引用：disp（base，index，scale）

   Intel汇编语法中间接引用：[base + index * scal + disp] .

   Intel汇编 ：```sub eax, [ebx + ecx * 4h - 20h]```

   AT&T汇编：```sub -0x20(%ebx, %ecx, 04), %eax```



| Intel Code                      | AT&T Code                               |
| ------------------------------- | --------------------------------------- |
| ```move eax, 1```               | ```movl $1, %eax```                     |
| ```mov ebx, 0ffh```             | ```movl $0xff, %ebx```                  |
| ```int 80h```                   | ```int $0x80```                         |
| ```mov ebx, eax```              | ```movl %eax, %ebx```                   |
| ```mov eax, [ecx]```            | ```movl (%ecx), %ebx```                 |
| ```mov eax, [ebx+3]```          | ```movl 3(%ebx), %eax```                |
| ```mov eax, [ebx+20h]```        | ```movl 0x20(%ebx), $eax```             |
| ```add eax, [ebx+ecx*2h]```     | ```add (%ebx, %ecx, 0x2), %eax```       |
| ```lea eax, [ebx+ecx]```        | ```leal (%ebx, %ecx), %eax```           |
| ```sub eax, [ebx+ecx*4h-20h]``` | ```subl -0x20(%ebx, %ecx, 0x4), %eax``` |

## 基本格式

**asm**关键字和**\__asm__**是等同的。

单行示例：

```c
/* moves the contents of ecx to eax */
asm("movl %ecx, %eax");

/* moves the byte from bh to the memory pointed by eax */
__asm__("mov1 %bh, (%eax)");
```

多行示例：

```c
__asm__(
	"movl %eax, %bx \n\t"
    "movl $56, %esi \n\t"
    "mol %ecx, $label(%edx, %ebx, $4) \n\t"
    "mob %ah, (%ebx)"
);
```

## 扩展汇编

扩展汇编允许指定（操作数）对应的输入寄存器，输出寄存器以及多个变更寄存器（clobbered registers）。

```
asm (
	assembler template
	: output operands             /* optional */
	: input operands              /* optional */
	: list of clobbered registers /* optional */
);
```

示例：

```c
int a = 10, b;
// b = a;
asm (
	"movl %1, %%eax \n\t"
    "movl %%eax, %0"
    : "=r" (b)
    : "r" (a)
    : "%eax"
);
/*
 * "b" is the output operand referred to by %0 and "a" is the input operand referred to by %1.
 * "r" is a constraint on the operands that says to GCC to use any register for storing the operands.
 * "=" says that is output operand and it write-only.
 * There are two %% prefixed to the register name to helps GCC to distinguish between the operands and registers. operands have a single % as prefix.
 * The clobbered register %eax tells GCC that the value of %eax is to be modified inside "asm", so GCC won't use this register to store any other value.
 * When the execution of "asm" is complete. The change made to "b" inside "asm" is supposed to be reflected output the "asm".
 */
```

### 汇编器模板(Assembler Temmplate)

```assembly
; Example 1
"mov1 %%1, %%eax;
 mov1 %%eax, %0;"

; Example 2
"mov1 %%1, %%eax \n\t"
"mov1 %%eax, %0 \n\t"
```

### 操作数(Operands)

格式：```"constraint" (C expression)```

在汇编器模板中每个操作数通过数字引用，包括输出和输入操作数从0开始依次依序递增。假设总计有n个操作数则第一个输出操作数为%0，最后一个输入操作数为 %n-1。

输出操作数必须是一个左值。输入则不限定。如果输出操作数不能直接寻址，可以使用一个寄存器，这时候GCC将使用这个寄存器作为输出并随后存储该寄存器值。

```assembly
; void func(int* v, int x) {
; *v = x + x * 4;
	asm(
		"leal (%1, %1, 4), %0"
		: "=r" (*v)
		: "r" (x) 
	);
;}
```

输入和输出使用相同寄存器，不指定特定寄存器

```assembly
; void func(int* v, int x) {
; *v = x + x * 4;
	asm(
		"leal (%0, %0, 4), %0"
		: "=r" (*v)
		: "0" (x) 
	);
;}
```

输入和输出使用相同寄存器，指定特定寄存器

```assembly
; void func(int* v, int x) {
; *v = x + x * 4;
	asm(
		"leal (%%ecx, %%ecx, 4), %%ecx"
		: "=c" (*v)
		: "c" (x) 
	);
;}
```

### 破坏寄存器列表（Clobber List）

一些指令会破坏硬件寄存器，将这些寄存器加入到clobber-list通知GCC这些寄存器被修改，GCC将不会假设加载到这些寄存器中的值是有效的。

这里不应该加入输入和输出寄存器，应为GCC知道使用它们了。

如果指令改变了条件代码寄存器，必须把“cc”添加到这个列表中。

如果指令修改了内存，必须把"memory"添加到这个列表中，这将导致GCC不会在汇编指令中将内存值缓存到寄存器中。如果受影响的内存未在asm的输入和输出中，还必须添加**volatile**关键字。

### Volatile

如果汇编语句必须在放置的地方执行，不能作为优化移除循环，需要将关键字**volatile**放置在asm和（）之前放置移动和删除（优化）。

```c
asm volatile (...);
```

当需要非常小心的时候使用**__volatile__**。

当汇编代码仅做一些计算，没有其他影响时，避免使用**volatile**以助于GCC优化。

## 更多关于约束

### 常用约束

1. 寄存器操作约束（r）

   当使用此越苏指定操作数时，它们将存储在通用寄存器(GPR)中。

   ```c
   asm ("movl %%eax, %0\n" :"=r"(myval));
   ; 这里变量myval保持在寄存器中，寄存器中的值eax被复制到寄存器中，并且myval从寄存器更新到内存中。
   ; 当使用r约束时，可以将变量保存到任意可用GPR中。
   ```

   | ```r``` | Register(s)          |
   | ------- | -------------------- |
   | ```a``` | ```%eax, %ax, %al``` |
   | ```b``` | ```%ebx, %bx, %bl``` |
   | ```c``` | ```%ecx, %cx, %cl``` |
   | ```d``` | ```%edx, %dx, %dl``` |
   | ```S``` | ```%esi, %si```      |
   | ```D``` | ```%edi, %di```      |

   ​

2. 内存操作约束（m）

   当操作数在内存中时，对它们的任何操作都将直接发生在内存位置，这与寄存器约束相反，寄存器约束首先将值存储在要修改的寄存器中给，然后将其回写到内存位置。但寄存器约束通常进在指令绝对必要或者显著加速进程时才使用。

   在需要在asm中更新C变量并且确实不想使用寄存器保存其值的时候，可以最有效的使用内存约束。

   ```c
   asm("sidt %0\n" : :"m"(loc));
   ```

   ​

3. 匹配（数字）约束

   某些情况下，变量可以同时作为输入和输出操作数。这种情况可以通过使用匹配约束在asm中指定。

   ```c
   asm ("incl %0" :"=a"(var):"0"(var));
   ```

   这里0指定与第0个输出变量约束相同。

   以下情况下可以使用匹配约束：

   * 在从变量读取输入或修改变量并值写回同一个变量的情况下。
   * 在不需要输入和输出操作数的单独实例的情况下。

4. 其他约束

   | ```m``` | 允许使用内存操作数，通常支持任意类型的地址                    |
   | ------- | ---------------------------------------- |
   | ```o``` | 允许使用内存操作数，但前提是地址可偏移，即向地址添加一个小的偏移量会给出一个有效的地址 |
   | ```V``` | 不可偏移的内存操作数（任何符合m约束但不符合o的约束）              |
   | ```i``` | 允许立即整数操作数（具有常数值的操作数），包括符号常量              |
   | ```n``` | 允许使用一直的立即整数操作数                           |
   | ```g``` | 允许任意寄存器，内存或立即整数操作数但不是通用寄存器               |

5. 特定于x86约束

   | ```r``` | 见上                                       |
   | ------- | ---------------------------------------- |
   | ```q``` | 寄存器a b c d                               |
   | ```I``` | 约束范围0-31（对于32位移位）                        |
   | ```J``` | 约束范围0-63（对于64位移位）                        |
   | ```K``` | 0xff                                     |
   | ```L``` | 0xffff                                   |
   | ```M``` | 0 1 2 3（lea指令的移位）                        |
   | ```N``` | 0-255常数（用于out指令）                         |
   | ```f``` | 浮点寄存器                                    |
   | ```t``` | 第一个（栈顶）浮点寄存器                             |
   | ```u``` | 第二个浮点寄存器                                 |
   | ```A``` | 指定a或d寄存器，主要用于返回64位整数值，d寄存器保存最高有效位，a寄存器保存最低有效位 |

### 约束修饰符

* = 表示该操作数对于该指令只写
* &表示该操作数是一个最近早前破坏的操作数，指令完成之前使用输入操作数进行修改。

## 有用的诀窍

求和

```c
int main(void)
{
	int foo = 10, bar = 15;
 	__asm__ __volatile__(
      "addl  %%ebx,%%eax"
      :"=a"(foo)
      :"a"(foo), "b"(bar)
    );
	printf("foo+bar=%d\n", foo);
	return 0;
}
```

原子求和

```c
 __asm__ __volatile__(
	"   lock       ;\n"
	"   addl %1,%0 ;\n"
	: "=m"  (my_var)
	: "ir"  (my_int), "m" (my_var)
	: /* no clobber-list */
);
```

比较值

```c
__asm__ __volatile__(
  "decl %0; sete %1"
  : "=m" (my_var), "=q" (cond)
  : "m" (my_var)
  : "memory"
);
```

设置/清楚寄存器的位

```c
__asm__ __volatile__(
	"btsl %1,%0"
  	: "=m" (ADDR)
  	: "Ir" (pos)
  	: "cc"
);
```

复制字符串

```c
static inline char * strcpy(char * dest,const char *src) {
  int d0, d1, d2;
  __asm__ __volatile__(  "1:\tlodsb\n\t"
                         "stosb\n\t"
                         "testb %%al,%%al\n\t"
                         "jne 1b"
                       : "=&S" (d0), "=&D" (d1), "=&a" (d2)
                       : "0" (src),"1" (dest) 
                       : "memory");
  return dest;
}

#define mov_blk(src, dest, numwords) \
__asm__ __volatile__ (                                          \
                       "cld\n\t"                                \
                       "rep\n\t"                                \
                       "movsl"                                  \
                       :                                        \
                       : "S" (src), "D" (dest), "c" (numwords)  \
                       : "%ecx", "%esi", "%edi"                 \
                       )

```

linux系统调用

```c
#define _syscall3(type,name,type1,arg1,type2,arg2,type3,arg3) \
type name(type1 arg1,type2 arg2,type3 arg3) \
  { \
  long __res; \
  __asm__ volatile (  "int $0x80" \
                    : "=a" (__res) \
                    : "0" (__NR_##name),"b" ((long)(arg1)),"c" ((long)(arg2)), \
                      "d" ((long)(arg3))); \
  __syscall_return(type,__res); \
}
```

系统调用Exit

```c
{
        asm("movl $1,%%eax;         /* SYS_exit is 1 */
             xorl %%ebx,%%ebx;      /* Argument is in ebx, it is 0 */
             int  $0x80"            /* Enter kernel mode */
             );
}
```
