# Note
目录下 [*homework.cpp*](/homework.cpp) 是本次分析的源程序，[*homework.ll*](/homework.ll)是经过LLVM编译过后的可读的IR前端文件，编译的命令``` clang -Xclang -disable-O0-optnone -S -emit-llvm homework.cpp -o homework.ll ``` 

虽然LLVM IR是SSA形式的，但是这种SSA并不是完全的SSA形式。原因是SSA构造算法需要各种复杂数据结构，而这些导致前端在生成SSA形式的IR时候十分低效，实际上为了提高前端执行效率，LLVM是将构造SSA的工作从前端分离出来，借助“memory不是SSA value”的特点，把局部变量生成alloca/load/store形式。

这样对局部变量的读写就跟对普通内存的读写一样，不需要SSA形式，这也是为什么我们会在[*homework.ll*](/homework.ll)文件中看到alloca、store、load等操作是直接对变量进行。而为了能够让IR真正进入SSA的形式，需要对[*homework.ll*](/homework.ll)文件进行进一步处理，这里用到LLVM中mem2reg pass，执行命令```opt -mem2reg -S homework.ll -o homework-mem2reg.ll``` 就可以得到真正的SSA形式文件[*homework-mem2reg.ll*](/homework-mem2reg.ll)

### 对比两个文件

源程序[*homework.cpp*](/homework.cpp)中构造了一个无法完全转化成SSA的程序，原因是在
```c++
ptr = &x;
```
这段代码中，对x变量进行了取地址的操作，因而x就成为了address-taken类型的变量，而SSA不对该类型变量进行转化。

为了能够与之形成对比，程序中定义了一个变量```y```,不进行取地址的操作，并在上述代码之后与x变量执行相同的操作。

对比不是完全SSA形式的文件[*homework.ll*](/homework.ll)会发现在经过部分SSA后的文件[*homework-mem2reg.ll*](/homework-mem2reg.ll)中
```c++
  %y.0 = phi i32 [ 11, %if.then ], [ 22, %if.else ]
  %0 = load i32, i32* %x, align 4
  %add = add nsw i32 %0, 33
  store i32 %add, i32* %x, align 4
  %add1 = add nsw i32 %y.0, 33
  ret i32* %x
```

```x```变量没有进行SSA的转换和优化，即其内存位置没有，仍然用alloca/load/store对其进行操作，而对```y```变量进行了SSA的转换和优化操作，即其内存位置发生了变化。