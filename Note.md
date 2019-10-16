# Note
目录下 [*homework.cpp*](/homework.cpp) 是本次分析的源程序，[*homework.ll*](/homework.ll)是经过LLVM编译过后的可读的IR前端文件，编译的命令``` clang -Xclang -disable-O0-optnone -S -emit-llvm homework.cpp -o homework.ll ``` 

虽然LLVM IR是SSA形式的，但是SSA构造算法并不高效，需要各种复杂数据结构，而这些导致前端在生成SSA形式的IR时候十分低效，实际上为了提高前端执行效率，LLVM是将构造SSA的工作从前端分离出来，借助“memory不是SSA value”的特点，把局部变量生成alloca/load/store形式。

这样对局部变量的读写就跟对普通内存的读写一样，不需要SSA形式，这也是为什么我们会在[*homework.ll*](/homework.ll)文件中看到alloca、store、load等操作是直接对变量进行。而为了能够让IR真正进入SSA的形式，需要对[*homework.ll*](/homework.ll)文件进行进一步处理，这里用到LLVM中mem2reg pass，执行命令```opt -mem2reg -S homework.ll -o homework-mem2reg.ll``` 就可以得到真正的SSA形式文件[*homework-mem2reg.ll*](/homework-mem2reg.ll)

### 对比两个文件

源程序[*homework.cpp*](/homework.cpp)和经过部分SSA后的文件[*homework-mem2reg.ll*](/homework-mem2reg.ll)，会发现在SSA形式的[*homework-mem2reg.ll*](/homework-mem2reg.ll)文件中
```c++
  %y.0 = phi i32 [ 10, %if.then ], [ 16, %if.else ]
  %z.0 = phi i32 [ 1, %if.then ], [ 2, %if.else ]
```
```c++
  %y.1 = phi i32 [ %y.0, %if.end ], [ %add, %while.body ]
  %x.0 = phi i32 [ 10, %if.end ], [ %dec, %while.body ]
  %z.1 = phi i32 [ %z.0, %if.end ], [ %inc, %while.body ]
```
```c++
  %add = add nsw i32 %y.1, %x.0
  %dec = add nsw i32 %x.0, -1
  %inc = add nsw i32 %z.1, 1
```
对```x```,```y```和```z```变量都进行了SSA的转换并进行了优化，而对于```ptr```变量则没有进行SSA的转换,因而没有出现在[*homework-mem2reg.ll*](/homework-mem2reg.ll)文件中。

