

## PostgreSQL 安装

- [Windows](https://www.yiibai.com/postgresql/install-postgresql.html)
- [Linux](https://www.cnblogs.com/freeweb/p/8006639.html)

## 创建数据库和表  

使用SQL语句，完成创建一个数据库，创建关系。

1. 创建数据库scDB；

   ```sql
   create database scDB;
   ```

2. 按要求创建四个表：  

   - Student(Sno,Sname,Ssex,Sage,Sdept)  

     - ```sql
       create table Student  //学生表
       (
       	Sno char(9) unique,
       	Sname char(5) not null,
       	Ssex char(2),
       	Sage smallint,
       	Sdept char(20),
       	primary key(Sno)  // 设置主码为学号
       );
       ```

   - Course(Cno,Cname,Cpno,Ccredits)  

     - ```sql
       create table Course  //课程表
       (
       	Cno char(4),
       	Cname char(20) not null,
       	Cpno char(4),   // 先修课程
       	Ccredits smallint,
       	primary key(Cno),  // 设置主码为课程编号
       	foreign key(Cpno) references Courese(Cno) // 约束条件，Cpno为外码，被参照表Course，被参照列为Cno
       );
       ```

   - SC(Sno,Cno,Grade)

     - ```sql
       create table SC   //学生选课表
       (
       	Sno char(9),
       	Cno char(4),
       	Grade smallint,
       	foreign key(Sno) references Student(Sno), // 约束条件，Cpno为外码，被参照表Course，被参照列为Cno
           foreign key(Cno) references Course(Cno)
       );
       ```

3. 注意，执行SQL脚本时，Postgre不能识别大小写，必须添加双引号，如果有中文，则使用`GB 2312`格式保存：

   ![53930680031](http://wx4.sinaimg.cn/mw690/0060lm7Tly1fw5675efbaj30bw0cigmd.jpg)

4. 执行脚本，注意，在Windows下，**路径要使用正斜杠**

   ![53930664344](http://wx3.sinaimg.cn/mw690/0060lm7Tly1fw564eg8t3j30df06emx6.jpg)

5. 查看表，注意，若有大小写，**也要加引号**：

   ![53930717428](http://wx3.sinaimg.cn/mw690/0060lm7Tly1fw56djj7zej30m506wjrp.jpg)简
   
## 单单表查询  

   - 运行脚本
     - ![53930755651](http://wx4.sinaimg.cn/mw690/0060lm7Tly1fw56k596nkj30da04tjra.jpg)
   - 查看表，其中`S`是供应商表，`P`为零件表，`J`为工程项目表，`SPJ`是供应情况表
     - ![53931042136](http://wx3.sinaimg.cn/mw690/0060lm7Tly1fw57y07ytoj309x066t8k.jpg)


### 课上习题

1. 找出所有供应商的姓名和所在城市

   ```sql
   select sno,sname from s;
   ```

2. 找出所有零件的名称，颜色，重量

   ```sql
   select pname,color,weight from p;
   ```

3. 找出使用供应商`S1`所提供零件的工程号码

   ```sql
   select jno from spj where sno='S1';
   ```

4. 找出工程项目`J2`使用的各种零件的名称及数量

   ```sql
   SELECT spj.qty,p.pname
   FROM spj, p
   WHERE spj.pno = p.pno and spj.jno='J2'; 
   ```

5. 找出上海厂商供应的所有零件号码

   ```sql
   SELECT distinct spj.pno 
   FROM spj, s
   WHERE s.sno = spj.sno and s.city='上海'; 
   ```

6. 找出使用上海产的零件的工程号码

   ```sql
   SELECT distinct spj.jno 
   FROM spj, s
   WHERE s.sno = spj.sno and s.city='上海'; 
   ```

7. 找出没有使用天津产的零件的工程号码

   ```sql
   SELECT distinct spj.jno 
   FROM spj, s
   WHERE s.sno = spj.sno and s.city<>'天津'; 
   ```

8. 把所有红色零件的颜色改成蓝色

   ```sql
   UPDATE p SET color = '蓝' WHERE color = '红' ;
   ```

9. 由`S5`供给`J4`的零件`P6`改为由`S3`供应，请做出必要的修改：

   ```sql
   UPDATE spj SET sno = 'S3' WHERE sno = 'S5' and jno = 'J4' and pno ='P6' ;
   ```

10. 从供应商关系中删除`S2`的记录，并从供应情况关系中删除相应的记录

   ```sql
   DELETE FROM spj WHERE sno = 'S2'; 
   DELETE FROM s WHERE sno = 'S2'; 
   ```

11. 请将（S2，J6，P4,200）插入供应情况关系

    ```sql
    INSERT INTO s VALUES ('S2', '盛锡', 10 ,'北京');
    INSERT INTO spj VALUES ('S2', 'P4', 'J6' ,200);
    ```




## 操作

数据库命令

> \password           设置密码
> \q                  退出
> \h                  查看SQL命令的解释，比如\h select 
> \e                  打开文本编辑器。
> \conninfo           列出当前数据库和连接的信息。
> \\?                  查看psql命令列表
> \du                 列出所有用户。 
> \i yourPath 	运行sql脚本
> 

 数据库操作

> \l                  列出所有数据库
> \c [database_name]  连接其他数据库
> \d                  列出当前数据库的所有表格
> \d [table_name]     列出某一张表格的结构
> drop database db; 	删除一个数据库
> drop table table_name;	删除一个表


