This project aims to complement a simple Optimistic Concurrency Control.

compile the file:

```shell
clang++ -std=c++11 main.cpp cc_occ.cpp data_occ.cpp -o p
```

- global.h：全局配置及变量
- data_to.h/.cpp：基于时间戳的并发控制的算法
  - 在`update_buffer`中，由于写操作会引起读操作和预写操作的连锁反应
  - 在读操作中，首先取出需要读的`readExcuteEntry`，然后将`data_`的值赋到`data`中，这样用户就可以看见了。
  - 在写操作中，通过`delete_buffer`找到需要执行写操作的队列，然后找到时间戳最大的进行执行。
- cc_to.h/.cpp：基于时间戳的读写操作实现
- main.cpp：程序入口，测试程序的实现