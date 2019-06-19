// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// The representation of a DBImpl consists of a set of Versions.  The
// newest version is called "current".  Older versions may be kept
// around to provide a consistent view to live iterators.
//
// Each Version keeps track of a set of Table files per level.  The
// entire set of versions is maintained in a VersionSet.
//
// Version,VersionSet are thread-compatible, but require external
// synchronization on all accesses.

#ifndef STORAGE_LEVELDB_DB_VERSION_SET_H_
#define STORAGE_LEVELDB_DB_VERSION_SET_H_

#include <map>
#include <set>
#include <vector>
#include "db/dbformat.h"
#include "db/version_edit.h"
#include "port/port.h"
#include "port/thread_annotations.h"

namespace leveldb {

    namespace log { class Writer; }

    class Compaction;

    class Iterator;

    class MemTable;

    class TableBuilder;

    class TableCache;

    class Version;

    class VersionSet;

    class WritableFile;

// Return the smallest index i such that files[i]->largest >= key.
// Return files.size() if there is no such file.
// REQUIRES: "files" contains a sorted list of non-overlapping files.

    // 给定一组 FileMetaData[], 二分查找，找出 key 可能所在的 FileMetaData,
    // 需要保证 FileMetaData[] 有序, 且没有键的重叠
    // 返回所在文件的索引 (0 < 索引 < file.size()), 若没有找到, 返回 file.size()
    // 由于 files 是有序的，这里只找了第一个 key<largest_key 的文件，但是很有可能 key<smallist_key
    // 所以只是有可能存在于某个 file 中
    int FindFile(const InternalKeyComparator &icmp,
                 const std::vector<FileMetaData *> &files,
                 const Slice &key);

// Returns true iff some file in "files" overlaps the user key range
// [*smallest,*largest].
// smallest==nullptr represents a key smaller than all keys in the DB.
// largest==nullptr represents a key largest than all keys in the DB.
// REQUIRES: If disjoint_sorted_files, files[] contains disjoint ranges
//           in sorted order.
    // smallest==nullptr represents a key smaller than all keys in the DB.
    // largest==nullptr represents a key largest than all keys in the DB.

    // 返回 [*smallest_user_key, *largest_user_key] 是否存在于 level[i] 的文件 files 中
    bool SomeFileOverlapsRange(const InternalKeyComparator &icmp,
                               bool disjoint_sorted_files,  // level > 0 ? true : false, 表示 files 内文件（或key）是否有序
                               const std::vector<FileMetaData *> &files,
                               const Slice *smallest_user_key,
                               const Slice *largest_user_key);

    class Version {
    public:
        // Append to *iters a sequence of iterators that will
        // yield the contents of this Version when merged together.
        // REQUIRES: This version has been saved (see VersionSet::SaveTo)

        // *iters 指向一组迭代器，迭代各层的文件
        // 具体指向：
        //      iter[0] -> files_[0][0]
        //      iter[i] -> files_[0][i]
        //      iter[n] -> files_[0][n]
        //      iter[n+1] -> files_[1]
        //      iter[n+i] -> files_[i] (i>1)
        //      iter[n+3] -> files_[n]
        // 这里 files_[0][i]、files_[i] 并不是文件，而是这个迭代器指向的二级迭代器
        // 二级迭代器的概念见 table 中的定义
        void AddIterators(const ReadOptions &, std::vector<Iterator *> *iters);

        // Lookup the value for key.  If found, store it in *val and
        // return OK.  Else return a non-OK status.  Fills *stats.
        // REQUIRES: lock is not held
        struct GetStats {
            FileMetaData *seek_file;
            int seek_file_level;
        };

        // 给定 key,查找 value
        // 先查找 level[0],然后 level[i] (i>0)
        // TODO: stats 不清楚干嘛用
        Status Get(const ReadOptions &, const LookupKey &key, std::string *val,
                   GetStats *stats);

