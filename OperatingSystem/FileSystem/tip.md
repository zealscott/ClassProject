# C语言

- 结构体变量名不是指针
- 读取数据，只需要将char *类型转换为所需要的类型，即可正确读写（例如结构体）
- [可变参数](http://www.runoob.com/cprogramming/c-variable-arguments.html)
- 字符串测长度，`strlen`与`sizeof`的区别
  - https://blog.csdn.net/kangroger/article/details/20653255
  - 数组名不完全等于指针



- 每一次都记得要清空`Path_name`中的值
- 注意`creat`之后，`i-node`没有初始化，不能读，可以通过`block_num`判断（若没有初始化，则为0）
- 读文件，直接将其全部缓存到内存，然后保存一个`fd`偏移量。