#include "test.h"

int main(int argc, char * argv[])
{
    test_string();
    test_hex();
    test_base64();
    test_exception();
    test_bytestream();
    test_memory();
    test_locker();
    test_guard();
    test_thread();
    test_object_pool();
    test_time();
    test_log();
    test_directory();
    test_ini();
    test_single_timer();
    test_multi_timer();
    test_id_manager();
    test_singleton();
    test_splitter();
    test_bcd();
    test_charset();
    test_counter();
    test_bitmap();
    test_thread_local_storage();
    test_utility();
    test_xml();
    test_library();
    test_file();
    test_hardware();
    test_process();
    test_json();
    test_sqlite();
    test_hash();
    test_pipe();
    return (0);
}