        // Adds "stats" into the current state.  Returns true if a new
        // compaction may need to be triggered, false otherwise.
        // REQUIRES: lock is held
        // TODO:
        // 用于判断是否执行 DB::Impl::MaybeScheduleCompaction()
        bool UpdateStats(const GetStats &stats);

        // Record a sample of bytes read at the specified internal key.
        // Samples are taken approximately once every config::kReadBytesPeriod
        // bytes.  Returns true if a new compaction may need to be triggered.
        // REQUIRES: lock is held
        // 用于判断是否执行 DB::Impl::MaybeScheduleCompaction()
        bool RecordReadSample(Slice key);

        // Reference count management (so Versions do not disappear out from
        // under live iterators)
        void Ref();

        void Unref();

        // 给定 begin、end, 返回该层中，哪些文件和这个范围内的数据项有重合
        // 返回的是 inputs
        void GetOverlappingInputs(
                int level,
                const InternalKey *begin,         // nullptr means before all keys
                const InternalKey *end,           // nullptr means after all keys
                std::vector<FileMetaData *> *inputs);

        // Returns true iff some file in the specified level overlaps
        // some part of [*smallest_user_key,*largest_user_key].
        // smallest_user_key==nullptr represents a key smaller than all the DB's keys.
        // largest_user_key==nullptr represents a key largest than all the DB's keys.

        // 返回 [*smallest_user_key, *largest_user_key] 是否存在于 level 中
        bool OverlapInLevel(int level,
                            const Slice *smallest_user_key,
                            const Slice *largest_user_key);

        // Return the level at which we should place a new memtable compaction
        // result that covers the range [smallest_user_key,largest_user_key].

        // DB::Impl::WriteLevel0Table() 在讲 memtable 刷盘时，先构建 FileMetaData,写入 memtable 数据
        // 然后这个 FileMetaData 必然要添加到某个 version 的 level 和 files_ 中去
        // 这里并没有直接写入 level[0]
        //      而是先判断 memtable 中的数据和 0 层有没有重合，
        //          若有，则添加到 0 层，后面要compation
        //          若没有，找到合适的 i 层(i > 0),这里合适指 memtable 和 i 层 也没有重合，则直接放到 i 层
        int PickLevelForMemTableOutput(const Slice &smallest_user_key,
                                       const Slice &largest_user_key);

        int NumFiles(int level) const { return files_[level].size(); }

        // Return a human readable string that describes this version's contents.
        std::string DebugString() const;

        std::vector<FileMetaData *> GetFiles(int level) {
            return files_[level];
        }

        int GetLevelNums(){
            return config::kNumLevels;
        }

    private:
        friend class Compaction;

        friend class VersionSet;

        class LevelFileNumIterator;

        // 返回 level[i] (i>0) 二级迭代器，Version::AddIterators() 会用到
        Iterator *NewConcatenatingIterator(const ReadOptions &, int level) const;

        // Call func(arg, level, f) for every file that overlaps user_key in
        // order from newest to oldest.  If an invocation of func returns
        // false, makes no more calls.
        //
        // REQUIRES: user portion of internal_key == user_key.
        // Version::RecordReadSample() 会用到
        void ForEachOverlapping(Slice user_key, Slice internal_key,
                                void *arg,
                                bool (*func)(void *, int, FileMetaData *));

        VersionSet *vset_;            // VersionSet to which this Version belongs
        Version *next_;               // Next version in linked list
        Version *prev_;               // Previous version in linked list
        int refs_;                    // Number of live refs to this version

        // List of files per level
        std::vector<FileMetaData *> files_[config::kNumLevels];

        // Next file to compact based on seek stats.
        FileMetaData *file_to_compact_;
        int file_to_compact_level_;

        // Level that should be compacted next and its compaction score.
        // Score < 1 means compaction is not strictly needed.  These fields
        // are initialized by Finalize().

