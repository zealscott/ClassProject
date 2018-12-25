本文件包含多线程例子及经典IPC问题原型，注意，在编译时需要加`-lpthread`参数。

# 目录

1. 银行取钱(thread.c)
   - 保证每笔交易都是正确的
2. 哲学家进餐问题(philosopher.c)
   - 多线程编程
   - 使用互斥（mutex）
   - 熟悉使用信号量(semaphore.h)
3. 读者-写者问题(reader-writer.c)
   - 对于多个竞争进程互斥地访问有限资源的建模
   - 本程序中模拟的对数据库的访问
   - 允许不同进程的读，但不允许读写一起进入临界区，也不允许同时写。

# 参考

1. [多线程同步--信号量](https://blog.csdn.net/ooc_zc/article/details/50729607)
2. [sen_init](http://pubs.opengroup.org/onlinepubs/7908799/xsh/sem_init.html)
3. [Linux进程间通信——使用信号量](https://blog.csdn.net/ljianhui/article/details/10243617)
4. [哲学家进餐问题的C语言实现](https://blog.csdn.net/lifupan/article/details/4133790)