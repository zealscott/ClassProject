This project aims to complement a simple 2PL concurrency control.

compile the file:

```shell
clang++ -std=c++11 main.cpp cc_lock.cpp structure.cpp -o p
```

- global.h：全局配置及变量
- structure.h/.cpp：实验所用的数据结构定义及基本操作，采用的基础数据结构是hash map
- cc_lock.h/.cpp：基于锁的并发控制的实现
- cc_occ.h/.cpp：基于乐观并发控制的实现
- cc_to.h/.cpp：基于时间戳的并发控制的实现
- main.cpp：程序入口，测试程序的实现