        // 用于 VersionSet::Finalize(), 这个函数从某个 version 中选出某一 level 用于合并，
        // 该level 一定是最需要合并的
        // 如何判断最需要合并 ? 每层都定义了 MaxBytesForLevel(), 用当前层的所有文件大小/MaxBytesForLevel(),
        // 得分最大，则更新 compaction_score_、compaction_level_
        double compaction_score_;
        int compaction_level_;

        explicit Version(VersionSet *vset)
                : vset_(vset), next_(this), prev_(this), refs_(0),
                  file_to_compact_(nullptr),
                  file_to_compact_level_(-1),
                  compaction_score_(-1),
                  compaction_level_(-1) {
        }

        ~Version();

        // No copying allowed
        Version(const Version &);

        void operator=(const Version &);
    };

    class VersionSet {
    public:
        VersionSet(const std::string &dbname,
                   const Options *options,
                   TableCache *table_cache,
                   const InternalKeyComparator *);

        ~VersionSet();

        // Apply *edit to the current version to form a new descriptor that
        // is both saved to persistent state and installed as the new
        // current version.  Will release *mu while actually writing to the file.
        // REQUIRES: *mu is held on entry.
        // REQUIRES: no other thread concurrently calls LogAndApply()
        Status LogAndApply(VersionEdit *edit, port::Mutex *mu)
        EXCLUSIVE_LOCKS_REQUIRED(mu);

        // Recover the last saved descriptor from persistent storage.
        Status Recover(bool *save_manifest);

        // Return the current version.
        Version *current() const { return current_; }

        // Return the current manifest file number
        uint64_t ManifestFileNumber() const { return manifest_file_number_; }

        // Allocate and return a new file number
        uint64_t NewFileNumber() { return next_file_number_++; }

        // Arrange to reuse "file_number" unless a newer file number has
        // already been allocated.
        // REQUIRES: "file_number" was returned by a call to NewFileNumber().
        void ReuseFileNumber(uint64_t file_number) {
            if (next_file_number_ == file_number + 1) {
                next_file_number_ = file_number;
            }
        }

        // Return the number of Table files at the specified level.
        int NumLevelFiles(int level) const;

        // Return the combined file size of all files at the specified level.
        int64_t NumLevelBytes(int level) const;

        // Return the last sequence number.
        uint64_t LastSequence() const { return last_sequence_; }

        // Set the last sequence number to s.
        void SetLastSequence(uint64_t s) {
            assert(s >= last_sequence_);
            last_sequence_ = s;
        }

        // Mark the specified file number as used.
        void MarkFileNumberUsed(uint64_t number);

        // Return the current log file number.
        uint64_t LogNumber() const { return log_number_; }

        // Return the log file number for the log file that is currently
        // being compacted, or zero if there is no such log file.
        uint64_t PrevLogNumber() const { return prev_log_number_; }

        // Pick level and inputs for a new compaction.
        // Returns nullptr if there is no compaction to be done.
        // Otherwise returns a pointer to a heap-allocated object that
        // describes the compaction.  Caller should delete the result.

        // leveldb 中有两种情况会触发合并操作：
        //      1 是 compaction_score_ > 1, 即当前层的文件总大小大于 MaxFileSizeForLevel()
        //      2 是 seeks 引起的 TODO: what does seeks mean?
        // leveldb 更倾向于由 compaction_score_ > 1 引起的合并
        Compaction *PickCompaction();

        // Return a compaction object for compacting the range [begin,end] in
        // the specified level.  Returns nullptr if there is nothing in that
        // level that overlaps the specified range.  Caller should delete
        // the result.
        Compaction *CompactRange(
                int level,
                const InternalKey *begin,
                const InternalKey *end);

        // Return the maximum overlapping data (in bytes) at next level for any
        // file at a level >= 1.
        // 从 level >= 1开始，寻找 file[level][i] 中和 level+1 重叠的文件总数
        // 返回 file[level+1] 中这些文件总大小
        // 注意是 level 中单个文件和 level+1 中所有文件比较
        // 而不是 level 中所有文件和 level+1 中所有文件比较
        int64_t MaxNextLevelOverlappingBytes();

