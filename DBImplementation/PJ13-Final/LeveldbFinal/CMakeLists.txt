cmake_minimum_required(VERSION 3.9)
project(test_read_sst)

set(CMAKE_CXX_STANDARD 11)

include_directories(db)
include_directories(helpers/memenv)
include_directories(include)
include_directories(include/leveldb)
include_directories(include/port)
include_directories(port)
include_directories(table)
include_directories(util)

include_directories(
        "${PROJECT_BINARY_DIR}/include"
        "${PROJECT_SOURCE_DIR}"
)

add_executable(test_read_sst
#        db/autocompact_test.cc
        db/builder.cc
        db/builder.h
        db/c.cc
#        db/c_test.c
#        db/corruption_test.cc

        db/db_impl.cc
        db/db_impl.h
        db/db_iter.cc
        db/db_iter.h
#        db/db_test.cc
        db/dbformat.cc
        db/dbformat.h
#        db/dbformat_test.cc
        db/dumpfile.cc
#        db/fault_injection_test.cc
        db/filename.cc
        db/filename.h
#        db/filename_test.cc
#        db/leveldbutil.cc
        db/log_format.h
        db/log_reader.cc
        db/log_reader.h
#        db/log_test.cc
        db/log_writer.cc
        db/log_writer.h
        db/memtable.cc
        db/memtable.h
#        db/recovery_test.cc
        db/repair.cc
        db/skiplist.h
#        db/skiplist_test.cc
        db/snapshot.h
        db/table_cache.cc
        db/table_cache.h
        db/version_edit.cc
        db/version_edit.h
#        db/version_edit_test.cc
        db/version_set.cc
        db/version_set.h
#        db/version_set_test.cc
        db/write_batch.cc
        db/write_batch_internal.h
#        db/write_batch_test.cc
#        doc/bench/db_bench_sqlite3.cc
#        doc/bench/db_bench_tree_db.cc
        helpers/memenv/memenv.cc
        helpers/memenv/memenv.h
#        helpers/memenv/memenv_test.cc
        include/leveldb/c.h
        include/leveldb/cache.h
        include/leveldb/comparator.h
        include/leveldb/db.h
        include/leveldb/dumpfile.h
        include/leveldb/env.h
        include/leveldb/export.h
        include/leveldb/filter_policy.h
        include/leveldb/iterator.h
        include/leveldb/options.h
        include/leveldb/slice.h
        include/leveldb/status.h
        include/leveldb/table.h
        include/leveldb/table_builder.h
        include/leveldb/write_batch.h
        include/port/port_config.h
#        issues/issue178_test.cc
#        issues/issue200_test.cc
        port/port.h
        port/port_example.h
        port/port_stdcxx.h
        port/thread_annotations.h
        table/block.cc
        table/block.h
        table/block_builder.cc
        table/block_builder.h
        table/filter_block.cc
        table/filter_block.h
#        table/filter_block_test.cc
        table/format.cc
        table/format.h
        table/iterator.cc
        table/iterator_wrapper.h
        table/merger.cc
        table/merger.h
        table/table.cc
        table/table_builder.cc
#        table/table_test.cc
        table/two_level_iterator.cc
        table/two_level_iterator.h
        util/arena.cc
        util/arena.h
#        util/arena_test.cc
        util/bloom.cc
#        util/bloom_test.cc
        util/cache.cc
#        util/cache_test.cc
        util/coding.cc
        util/coding.h
#        util/coding_test.cc
        util/comparator.cc
        util/crc32c.cc
        util/crc32c.h
#        util/crc32c_test.cc
        util/env.cc
        util/env_posix.cc
#        util/env_posix_test.cc
#        util/env_posix_test_helper.h
#        util/env_test.cc
#        util/env_windows.cc
#        util/env_windows_test.cc
#        util/env_windows_test_helper.h
        util/filter_policy.cc
        util/hash.cc
        util/hash.h
#        util/hash_test.cc
        util/histogram.cc
        util/histogram.h
        util/logging.cc
        util/logging.h
#        util/logging_test.cc
        util/mutexlock.h
        util/no_destructor.h
#        util/no_destructor_test.cc
        util/options.cc
        util/posix_logger.h
        util/random.h
        util/status.cc
#        util/status_test.cc
#        util/testharness.cc
#        util/testharness.h
#        util/testutil.cc
        util/testutil.h
#        util/windows_logger.h
#        db/db_bench.cc
        test/FinalDemo.cc
#        mytest.cpp
        )

#add_executable(db_bench db/db_bench.cc)
#add_executable(mytest mytest.cpp)

add_executable(multimap
        "${PROJECT_SOURCE_DIR}/test/multimap.cc"
        )
target_link_libraries(multimap leveldb)

add_executable(read_sst
        "${PROJECT_SOURCE_DIR}/test/read_sst.cc"
        )
target_link_libraries(read_sst leveldb)

add_executable(sstMaxMin
        "${PROJECT_SOURCE_DIR}/test/sstMaxMin.cc"
        )
target_link_libraries(sstMaxMin leveldb)

find_package(Threads REQUIRED)
target_link_libraries(test_read_sst Threads::Threads)

#target_link_libraries(db_bench Threads::Threads)
#target_link_libraries(mytest Threads::Threads)