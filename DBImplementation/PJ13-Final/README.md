---
title: LevelDB Project
categories:
  - DBImplementation
  - Project
mathjax: true
date: 2019-06-19 22:18:03
---

基于LevelDB的大作业。

<!-- more --> 

# Project说明

## 要求

- 对于给定数据集说明如下
  - 只有 key,value 无其它字段
  - 数量级在5万-50万之间
  - 对于主键和非主键都存在过滤条件
- 给定`id`查询的范围，同时给出特定的`Email`，输出该`Email`对应的所有`id`
- 可以自行采取优化方案

## 数据集

| **Id(int, Increment)** | **Email(string)**    |
| ---------------------- | -------------------- |
| 1                      | **xiaoming@ecnu.cn** |
| 2                      | hong@ecnu.cn         |
| 3                      | **xiaoming@ecnu.cn** |
| 4                      | wang@ecnu.cn         |
| …                      | …                    |
| 100                    | li@sjtu.cn           |
| 101                    | wang@sjtu.cn         |
| …                      | …                    |

- `id`为主键，`Email`为value，其中，`id`不重复，`Email`有重复
- 同一个学校后缀的邮箱放在一起

## 说明

- 过滤条件
  - 主键和非主键都存在过滤条件
  - 给定一个较大的id范围区间（涵盖多个不同的学校邮箱）
  - 查询Email相同学校邮箱下的同一个人
- 输出格式
  - 所有的Pair，他们具有相同的Email
  - `<id1,id2> + Email`
- 数据量
  - 5W-50W之间
- 注意
  - 导入之后会自动出发leveldb的compaction，所以存在memtable和sst都有需要查询结果的情况

# 实现

## 思路

1. 首先可以使用最简单的`scan`进行查找输出
2. 为了简单起见，在第一次载入数据后，再跑一遍程序，使得`cmake-build-debug/mydb`中的`.log`文件为空，此时所有数据都在sst中
3. 然后想到如何对于主键进行过滤：在sstable中，每一个table会维护当前key中的值域范围，可以通过该`metadata`对其进行主键过滤
4. 如何对非主键过滤？levelDB提供了布隆过滤器，可以用于非主键的过滤
5. 最后，如何对得到的所有可能kv对根据query进行输出？由于levelDB没有join等复杂的算子，因此可以使用`multiMap`数据结构进行存储。

## 主键过滤

事实上，levelDB已经实现了对于sst的key值划分，但这是根据字典序进行排序的。对于我们这个题来说，`key`都是`id`，因此需要使用整数进行比较，需要我们重写`comparator`函数。

在`util/comparator.cc`文件中，已经实现了默认的key字典序比较函数，因此我们需要继承该函数：

```cpp
class MyComparatorImpl : public Comparator {
        public:
            MyComparatorImpl() {}

            virtual const char *Name() const {
                return "leveldb.MyComparator";
            }

            virtual int Compare(const Slice &a, const Slice &b) const {
                // 用stio会导致level 1没有table，level 2两层table，不知道为啥
                int a_data = std::stoi(a.ToString());
                int b_data = std::stoi(b.ToString());
                if (a_data < b_data)
                    return -1;
                else
                    return +1;
            }

            virtual void FindShortestSeparator(
                    std::string *start,
                    const Slice &limit) const {
            }

            virtual void FindShortSuccessor(std::string *key) const {
            }
        };
```

同时，在`util/options.cc`文件中，需要将默认的`comparator`函数换为我们自定义的函数，这样就实现了重载。

对每个sst，通过对比与query range的大小，即可实现将部分sst进行过滤。

测试函数：

```cpp
// find level min-max
    leveldb::Version *current = db2->GetCurrentVersion();
    std::vector<leveldb::FileMetaData *> files_ = current->GetFiles(1);
    for (int i = 0; i < files_.size(); i++) {
        std::cout << "number: " << files_[i]->number << " ";
        std::cout << "id_min: " << files_[i]->smallest.user_key().ToString() << " ";
        std::cout << "id_max: " << files_[i]->largest.user_key().ToString() << std::endl;
    }
```

## 非主键过滤

非主键过滤是通过布隆过滤器实现的。在levelDB中，已经实现了主键的布隆过滤器，需要我们更改为非主键。

在`table/table.cc`文件中，仿照`InternalGet`函数，构造自己的布隆过滤器查询函数：

```cpp
// new method, to indicate whether the value is in this table
    bool Table::ValueInTable(const leveldb::Slice &value_) {
        Iterator *iiter = rep_->index_block->NewIterator(rep_->options.comparator);
        iiter->SeekToFirst();
        bool flag = false;
        while (iiter->Valid()) {
            Slice handle_value = iiter->value();
            FilterBlockReader *filter = rep_->filter;
            BlockHandle handle;
            if (filter != nullptr &&
                handle.DecodeFrom(&handle_value).ok() &&
                !filter->KeyMayMatch(handle.offset(), value_)) {
//                std::cout << "KeyInTable doesn't hit the filter!" << std::endl;
            } else {
//                std::cout << "KeyInTable hit the filter!" << std::endl;
                flag = true;
                return flag;
            }
            iiter->Next();
        }
//        std::cout << "KeyInTable doesn't hit the filter!" << std::endl;
        return flag;
    }
```

同时，我们需要将默认的主键上的布隆过滤器改为非主键的，在`table/table_builder.cc`文件中修改124行，这里需要注意的是，需要将`value`变为`slice`类型才能作为参数传递，因此需要加入`sequence`满足`slice`约束

```cpp
if (r->filter_block != nullptr) {
//            r->filter_block->AddKey(key);   // 向布隆过滤器添加值
            // 向布隆过滤器添加mail（value），而不是key
            uint64_t seq = 1;
            std::string tmp_value = value.ToString();
            PutFixed64(&tmp_value, seq);
            r->filter_block->AddKey(Slice(tmp_value));
        }
```

这样就实现了简单的对于非主键的过滤优化。

## 读取sst

那么，通过metadata选择了可能存在当前value的sst，如何对sst进行读取呢？

一个简单的测试函数为（助教提供）

```cpp
int main() {
  // open a db
  leveldb::DB *db;
  leveldb::Options options;
//    options.comparator = myCom;
  options.create_if_missing = true;
  std::string dbname_ = "../cmake-build-debug/mydb123";
  leveldb::Status status2 = leveldb::DB::Open(options, dbname_, &db);

  leveldb::Version *current = db->GetCurrentVersion();
  std::vector<leveldb::FileMetaData *> files_ = current->GetFiles(0);

  std::string fname = leveldb::TableFileName(dbname_, files_[0]->number);

  leveldb::RandomAccessFile *file = nullptr;
  leveldb::Table *table = nullptr;
  leveldb::Env *env_ = options.env;
  env_->NewRandomAccessFile(fname, &file);
  leveldb::Table::Open(options, file, files_[0]->file_size, &table);

  leveldb::Iterator *result = table->NewIterator(leveldb::ReadOptions());

  result->SeekToFirst();
  while (result->Valid()) {
    leveldb::ParsedInternalKey ikey;
    leveldb::ParseInternalKey(result->key(), &ikey);
    std::cout << "key:" << ikey.user_key.ToString();
    std::cout << "   value:" << result->value().ToString() << std::endl;
    result->Next();
  }

  delete db;
  return 0;

}
```

## 输出

可以使用`multimap`进行输出，不再赘述

## 总代码

本实验完成的总代码可[参考这里](https://github.com/Scottdyt/ClassProject/tree/master/DBImplementation/PJ13-Final/LeveldbFinal)。入口函数在`test/FinalDemo.cc`中。

