#include <q1x/test/test.h>

#include <q1x/io/file.h>
#include <q1x/exchange/timestamp.h>

TEST_CASE("touch-empty", "[io]") {
    std::string filename = "123.txt";
    io::write_file(filename);
}

TEST_CASE("touch-read-ftime", "[io]") {
    std::string filename = "123.txt";
    int64_t ms = io::last_modified_time(filename);
    exchange::timestamp ts = ms;
    std::cout<< ts << std::endl;
}

TEST_CASE("touch-write-ftime", "[io]") {
    std::string filename = "123.txt";
    auto mtime =  exchange::timestamp::now().pre_market_time();
    io::last_modified_time(filename, mtime);
}