        // Create an iterator that reads over the compaction inputs for "*c".
        // The caller should delete the iterator when no longer needed.

        // 和 merger.h 中 NewMergingIterator() 呼应上
        // TODO:
        Iterator *MakeInputIterator(Compaction *c);

        // Returns true iff some level needs a compaction.
        // 关于 compaction_score_ 和 file_to_compact_，见 PickCompaction()
        bool NeedsCompaction() const {
            Version *v = current_;
            return (v->compaction_score_ >= 1) || (v->file_to_compact_ != nullptr);
        }

        // Add all files listed in any live version to *live.
        // May also mutate some internal state.
        // 这里 live 指系统中现有所有文件
        // 迭代每个 version, 然后把 version.files_ 中的所有文件号添加到 lives 中
        void AddLiveFiles(std::set<uint64_t> *live);

        // Return the approximate offset in the database of the data for
        // "key" as of version "v".
        // 返回某个 Version 版本中 key 对应的偏移量
        // 这个偏移量为之前所有小于 key 的文件大小 加上 该 key 所在 SSTable 内的偏移量
        uint64_t ApproximateOffsetOf(Version *v, const InternalKey &key);

        // Return a human-readable short (single-line) summary of the number
        // of files per level.  Uses *scratch as backing store.
        struct LevelSummaryStorage {
            char buffer[100];
        };

        // 返回各层 level 包含的 SSTable 文件数量
        // 示例:
        //      files[ 1 2 3 4 5 6 7 ]
        const char *LevelSummary(LevelSummaryStorage *scratch) const;

    private:
        class Builder;

        friend class Compaction;

        friend class Version;

        bool ReuseManifest(const std::string &dscname, const std::string &dscbase);

        // 这个函数从某个 version 中选出某一 level 用于合并，
        // 该level 一定是最需要合并的
        // 如何判断最需要合并 ? 每层都定义了 MaxBytesForLevel(), 用当前层的所有文件大小/MaxBytesForLevel(),
        // 得分最大，则更新 compaction_score_、compaction_level_
        void Finalize(Version *v);

        // 给定某层 level 的文件 inputs,
        // 返回最小的 smallest 和最大的 largest InternalKey
        void GetRange(const std::vector<FileMetaData *> &inputs,
                      InternalKey *smallest,
                      InternalKey *largest);

        // 同样给定 2 个inputs,
        // 返回最小的 smallest 和最大的 largest InternalKey
        // 内部调用 GetRange 实现
        void GetRange2(const std::vector<FileMetaData *> &inputs1,
                       const std::vector<FileMetaData *> &inputs2,
                       InternalKey *smallest,
                       InternalKey *largest);

        // 此时 c 已经初始化, 即 c 的 level_、input_version_、inputs_ 等均非空
        // TODO:
        void SetupOtherInputs(Compaction *c);

        // Save current contents to *log
        // 内部构建 VersionEdit edit,
        // 把 icmp_、 compact_pointer_、 current_->files_放入 edit,
        // 然后把 edit.EncodeTo() 追加到 log 后
        Status WriteSnapshot(log::Writer *log);

        // 在双向链表最后添加 Version 节点。current_ 指向新添加的节点。
        void AppendVersion(Version *v);

        Env *const env_;
        const std::string dbname_;
        const Options *const options_;
        TableCache *const table_cache_;
        const InternalKeyComparator icmp_;
        uint64_t next_file_number_;
        uint64_t manifest_file_number_;
        uint64_t last_sequence_;
        uint64_t log_number_;
        uint64_t prev_log_number_;  // 0 or backing store for memtable being compacted

        // Opened lazily
        WritableFile *descriptor_file_;
        log::Writer *descriptor_log_;
        Version dummy_versions_;  // Head of circular doubly-linked list of versions.
        Version *current_;        // == dummy_versions_.prev_

