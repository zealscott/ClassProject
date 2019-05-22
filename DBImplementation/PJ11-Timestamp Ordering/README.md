This project aims to complement a simple Timestamp ordering concurrency control.

compile the file:

```shell
clang++ -std=c++11 main.cpp cc_lock.cpp data_to.cpp -o p
```

- global.h：全局配置及变量
- data_to.h/.cpp：基于时间戳的并发控制的算法
- cc_to.h/.cpp：基于时间戳的读写操作实现
- main.cpp：程序入口，测试程序的实现