        // Per-level key at which the next compaction at that level should start.
        // Either an empty string, or a valid InternalKey.
        std::string compact_pointer_[config::kNumLevels];  // kNumLevels = 7

        // No copying allowed
        VersionSet(const VersionSet &);

        void operator=(const VersionSet &);
    };

// A Compaction encapsulates information about a compaction.
    class Compaction {
    public:
        ~Compaction();

        // Return the level that is being compacted.  Inputs from "level"
        // and "level+1" will be merged to produce a set of "level+1" files.
        int level() const { return level_; }

        // Return the object that holds the edits to the descriptor done
        // by this compaction.
        VersionEdit *edit() { return &edit_; }

        // "which" must be either 0 or 1
        int num_input_files(int which) const { return inputs_[which].size(); }

        // Return the ith input file at "level()+which" ("which" must be 0 or 1).
        FileMetaData *input(int which, int i) const { return inputs_[which][i]; }

        // Maximum size of files to build during this compaction.
        uint64_t MaxOutputFileSize() const { return max_output_file_size_; }

        // Is this a trivial compaction that can be implemented by just
        // moving a single input file to the next level (no merging or splitting)

        // 判断当前层是否能直接放到下一层, 没有合并操作和键值范围重合
        // 判断条件为:
        //      当前层数 level 文件数量为 1
        // &&   下层 level+1 文件数数量为 0
        // &&   level + 2 层所有文件站磁盘总字节数 小于 20MB(10*options->max_file_size)
        bool IsTrivialMove() const;

        // Add all inputs to this compaction as delete operations to *edit.
        // 把 level、level+1 层所有的文件标记为删除,
        // 函数的作用就是把 level、level+1 层所有的文件
        // 以 <level, FileMetaData.number> 格式添加到 VersionEdit.deleted_files_ 中
        void AddInputDeletions(VersionEdit *edit);

        // Returns true if the information we have available guarantees that
        // the compaction is producing data in "level+1" for which no data exists
        // in levels greater than "level+1".
        // 用于判断是否 DB::Impl 合并操作中，是否要放弃这个 user_key
        // 对于给定的 internal_key, 该函数确保该 internal_key 不会出现在 level[i]层(i>=level+2) 当中。
        bool IsBaseLevelForKey(const Slice &user_key);

        // Returns true iff we should stop building the current output
        // before processing "internal_key".
        // 避免
        //
        // 判断 output 是否过大(大于20MB)
        bool ShouldStopBefore(const Slice &internal_key);

        // Release the input version for the compaction, once the compaction
        // is successful.
        void ReleaseInputs();

    private:
        friend class Version;

        friend class VersionSet;

        Compaction(const Options *options, int level);

        int level_;     // 当前层, 将和 level + 1 层合并
        uint64_t max_output_file_size_;   // 2MB (options->max_file_size;)
        Version *input_version_;
        VersionEdit edit_;

        // Each compaction reads inputs from "level_" and "level_+1"
        std::vector<FileMetaData *> inputs_[2];      // The two sets of inputs

        // State used to check for number of of overlapping grandparent files
        // (parent == level_ + 1, grandparent == level_ + 2)
        std::vector<FileMetaData *> grandparents_;  // grandparent == level_+2
        size_t grandparent_index_;  // Index in grandparent_starts_
        bool seen_key_;             // Some output key has been seen
        int64_t overlapped_bytes_;  // Bytes of overlap between current output
        // and grandparent files

        // State for implementing IsBaseLevelForKey

        // level_ptrs_ holds indices into input_version_->levels_: our state
        // is that we are positioned at one of the file ranges for each
        // higher level than the ones involved in this compaction (i.e. for
        // all L >= level_ + 2).
        size_t level_ptrs_[config::kNumLevels]; // kNumLevels = 7
    };

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_DB_VERSION_SET_H_
